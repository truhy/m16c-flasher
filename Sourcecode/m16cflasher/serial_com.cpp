#include "serial_com.h"

cl_serial_com::cl_serial_com() :
	m_dev_handle(INVALID_HANDLE_VALUE),
	m_is_rd_timed_out(FALSE){
	m_dcb.DCBlength = sizeof(m_dcb);
	memset(&m_timeouts, 0, sizeof(m_timeouts));
}

cl_serial_com::~cl_serial_com(){
	close_handle();
}

std::string cl_serial_com::last_err_msg(){
	return m_last_err_msg;
}

size_t cl_serial_com::last_err_msg_len(){
	return m_last_err_msg.length();
}

void cl_serial_com::clear_last_err_msg(){
	m_last_err_msg.clear();
}

my_bool cl_serial_com::close_handle(){
	if(m_dev_handle != INVALID_HANDLE_VALUE){
		if(CloseHandle(m_dev_handle)){
			m_dev_handle = INVALID_HANDLE_VALUE;
		}else{
			return FALSE;
		}
	}

	return TRUE;
}

my_bool cl_serial_com::open_handle(const TCHAR* m_arg_dev_path)
{
	m_last_err_msg.clear();

	if(close_handle()){
		m_dev_handle =
			CreateFile(
				m_arg_dev_path,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);
	}

	//Invalid handle?
	if(m_dev_handle == INVALID_HANDLE_VALUE){
		m_last_err_msg = "Error: could not open device!";
		return FALSE;
	}

	return TRUE;
}

/*
	Opens a device by string of single byte characters.

	Examples:
		Device path of serial com port number 1:
		\\.\COM1
*/
my_bool cl_serial_com::open_by_str(std::string m_arg_dev_path)
{
	tc_string m_tc_str;

	//Convert path stored as std::string to TCHAR string
	m_tc_str = ns_tc_string_utils::str_to_tc(m_arg_dev_path);
	return open_handle(m_tc_str.c_str());
}

