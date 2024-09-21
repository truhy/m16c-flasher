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
unsigned char cl_m16c_cmd::baud_rate_cmd_code(uint32_t m_arg_baud_rate){
	switch(m_arg_baud_rate){
		case 9600:{ return 0xB0; break; }
		case 19200:{ return 0xB1; break; }
		case 38400:{ return 0xB2; break; }
		case 57600:{ return 0xB3; break; }
		case 115200:{ return 0xB4; break; }
		default:{ return 0xB0; }
	}
	return 0;
}

void cl_m16c_cmd::set_baud_rate(serial_com& m_arg_serial_com, uint32_t m_arg_baud_rate){
	unsigned char m_baud_rate_cmd_code = baud_rate_cmd_code(m_arg_baud_rate);
	unsigned char m_rx_data;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send set baud rate command
	if(m_arg_serial_com.write_port(&m_baud_rate_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// If successful the M16C boot loader will echo the command code
	if(m_arg_serial_com.read_port(&m_rx_data, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}

	// The echo is same as the one sent?
	if(m_rx_data != m_baud_rate_cmd_code){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_ECHO_ID, app_error_string::messages[APP_ERROR_ECHO_ID], std::format(app_error_string::messages[APP_ERROR_ECHO_INFO_ID], m_baud_rate_cmd_code, m_rx_data));
	}
}

void cl_m16c_cmd::rd_version(serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_VERSION;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send read boot loader version command
	if(m_arg_serial_com.write_port(&m_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(m_ver, 0, 8);
	// Read boot loader version data
	if(m_arg_serial_com.read_port(m_ver, 8) < 8){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::rd_status(serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_READ_STATUS_REG;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send read status register command
	if(m_arg_serial_com.write_port(&m_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	memset(m_srd, 0, 2);
	// Read status register data
	if(m_arg_serial_com.read_port(m_srd, 2) < 2){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::id_chk(serial_com& m_arg_serial_com, uint32_t m_arg_id_addr, std::string m_arg_id_hex_str){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_ID_CHECK,
		(unsigned char)m_arg_id_addr,
		(unsigned char)(m_arg_id_addr >> 8),
		(unsigned char)(m_arg_id_addr >> 16),
		7,
		0,
		0,
		0,
		0,
		0,
		0,
		0
	};
	uint16_t m_num_of_hex_bytes = (uint16_t)(m_arg_id_hex_str.length() / 2);

	if(m_num_of_hex_bytes > 7){
		m_num_of_hex_bytes = 7;
	}

	for(uint16_t m_i = 0; m_i < m_num_of_hex_bytes; ++m_i){
		m_cmd[5 + m_i] = (unsigned char)strtoul(m_arg_id_hex_str.substr(2 * m_i, 2).c_str(), NULL, 16);
	}

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send ID check command + addr + ID one byte at a time
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::page_rd(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256){
	unsigned char m_cmd[] = {
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
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
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

void cl_m16c_cmd::clear_status(serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_CLEAR_STATUS_REG;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(m_arg_serial_com.write_port(&m_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

/*
	Program (i.e. flash) a page (256 bytes).
	The from address must be an address at the beginning of a page, i.e. multiple of 256 from the start of a block.
*/
void cl_m16c_cmd::page_program(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256){
	cl_my_buf m_cmd;

	m_cmd.alloc_buf(259);

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_PAGE_PROGRAM;
	m_cmd.buf()[1] = (unsigned char)(m_arg_from_addr >> 8);
	m_cmd.buf()[2] = (unsigned char)(m_arg_from_addr >> 16);
	memcpy(m_cmd.buf() + 3, m_arg_buf_256, 256);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + data one byte at a time
	for(uint16_t m_i = 0; m_i < 259; ++m_i){
		if(m_arg_serial_com.write_port(m_cmd.buf() + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::erase_block(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_BLOCK_ERASE,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16),
		0xD0
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
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
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::read_lock_bit(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_READ_LOCK_BIT_STATUS,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16)
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr  one byte at a time
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}

	m_lock_bit = 0;
	// Read
	if(m_arg_serial_com.read_port(&m_lock_bit, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RX_FAIL_ID, app_error_string::messages[APP_ERROR_RX_FAIL_ID], "");
	}
}

/*
	This sets the specified block with a lock bit flag 0 (locked).
	To reset the lock bit flag to 1 (unlocked) send the lock bit disable command and then an erase command.
*/
void cl_m16c_cmd::lock_bit_program(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_LOCK_BIT_PROGRAM,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16),
		0xD0
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::lock_bit_enable(serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_ENABLE;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(m_arg_serial_com.write_port(&m_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

void cl_m16c_cmd::lock_bit_disable(serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_DISABLE;

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command
	if(m_arg_serial_com.write_port(&m_cmd_code, 1) < 1){
		throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
	}
}

unsigned char cl_m16c_cmd::calc_checksum(uint16_t m_arg_buf_len, void *m_arg_buf){
	unsigned char m_checksum = 0;
	uint32_t m_i;

	for(m_i = 0; m_i < m_arg_buf_len; ++m_i){
		m_checksum += ((unsigned char*)m_arg_buf)[m_i];
	}

	return m_checksum;
}

void cl_m16c_cmd::download_calc_checksum(serial_com& m_arg_serial_com, uint16_t m_arg_buf_len, void *m_arg_buf){
	cl_my_buf m_cmd;
	uint32_t m_cmd_buf_max_len = 256 + 4;
	uint16_t m_total_buf_sent = 0;
	uint16_t m_buf_len_to_send;
	uint16_t m_header_len = 4;
	uint32_t m_tx_len;

	m_cmd.alloc_buf(m_cmd_buf_max_len);

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	m_cmd.buf()[1] = (unsigned char)m_arg_buf_len;
	m_cmd.buf()[2] = (unsigned char)(m_arg_buf_len >> 8);
	m_cmd.buf()[3] = calc_checksum(m_arg_buf_len, m_arg_buf);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	while(m_total_buf_sent < m_arg_buf_len){
		if((m_arg_buf_len - m_total_buf_sent) > 256){
			m_buf_len_to_send = 256;
		}else{
			m_buf_len_to_send = m_arg_buf_len;
		}

		memcpy(m_cmd.buf() + m_header_len, (unsigned char*)m_arg_buf + m_total_buf_sent, m_buf_len_to_send);

		// Send data one byte at a time
		m_tx_len = m_header_len + m_buf_len_to_send;
		for(uint16_t m_i = 0; m_i < m_tx_len; ++m_i){
			if(m_arg_serial_com.write_port(m_cmd.buf() + m_i, 1) < 1){
				throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
			}
		}

		if(m_total_buf_sent == 0){
			m_header_len = 0;
		}
		m_total_buf_sent += m_buf_len_to_send;
	}
}

void cl_m16c_cmd::download(serial_com& m_arg_serial_com, unsigned char m_arg_checksum, uint16_t m_arg_buf_len, void* m_arg_buf){
	cl_my_buf m_cmd;
	uint32_t m_cmd_buf_len = m_arg_buf_len + 4;

	m_cmd.alloc_buf(m_cmd_buf_len);

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	m_cmd.buf()[1] = (unsigned char)m_arg_buf_len;
	m_cmd.buf()[2] = (unsigned char)(m_arg_buf_len >> 8);
	m_cmd.buf()[3] = m_arg_checksum;
	memcpy(m_cmd.buf() + 4, m_arg_buf, m_arg_buf_len);

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + data one byte at a time
	for(uint16_t m_i = 0; m_i < m_cmd_buf_len; ++m_i){
		if(m_arg_serial_com.write_port(m_cmd.buf() + m_i, 1) < 1){
			throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_TX_FAIL_ID, app_error_string::messages[APP_ERROR_TX_FAIL_ID], "");
		}
	}
}

void cl_m16c_cmd::boot_area_output(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_BOOT_AREA_OUTPUT,
		(unsigned char)(m_arg_from_addr >> 8),
		(unsigned char)(m_arg_from_addr >> 16)
	};

#if defined(WIN32) || defined(WIN64)
	Sleep(SLEEP_CMD_INTERVAL);  // We need to wait a bit for the downloaded program to become ready
#else
	usleep(SLEEP_CMD_INTERVAL * 1000);  // We need to wait a bit for the downloaded program to become ready
#endif

	// Send command + addr one byte at a time
	for(uint16_t m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write_port(m_cmd + m_i, 1) < 1){
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

bool cl_m16c_cmd::is_srd_id_verified(){
	if((m_srd[1] & 0x0C) == 0x0C){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_write_state_ready(){
	if(m_srd[0] & 0x80){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_write_state_busy(){
	if((m_srd[0] & 0x80) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_erase_error(){
	if(m_srd[0] & 0x20){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_erase_ok(){
	if((m_srd[0] & 0x20) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_ok(){
	if((m_srd[0] & 0x08) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_error(){
	if(m_srd[0] & 0x08){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_excessive_ok(){
	if((m_srd[0] & 0x10) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_excessive_error(){
	if(m_srd[0] & 0x10){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_and_excessive_ok(){
	if((m_srd[0] & 0x18) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_program_and_excessive_error(){
	if(m_srd[0] & 0x18){
		return true;
	}
	return false;
}
bool cl_m16c_cmd::is_srd_rx_time_out_no_error(){
	if((m_srd[1] & 0x02) == 0){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_rx_time_out_error(){
	if(m_srd[1] & 0x02){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_checksum_match(){
	if(m_srd[1] & 0x10){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_checksum_mismatch(){
	if((m_srd[1] & 0x10) == 0){
		return true;
	}
	return false;
}
bool cl_m16c_cmd::is_srd_download_completed(){
	if(m_srd[1] & 0x80){
		return true;
	}
	return false;
}

bool cl_m16c_cmd::is_srd_download_not_completed(){
	if((m_srd[1] & 0x80) == 0){
		return true;
	}
	return false;
}
