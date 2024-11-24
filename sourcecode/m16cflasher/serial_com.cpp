/*
	MIT License

	Copyright (c) 2024 Truong Hy

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "serial_com.h"
#include "tru_exception.h"
#include <stdio.h>

#if defined(WIN32) || defined(WIN64)

// =======
// Windows
// =======

serial_com::serial_com() :
	fd(INVALID_HANDLE_VALUE),
	is_rd_timed_out(false){
	dcb.DCBlength = sizeof(dcb);
	memset(&timeouts, 0, sizeof(timeouts));
}

serial_com::~serial_com(){
	close_handle();
}

void serial_com::close_handle(){
	if(fd != INVALID_HANDLE_VALUE){
		if(CloseHandle(fd)){
			fd = INVALID_HANDLE_VALUE;
		}else{
			throw tru_exception::get_os_last_error(__func__, "");
		}
	}
}

/*
	Opens a device by TCHAR string (WCHAR if UNICODE) path.
	Example device path of serial com port number 0: \\.\COM0
*/
void serial_com::open_handle(const TCHAR *path)
{
	close_handle();

#ifdef USE_ASYNC_READ_WRITE
	fd =
		CreateFile(
			path,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL
		);
#else
	fd =
		CreateFile(
			path,
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
#endif
	if (fd == INVALID_HANDLE_VALUE) throw tru_exception::get_os_last_error(__func__, path);  // Invalid handle?
}

/*
	Opens a device by char string (single byte characters) path.
	Example device path of serial com port number 0: \\.\COM0
*/
void serial_com::open_handle(std::string path)
{
	tc_string tc_str;

	// Convert path stored as std::string to TCHAR string
	tc_str = string_utils_ns::str_to_tc(path);
	open_handle(tc_str.c_str());
}

void serial_com::clear_comm_error(){
	DWORD com_errors;
	COMSTAT com_stat;

	ClearCommError(fd, &com_errors, &com_stat);
	PurgeComm(fd, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

void serial_com::set_params(uint32_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits, bool rtscts_en){
	if(!GetCommState(fd, &dcb)){
		throw tru_exception::get_os_last_error(__func__, "");
	}

	dcb.BaudRate = baud_rate;
	dcb.ByteSize = byte_size;
	dcb.Parity = parity;
	dcb.StopBits = stop_bits;
	dcb.fParity = (parity == NOPARITY) ? false : true;
	dcb.fBinary = true;
	dcb.fOutxCtsFlow = rtscts_en;
	dcb.fOutxDsrFlow = false;
	dcb.fDsrSensitivity = false;
	dcb.fTXContinueOnXoff = false;
	dcb.fOutX = false;
	dcb.fInX = false;
	dcb.fRtsControl = rtscts_en ? RTS_CONTROL_ENABLE : RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fNull = false;
	dcb.fAbortOnError = false;
	dcb.fErrorChar = false;

	if(!SetCommState(fd, &dcb)){
		throw tru_exception::get_os_last_error(__func__, "");
	}
}

void serial_com::set_timeout(uint32_t timeout_ms){
	if(!GetCommTimeouts(fd, &timeouts)){
		throw tru_exception::get_os_last_error(__func__, "");
	}

	//timeouts.ReadIntervalTimeout = MAXDWORD;
	//timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.ReadTotalTimeoutConstant = timeout_ms;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = timeout_ms;

	if(!SetCommTimeouts(fd, &timeouts)){
		throw tru_exception::get_os_last_error(__func__, "");
	}
}

#ifdef USE_ASYNC_READ_WRITE

DWORD serial_com::read_port(void *buf, uint32_t len){
	DWORD bytes_read = 0;
	DWORD overlapped_bytes_read;
	OVERLAPPED overlapped;
	DWORD wait_result = WAIT_OBJECT_0;
	BOOL result = true;
	DWORD read_error;

	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = CreateEvent(NULL, true, false, NULL);
	// Bad result?
	if(!overlapped.hEvent){
		result = false;
	}else{
		// Read device asynchronously
		result = ReadFile(fd, buf, len, &bytes_read, &overlapped);
		// Bad result?
		if(!result){
			read_error = GetLastError();
			switch(read_error){
				case ERROR_HANDLE_EOF:  // End of file reached!
					break;

				case ERROR_IO_PENDING:  // IO is still pending
					// Wait some time until overlapped read event is triggered or timed-out
					wait_result = WaitForSingleObject(overlapped.hEvent, len * timeouts.ReadTotalTimeoutMultiplier + timeouts.ReadTotalTimeoutConstant);
					switch(wait_result){
						case WAIT_OBJECT_0:  // Overlapped read event was triggered
							// Get overlapped result info
							result = GetOverlappedResult(fd, &overlapped, &overlapped_bytes_read, false);
							if(!result){
								read_error = GetLastError();
								switch(read_error){
									case ERROR_HANDLE_EOF: break;  // End of file reached!
									case ERROR_IO_INCOMPLETE: break;  // IO is still pending
									case ERROR_IO_PENDING: CancelIo(fd); break;  // IO is still pending, cancel the read
								}
							}
							bytes_read += overlapped_bytes_read;
							break;

						case WAIT_ABANDONED:
						case WAIT_TIMEOUT:  // Timed-out
							// Cancel the read
							CancelIo(fd);
							break;

						case WAIT_FAILED:  // WaitForSingleObject error!
							break;
					}
					break;
			}
		}

		CloseHandle(overlapped.hEvent);
	}

	if(wait_result == WAIT_ABANDONED){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_WAITABANDONED_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_WAITABANDONED_ID], "");
	}else if(wait_result == WAIT_TIMEOUT){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");
	}else{
		if(!result) throw tru_exception::get_os_last_error(__func__, "");
	}

	return bytes_read;
}

DWORD serial_com::write_port(void *buf, uint32_t len){
	DWORD overlapped_bytes_written;
	OVERLAPPED overlapped;
	DWORD wait_result = WAIT_OBJECT_0;
	BOOL result = true;
	DWORD write_error;
	DWORD bytes_written = 0;

	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent = CreateEvent(NULL, true, false, NULL);
	if(!overlapped.hEvent){
		result = false;
	}else{
		// Write device asynchronously
		result = WriteFile(fd, buf, len, &bytes_written, &overlapped);
		// Bad result?
		if(!result){
			write_error = GetLastError();
			switch(write_error){
				case ERROR_IO_PENDING:  // IO is still pending
					// Wait some time until overlapped read event is triggered or timed-out
					wait_result = WaitForSingleObject(overlapped.hEvent, len * timeouts.WriteTotalTimeoutMultiplier + timeouts.WriteTotalTimeoutConstant);
					switch(wait_result){
						case WAIT_OBJECT_0:  // Overlapped read event was triggered
							// Get overlapped result info
							result = GetOverlappedResult(fd, &overlapped, &overlapped_bytes_written, false);
							// Bad result?
							if(!result){
								write_error = GetLastError();
								switch(write_error){
									case ERROR_IO_INCOMPLETE: break;  // IO is still pending
									case ERROR_IO_PENDING: CancelIo(fd); break;  // IO is still pending, cancel the read
								}
							}
							bytes_written += overlapped_bytes_written;
							break;

						case WAIT_ABANDONED:
						case WAIT_TIMEOUT:  // Timed-out
							// Cancel the write
							CancelIo(fd);
							break;


						case WAIT_FAILED:  // WaitForSingleObject error!
							break;
					}
					break;
			}
		}

		CloseHandle(overlapped.hEvent);
	}

	if(wait_result == WAIT_ABANDONED){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_WAITABANDONED_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_WAITABANDONED_ID], "");
	}else if(wait_result == WAIT_TIMEOUT){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");
	}else{
		if(!result) throw tru_exception::get_os_last_error(__func__, "");
	}

	return bytes_written;
}