void cl_serial_com::clear_last_err(){
	DWORD m_com_errors;
	COMSTAT m_com_stat;

	m_last_err_msg.clear();
	ClearCommError(m_dev_handle, &m_com_errors, &m_com_stat);
	PurgeComm(m_dev_handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

my_bool cl_serial_com::set_params(DWORD m_arg_baud_rate){
	if(!GetCommState(m_dev_handle, &m_dcb)){
		m_last_err_msg = "Error: could not get current serial com state parameters!";
		return FALSE;
	}

	m_dcb.BaudRate = m_arg_baud_rate;
	//Defaults of M16C boot loader
	m_dcb.ByteSize = 8;
	m_dcb.Parity = NOPARITY;
	m_dcb.StopBits = ONESTOPBIT;

	//Disable control flow
	m_dcb.fOutxCtsFlow = FALSE;
	m_dcb.fOutxDsrFlow = FALSE;
	m_dcb.fDsrSensitivity = FALSE;
	m_dcb.fTXContinueOnXoff = FALSE;
	m_dcb.fOutX = FALSE;
	m_dcb.fInX = FALSE;
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;

	if(!SetCommState(m_dev_handle, &m_dcb)){
		m_last_err_msg = "Error: could not set serial com state parameters!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_serial_com::set_timeout(DWORD m_arg_timeout_ms){
	if(!GetCommTimeouts(m_dev_handle, &m_timeouts)){
		m_last_err_msg = "Error: could not get current serial com timeouts parameters!";
		return FALSE;
	}

	m_timeouts.ReadIntervalTimeout = 50;
	m_timeouts.ReadTotalTimeoutMultiplier = 10;
	m_timeouts.ReadTotalTimeoutConstant = m_arg_timeout_ms;
	m_timeouts.WriteTotalTimeoutMultiplier = 0;
	m_timeouts.WriteTotalTimeoutConstant = 0;

	if(!SetCommTimeouts(m_dev_handle, &m_timeouts)){
		m_last_err_msg = "Error: could not set current serial com time outs parameters!";
		return FALSE;
	}

	return TRUE;
}

/*
DWORD cl_serial_com::read(void* m_arg_buf, my_uint32 m_arg_req_len){
	DWORD m_prev_time;
	DWORD m_bytes_rd;
	DWORD m_total_bytes_rd = 0;

	LPVOID m_buf = m_arg_buf;

	m_last_err_msg.clear();
	m_is_rd_timed_out = FALSE;

	m_prev_time = GetTickCount();
	while(m_total_bytes_rd < m_arg_req_len){
		if(!ReadFile(m_dev_handle, m_buf, m_arg_req_len - m_total_bytes_rd, &m_bytes_rd, NULL)){
			m_last_err_msg = "Error: read serial data failed!";
			return m_total_bytes_rd;
		}

		//Update buffer pointer position
		m_buf = (char*)m_buf + m_bytes_rd;
		//Update total bytes read
		m_total_bytes_rd += m_bytes_rd;

		//Timed out?
		if(((m_prev_time + m_timeouts.ReadTotalTimeoutConstant) < GetTickCount()) && (m_timeouts.ReadTotalTimeoutConstant != 0)){
			m_last_err_msg = "Error: read serial data timed out!";
			m_is_rd_timed_out = TRUE;
			return m_total_bytes_rd;
		}
	}

	return m_total_bytes_rd;
}
*/

DWORD cl_serial_com::read(void* m_arg_buf, my_uint32 m_arg_req_len){
	DWORD m_prev_time;
	DWORD m_com_errors;
	COMSTAT m_com_stat;
	DWORD m_bytes_rd;
	DWORD m_total_bytes_rd = 0;
	LPVOID m_buf = m_arg_buf;

	m_last_err_msg.clear();
	m_is_rd_timed_out = FALSE;

	m_prev_time = GetTickCount();
	while(m_total_bytes_rd < m_arg_req_len){
		//Get and clear current errors on the port
		if(!ClearCommError(m_dev_handle, &m_com_errors, &m_com_stat)){
			m_last_err_msg = "Error: ClearCommError failed!";
			return m_total_bytes_rd;
		}

		if(m_com_errors){
			switch(m_com_errors){
				case CE_BREAK:{
					m_last_err_msg = "Error: the hardware detected a break condition!";
					break;
				}
				case CE_FRAME:{
					m_last_err_msg = "Error: the hardware detected a framing error!";
					break;
				}
				case CE_OVERRUN:{
					m_last_err_msg = "Error: a character-buffer overrun has occurred!";
					break;
				}
				case CE_RXOVER:{
					m_last_err_msg = "Error: an input buffer overflow has occurred!";
					break;
				}
				case CE_RXPARITY:{
					m_last_err_msg = "Error: the hardware detected a parity error!";
					break;
				}
				default:{
					m_last_err_msg = "Error: unknown serial comm error!";
				}
			}
			return m_total_bytes_rd;
		}

		m_bytes_rd = 0;
		//Remaining requested length is bigger than the currently available data in the serial read queue?
		if((m_arg_req_len - m_total_bytes_rd) > m_com_stat.cbInQue){
			//Read upto the serial read queue length
			if(!ReadFile(m_dev_handle, m_buf, m_com_stat.cbInQue, &m_bytes_rd, NULL)){
				m_last_err_msg = "Error: read serial data failed!";
				return m_total_bytes_rd;
			}
		}else{
			//Read upto the remaining requested length
			if(!ReadFile(m_dev_handle, m_buf, m_arg_req_len - m_total_bytes_rd, &m_bytes_rd, NULL)){
				m_last_err_msg = "Error: read serial data failed!";
				return m_total_bytes_rd;
			}
		}

		//Update buffer pointer position
		m_buf = (char*)m_buf + m_bytes_rd;
		//Update total bytes read
		m_total_bytes_rd += m_bytes_rd;

		//Timed out?
		if(((m_prev_time + m_timeouts.ReadTotalTimeoutConstant) < GetTickCount()) && (m_timeouts.ReadTotalTimeoutConstant != 0)){
			m_last_err_msg = "Error: read serial data timed out!";
			m_is_rd_timed_out = TRUE;
			return m_total_bytes_rd;
		}
	}

	return m_total_bytes_rd;
}

DWORD cl_serial_com::write(void* m_arg_buf, my_uint32 m_arg_wr_len){
	DWORD m_bytes_written = 0;

	m_last_err_msg.clear();

	if(!WriteFile(m_dev_handle, m_arg_buf, m_arg_wr_len, &m_bytes_written, NULL)){
		m_last_err_msg = "Error: write serial data failed!";
	}

	return m_bytes_written;
}
