#include "m16c_cmds.h"
#include "app_error_string.h"
#include <format>

// For the Sleep/sleep function
#if defined(WIN32) || defined(WIN64)
#include <Windows.h>
#else
#include <unistd.h>
#endif

cl_m16c_cmd::cl_m16c_cmd(){
}

// Older boot loaders only support 9600
unsigned char cl_m16c_cmd::baud_rate_cmd_code(uint32_t arg_baud_rate){
	switch(arg_baud_rate){
		case 9600: return 0xB0; break;
		case 19200: return 0xB1; break;
		case 38400: return 0xB2; break;
		case 57600: return 0xB3; break;
		case 115200: return 0xB4; break;
		default: return 0xB0;
	}
	return 0;
}

void cl_m16c_cmd::auto_baud(serial_com &arg_serial_com) {
	unsigned char null_byte = 0;
	unsigned char rx_data;

	// Send a 0x00 byte 2*16 times with 40ms delay (double the number of times specified in "Easy R8C/M16C/M32C/R32C Flash Programming" PDF, but needed on the tested M30260F8)
	for (int i = 0; i < 32; i++)
	{
		if (arg_serial_com.write_port(&null_byte, 1) < 1) {
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}

#if defined(WIN32) || defined(WIN64)
		Sleep(SLEEP_AUTO_BAUD_INTERVAL);
#else
		usleep(SLEEP_AUTO_BAUD_INTERVAL * 1000);
#endif
	}

	// Read back and throw away the value to make sure no junk is left unread
	arg_serial_com.read_port(&rx_data, 1);
}