#else

/*
DWORD serial_com::read_port(void *buf, uint32_t len){
	ULONGLONG prev_time;
	DWORD com_errors;
	COMSTAT com_stat;
	DWORD bytes_read;
	DWORD total_bytes_read = 0;
	LPVOID data_buf = buf;

	is_rd_timed_out = false;

	prev_time = GetTickCount64();
	while(total_bytes_read < len){
		//Get and clear current errors on the port
		if(!ClearCommError(fd, &com_errors, &com_stat)){
			throw tru_exception::get_os_last_error(__func__, "");
		}

		bytes_read = 0;
		// Remaining requested length is bigger than the currently available data in the serial read queue?
		if((len - total_bytes_read) > com_stat.cbInQue){
			// Read upto the serial read queue length
			if(!ReadFile(fd, data_buf, com_stat.cbInQue, &bytes_read, NULL)){
				throw tru_exception::get_os_last_error(__func__, "");
			}
		}else{
			// Read upto the remaining requested length
			if(!ReadFile(fd, data_buf, len - total_bytes_read, &bytes_read, NULL)){
				throw tru_exception::get_os_last_error(__func__, "");
			}
		}

		// Update buffer pointer position
		data_buf = (char*)data_buf + bytes_read;
		// Update total bytes read
		total_bytes_read += bytes_read;

		// Timed out?
		if(((prev_time + timeouts.ReadTotalTimeoutConstant) < GetTickCount64()) && (timeouts.ReadTotalTimeoutConstant != 0)){
			is_rd_timed_out = true;
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");
		}
	}

	return total_bytes_read;
}
*/

DWORD serial_com::read_port(void *buf, uint32_t len){
	DWORD bytes_read = 0;

	if(!ReadFile(fd, buf, len, &bytes_read, NULL)){
		throw tru_exception::get_os_last_error(__func__, "");
	}

	return bytes_read;
}

DWORD serial_com::write_port(void *buf, uint32_t len){
	DWORD bytes_written = 0;

	if(!WriteFile(fd, buf, len, &bytes_written, NULL)){
		throw tru_exception::get_os_last_error(__func__, "");
	}

	return bytes_written;
}

#endif

void serial_com::purge(){
	DWORD rc;

	rc = PurgeComm(fd, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	if(!rc){
		throw tru_exception::get_os_last_error(__func__, "");
	}
}

#else

// =====
// Linux
// =====

#include <sys/ioctl.h>

serial_com::serial_com() :
	fd(-1){
}

serial_com::~serial_com(){
	close_handle();
}

void serial_com::close_handle(){
	if(fd > 0){
		if(close(fd)){
			throw tru_exception::get_clib_last_error(__func__, "");
		}else{
			fd = -1;
		}
	}
}

speed_t serial_com::baud_rate_to_code(uint32_t baud_rate){
	switch(baud_rate){
		case 0: return B0; break;
		case 50: return B50; break;
		case 75: return B75; break;
		case 110: return B110; break;
		case 134: return B134; break;
		case 150: return B150; break;
		case 200: return B200; break;
		case 300: return B300; break;
		case 600: return B600; break;
		case 1200: return B1200; break;
		case 1800: return B1800; break;
		case 2400: return B2400; break;
		case 4800: return B4800; break;
		case 9600: return B9600; break;
		case 19200: return B19200; break;
		case 38400: return B38400; break;
		case 57600: return B57600; break;
		case 115200: return B115200; break;
		case 230400: return B230400; break;
		case 460800: return B460800; break;
		case 500000: return B500000; break;
		case 576000: return B576000; break;
		case 921600: return B921600; break;
		case 1000000: return B1000000; break;
		case 1152000: return B1152000; break;
		case 1500000: return B1500000; break;
		case 2000000: return B2000000; break;
		case 2500000: return B2500000; break;
		case 3000000: return B3000000; break;
		case 3500000: return B3500000; break;
		case 4000000: return B4000000; break;
		default: return baud_rate;
	}
}

uint8_t serial_com::byte_size_to_code(uint8_t byte_size){
	switch(byte_size){
		case 5: return CS5; break;
		case 6: return CS6; break;
		case 7: return CS7; break;
		case 8: return CS8; break;
	}
	return CS5;
}

void serial_com::set_params(uint32_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits, bool rtscts_en){
	// Set port settings using termios
	struct termios tio;
	if(tcgetattr(fd, &tio)) throw tru_exception::get_clib_last_error(__func__, "");
	// Set various settings
	tio.c_cflag &= ~CSIZE;  // Clear all the size bits first
	tio.c_cflag |= byte_size_to_code(byte_size);  // Set number of bits per byte
	//tio.c_cflag |= byte_size;  // Set number of bits per byte
	tio.c_cflag = (parity == NOPARITY) ? tio.c_cflag & ~PARENB : tio.c_cflag | PARENB;  // Set parity on or off
	tio.c_cflag = (stop_bits == TWOSTOPBITS) ? tio.c_cflag | CSTOPB : tio.c_cflag & ~CSTOPB;  // Set two stop bits else one stop bit
	tio.c_cflag = rtscts_en ? tio.c_cflag | CRTSCTS : tio.c_cflag & ~CRTSCTS;  // Set hardware RTS/CTS flow control
	tio.c_iflag &= ~(IXON | IXOFF | IXANY);  // Turn off software flow control
	// Turn off settings that might interfere with send and receive raw binary bytes
	tio.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tio.c_lflag &= ~(ECHO | ~ECHOE | ECHONL | ICANON | ISIG | IEXTEN);
	tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
	tio.c_oflag &= ~(ONLCR | OCRNL | OPOST);
	//tio.c_oflag &= ~OXTABS;  // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
	//tio.c_oflag &= ~ONOEOT;  // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)
	// Set baud rate
	//cfsetispeed(&tio, baud_rate_to_code(baud_rate));
	//cfsetospeed(&tio, baud_rate_to_code(baud_rate));
	cfsetspeed(&tio, baud_rate_to_code(baud_rate));
	if(tcsetattr(fd, TCSANOW, &tio)) throw tru_exception::get_clib_last_error(__func__, "");
}

void serial_com::set_timeout(uint32_t timeout_ms){
	struct termios tio;

	if(tcgetattr(fd, &tio) != 0) {
		throw tru_exception::get_clib_last_error(__func__, "");
	}

	// Note these are size of 8-bits, i.e. max value = 255
	tio.c_cc[VTIME] = timeout_ms / 100;  // Wait for up to deciseconds (e.g. 10 = 1 second), returning as soon as any data is received.
	tio.c_cc[VMIN] = 0;

	if(tcsetattr(fd, TCSANOW, &tio) != 0){
		throw tru_exception::get_clib_last_error(__func__, "");
	}
}

void serial_com::set_wait(uint32_t min_chars){
	struct termios tio;

	if(tcgetattr(fd, &tio) != 0) {
		throw tru_exception::get_clib_last_error(__func__, "");
	}

	tio.c_cc[VMIN] = min_chars;

	if(tcsetattr(fd, TCSANOW, &tio) != 0){
		throw tru_exception::get_clib_last_error(__func__, "");
	}
}

/*
	Example device path of serial com port number 0: /dev/ttyACM0
*/
void serial_com::open_handle(std::string path){
	close_handle();

	fd = open(path.c_str(), O_RDWR | O_NOCTTY);
	if(fd < 0) throw tru_exception::get_clib_last_error(__func__, path);  // Invalid handle
}

void serial_com::clear_comm_error(){
}

void serial_com::purge(){
	int rc = tcflush(fd, TCIOFLUSH);
	// Alternatives
	//rc = ioctl(fd, TCFLSH, 0); // Flush receive
	//rc = ioctl(fd, TCFLSH, 1); // Flush transmit
	//rc = ioctl(fd, TCFLSH, 2); // Flush both

	if(rc) throw tru_exception::get_clib_last_error(__func__, "");
}

ssize_t serial_com::read_port(void *buf, uint32_t len){
/*
	uint8_t *p = (uint8_t *)buf;
	uint32_t remain = len;
	uint8_t vmin;
	struct termios tio;
	ssize_t n;

	if(tcgetattr(fd, &tio)) throw tru_exception::get_clib_last_error(__func__, "");

	vmin = (remain > 255) ? 255 : remain;
	tio.c_cc[VMIN] = vmin;
	if(tcsetattr(fd, TCSANOW, &tio)) throw tru_exception::get_clib_last_error(__func__, "");

	n = read(fd, p, remain);
	if(n < 0) throw tru_exception::get_clib_last_error(__func__, "");
	p += n;
	remain -= n;

	while(remain){
		vmin = (remain > 255) ? 255 : remain;
		if(vmin < 255){
			tio.c_cc[VMIN] = vmin;
			if(tcsetattr(fd, TCSANOW, &tio)) throw tru_exception::get_clib_last_error(__func__, "");
		}

		n = read(fd, p, remain);
		if(n <= 0) throw tru_exception::get_clib_last_error(__func__, "");
		p += n;
		remain -= n;
	}

	return len - remain;
*/

	uint8_t *p = (uint8_t *)buf;
	uint32_t remain = len;
	ssize_t n;

	n = read(fd, p, remain);
	if(n < 0) throw tru_exception::get_clib_last_error(__func__, "");
	if(n <= 0) throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");
	p += n;
	remain -= n;

	while(remain){
		n = read(fd, p, remain);
		if(n < 0) throw tru_exception::get_clib_last_error(__func__, "");
		if(n <= 0) throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");
		p += n;
		remain -= n;
	}

	return len - remain;
}

ssize_t serial_com::write_port(void *buf, uint32_t len){
	ssize_t n = write(fd, buf, len);
	if(n < 0) throw tru_exception::get_clib_last_error(__func__, "");
	if(n <= 0) throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, SERIALCOMM_ERROR_TIMEDOUT_ID, serialcomm_error_string::messages[SERIALCOMM_ERROR_TIMEDOUT_ID], "");

	int result = tcdrain(fd);
	if(result) throw tru_exception::get_clib_last_error(__func__, "");

	return n;
}

#endif