void cl_m16c_cmd::set_baud_rate(serial_com &arg_serial_com, uint32_t arg_baud_rate){
	unsigned char baud_code = baud_rate_cmd_code(arg_baud_rate);
	unsigned char rx_data;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send set baud rate command
	if(arg_serial_com.write_port(&baud_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// If successful the M16C boot loader will echo the command code
	if(arg_serial_com.read_port(&rx_data, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}

	// The echo is same as the one sent?
	if(rx_data != baud_code){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_ECHO_ID, app_error_string::messages[APP_ERROR_ECHO_ID], std::format(app_error_string::messages[APP_ERROR_ECHO_INFO_ID], baud_code, rx_data));
	}
}

void cl_m16c_cmd::rd_version(serial_com &arg_serial_com){
	unsigned char cmd_code = M16C_BL_CMD_CODE_VERSION;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send read boot loader version command
	if(arg_serial_com.write_port(&cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(ver, 0, 8);
	// Read boot loader version data
	if(arg_serial_com.read_port(ver, 8) < 8){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::rd_status(serial_com &arg_serial_com){
	unsigned char cmd_code = M16C_BL_CMD_CODE_READ_STATUS_REG;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send read status register command
	if(arg_serial_com.write_port(&cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(srd, 0, 2);
	// Read status register data
	if(arg_serial_com.read_port(srd, 2) < 2){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::id_chk(serial_com &arg_serial_com, uint32_t arg_id_addr, std::string arg_id_hex_str){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_ID_CHECK,
		(unsigned char)arg_id_addr,
		(unsigned char)(arg_id_addr >> 8),
		(unsigned char)(arg_id_addr >> 16),
		7,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	uint16_t num_of_hex_bytes = (uint16_t)(arg_id_hex_str.length() / 2);

	if(num_of_hex_bytes > 7){
		num_of_hex_bytes = 7;
	}

	for(uint16_t i = 0; i < num_of_hex_bytes; ++i){
		cmd[5 + i] = (unsigned char)strtoul(arg_id_hex_str.substr(2 * i, 2).c_str(), NULL, 16);
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send ID check command + addr + ID one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::page_rd(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_PAGE_READ,
		(unsigned char)(m_arg_from_addr >> 8),
		(unsigned char)(m_arg_from_addr >> 16)
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send page read command + addr one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(m_arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(m_arg_buf_256, 0, 256);
	// Read 256 bytes
	if(m_arg_serial_com.read_port(m_arg_buf_256, 256) < 256){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::clear_status(serial_com &arg_serial_com){
	unsigned char cmd_code = M16C_BL_CMD_CODE_CLEAR_STATUS_REG;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(arg_serial_com.write_port(&cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

/*
	Program (i.e. flash) a page (256 bytes).
	The from address must be an address at the beginning of a page, i.e. multiple of 256 from the start of a block.
*/
void cl_m16c_cmd::page_program(serial_com &arg_serial_com, uint32_t arg_from_addr, void *arg_buf_256){
	cl_my_buf cmd;

	cmd.alloc_buf(259);

	cmd.buf_ptr()[0] = M16C_BL_CMD_CODE_PAGE_PROGRAM;
	cmd.buf_ptr()[1] = (unsigned char)(arg_from_addr >> 8);
	cmd.buf_ptr()[2] = (unsigned char)(arg_from_addr >> 16);
	memcpy(cmd.buf_ptr() + 3, arg_buf_256, 256);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + data one byte at a time
	for(uint16_t i = 0; i < 259; ++i){
		if(arg_serial_com.write_port(cmd.buf_ptr() + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::erase_block(serial_com &arg_serial_com, uint32_t arg_block_highest_addr){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_BLOCK_ERASE,
		(unsigned char)(arg_block_highest_addr >> 8),
		(unsigned char)(arg_block_highest_addr >> 16),
		0xD0
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::erase_all_unlocked(serial_com& m_arg_serial_com){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_ERASE_ALL_UNLOCKED,
		0xD0
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + parameter one byte at a time
	for(uint16_t i = 0; i < sizeof(m_cmd); ++i){
		if(m_arg_serial_com.write_port(m_cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::read_lock_bit(serial_com &arg_serial_com, uint32_t arg_block_highest_addr){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_READ_LOCK_BIT_STATUS,
		(unsigned char)(arg_block_highest_addr >> 8),
		(unsigned char)(arg_block_highest_addr >> 16)
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr  one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}

	lock_bit = 0;
	// Read
	if(arg_serial_com.read_port(&lock_bit, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

/*
	This sets the specified block with a lock bit flag 0 (locked).
	To reset the lock bit flag to 1 (unlocked) send the lock bit disable command and then an erase command.
*/
void cl_m16c_cmd::lock_bit_program(serial_com &arg_serial_com, uint32_t arg_block_highest_addr){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_LOCK_BIT_PROGRAM,
		(unsigned char)(arg_block_highest_addr >> 8),
		(unsigned char)(arg_block_highest_addr >> 16),
		0xD0
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::lock_bit_enable(serial_com &arg_serial_com){
	unsigned char cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_ENABLE;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(arg_serial_com.write_port(&cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::lock_bit_disable(serial_com &arg_serial_com){
	unsigned char cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_DISABLE;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(arg_serial_com.write_port(&cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

unsigned char cl_m16c_cmd::calc_checksum(uint16_t arg_buf_len, void *arg_buf){
	unsigned char checksum = 0;
	uint32_t i;

	for(i = 0; i < arg_buf_len; ++i){
		checksum += ((unsigned char*)arg_buf)[i];
	}

	return checksum;
}

void cl_m16c_cmd::download_calc_checksum(serial_com &arg_serial_com, uint16_t arg_buf_len, void *arg_buf){
	cl_my_buf cmd;
	uint32_t cmd_buf_max_len = 256 + 4;
	uint16_t total_buf_sent = 0;
	uint16_t buf_len_to_send;
	uint16_t header_len = 4;
	uint32_t tx_len;

	cmd.alloc_buf(cmd_buf_max_len);

	cmd.buf_ptr()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	cmd.buf_ptr()[1] = (unsigned char)arg_buf_len;
	cmd.buf_ptr()[2] = (unsigned char)(arg_buf_len >> 8);
	cmd.buf_ptr()[3] = calc_checksum(arg_buf_len, arg_buf);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	while(total_buf_sent < arg_buf_len){
		if((arg_buf_len - total_buf_sent) > 256){
			buf_len_to_send = 256;
		}else{
			buf_len_to_send = arg_buf_len;
		}

		memcpy(cmd.buf_ptr() + header_len, (unsigned char*)arg_buf + total_buf_sent, buf_len_to_send);

		// Send data one byte at a time
		tx_len = header_len + buf_len_to_send;
		for(uint16_t i = 0; i < tx_len; ++i){
			if(arg_serial_com.write_port(cmd.buf_ptr() + i, 1) < 1){
				throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
			}
		}

		if(total_buf_sent == 0){
			header_len = 0;
		}
		total_buf_sent += buf_len_to_send;
	}
}

void cl_m16c_cmd::download(serial_com &arg_serial_com, unsigned char arg_checksum, uint16_t arg_buf_len, void *arg_buf){
	cl_my_buf cmd;
	uint32_t cmd_buf_len = arg_buf_len + 4;

	cmd.alloc_buf(cmd_buf_len);

	cmd.buf_ptr()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	cmd.buf_ptr()[1] = (unsigned char)arg_buf_len;
	cmd.buf_ptr()[2] = (unsigned char)(arg_buf_len >> 8);
	cmd.buf_ptr()[3] = arg_checksum;
	memcpy(cmd.buf_ptr() + 4, arg_buf, arg_buf_len);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + data one byte at a time
	for(uint16_t i = 0; i < cmd_buf_len; ++i){
		if(arg_serial_com.write_port(cmd.buf_ptr() + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::boot_area_output(serial_com &arg_serial_com, uint32_t arg_from_addr, void *arg_buf_256){
	unsigned char cmd[] = {
		M16C_BL_CMD_CODE_BOOT_AREA_OUTPUT,
		(unsigned char)(arg_from_addr >> 8),
		(unsigned char)(arg_from_addr >> 16)
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t i = 0; i < sizeof(cmd); ++i){
		if(arg_serial_com.write_port(cmd + i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(arg_buf_256, 0, 256);
	// Read 256 bytes
	if(arg_serial_com.read_port(arg_buf_256, 256) < 256){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

bool cl_m16c_cmd::is_srd_id_verified(){
	if((srd[1] & 0x0C) == 0x0C){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_write_state_ready(){
	if(srd[0] & 0x80){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_write_state_busy(){
	if((srd[0] & 0x80) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_erase_error(){
	if(srd[0] & 0x20){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_erase_ok(){
	if((srd[0] & 0x20) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_ok(){
	if((srd[0] & 0x08) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_error(){
	if(srd[0] & 0x08){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_excessive_ok(){
	if((srd[0] & 0x10) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_excessive_error(){
	if(srd[0] & 0x10){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_and_excessive_ok(){
	if((srd[0] & 0x18) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_and_excessive_error(){
	if(srd[0] & 0x18){
		return true;
	}
	return false;
}
bool cl_m16c_cmd::is_srd_rx_time_out_no_error(){
	if((srd[1] & 0x02) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_rx_time_out_error(){
	if(srd[1] & 0x02){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_checksum_match(){
	if(srd[1] & 0x10){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_checksum_mismatch(){
	if((srd[1] & 0x10) == 0){
		return true;
	}
	return false;
}
bool cl_m16c_cmd::is_srd_download_completed(){
	if(srd[1] & 0x80){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_not_completed(){
	if((srd[1] & 0x80) == 0){
		return true;
	}
	return false;
}
