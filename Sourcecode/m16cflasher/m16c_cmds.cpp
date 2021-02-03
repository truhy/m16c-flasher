#include "m16c_cmds.h"

cl_m16c_cmd::cl_m16c_cmd(){
}

std::string cl_m16c_cmd::last_err_msg(){
	return m_last_err_msg;
}

size_t cl_m16c_cmd::last_err_msg_len(){
	return m_last_err_msg.length();
}

//Older boot loaders only support 9600
unsigned char cl_m16c_cmd::baud_rate_cmd_code(my_uint32 m_arg_baud_rate){
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

my_bool cl_m16c_cmd::init_baud_rate(cl_serial_com& m_arg_serial_com){
	unsigned char m_baud_rate_cmd_code = M16C_BL_CMD_CODE_9600;
	unsigned char m_rx_data;
	unsigned char m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue set baud rate command
	if(m_arg_serial_com.write(&m_baud_rate_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	#if SLEEP_BYTE_INTERVAL > 0
		Sleep(SLEEP_BYTE_INTERVAL);
	#endif

	for(m_i = 0; m_i < 16; ++m_i){
		//Send 0x00
		if(m_arg_serial_com.write(0x00, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	//If successful the M16C boot loader will echo the command code
	if(m_arg_serial_com.read(&m_rx_data, 1) < 1){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	//The echo is same as the one sent?
	if(m_rx_data != m_baud_rate_cmd_code){
		m_last_err_msg = "Error: initital communications failed!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::set_baud_rate(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_baud_rate){
	unsigned char m_baud_rate_cmd_code = baud_rate_cmd_code(m_arg_baud_rate);
	unsigned char m_rx_data;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue set baud rate command
	if(m_arg_serial_com.write(&m_baud_rate_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	#if SLEEP_BYTE_INTERVAL > 0
		Sleep(SLEEP_BYTE_INTERVAL);
	#endif

	//If successful the M16C boot loader will echo the command code
	if(m_arg_serial_com.read(&m_rx_data, 1) < 1){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	//The echo is same as the one sent?
	if(m_rx_data != m_baud_rate_cmd_code){
		m_last_err_msg = "Error: set baud rate incorrect echo!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::rd_version(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_VERSION;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue read boot loader version command
	if(m_arg_serial_com.write(&m_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	#if SLEEP_BYTE_INTERVAL > 0
		Sleep(SLEEP_BYTE_INTERVAL);
	#endif

	memset(m_ver, 0, 8);
	//Read boot loader version data
	if(m_arg_serial_com.read(m_ver, 8) < 8){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::rd_status(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_READ_STATUS_REG;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue read status register command
	if(m_arg_serial_com.write(&m_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	#if SLEEP_BYTE_INTERVAL > 0
		Sleep(SLEEP_BYTE_INTERVAL);
	#endif

	memset(m_srd, 0, 2);
	//Read status register data
	if(m_arg_serial_com.read(m_srd, 2) < 2){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::id_chk(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_id_addr, std::string m_arg_id_hex_str){
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
	my_uint16 m_i;
	my_uint16 m_num_of_hex_bytes = m_arg_id_hex_str.length() / 2;

	if(m_num_of_hex_bytes > 7){
		m_num_of_hex_bytes = 7;
	}

	for(m_i = 0; m_i < m_num_of_hex_bytes; ++m_i){
		m_cmd[5 + m_i] = (unsigned char)strtoul(m_arg_id_hex_str.substr(2 * m_i, 2).c_str(), NULL, 16);
	}

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue ID check command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::page_rd(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_PAGE_READ,
		(unsigned char)(m_arg_from_addr >> 8),
		(unsigned char)(m_arg_from_addr >> 16)
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue page read command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	memset(m_arg_buf_256, 0, 256);
	//Read 256 bytes
	if(m_arg_serial_com.read(m_arg_buf_256, 256) < 256){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::clear_status(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_CLEAR_STATUS_REG;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	if(m_arg_serial_com.write(&m_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	return TRUE;
}

/*
	Program (i.e. flash) a page (256 bytes).
	The from address must be an address at the beginning of a page, i.e. multiple of 256 from the start of a block.
*/
my_bool cl_m16c_cmd::page_program(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256){
	cl_my_buf m_cmd;
	my_uint16 m_i;

	m_last_err_msg.clear();

	if(!m_cmd.alloc_buf(259)){
		m_last_err_msg = "Error: could not allocate buffer!";
		return FALSE;
	}

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_PAGE_PROGRAM;
	m_cmd.buf()[1] = (unsigned char)(m_arg_from_addr >> 8);
	m_cmd.buf()[2] = (unsigned char)(m_arg_from_addr >> 16);
	memcpy(m_cmd.buf() + 3, m_arg_buf_256, 256);

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command with 256 bytes to write
	for(m_i = 0; m_i < 259; ++m_i){
		if(m_arg_serial_com.write(m_cmd.buf() + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::erase_block(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_BLOCK_ERASE,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16),
		0xD0
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::erase_all_unlocked(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_ERASE_ALL_UNLOCKED,
		0xD0
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::read_lock_bit(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_READ_LOCK_BIT_STATUS,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16)
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	m_lock_bit = 0;
	//Read
	if(m_arg_serial_com.read(&m_lock_bit, 1) < 1){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	return TRUE;
}

/*
	This sets the specified block with a lock bit flag 0 (locked).
	To reset the lock bit flag to 1 (unlocked) send the lock bit disable command and then an erase command.
*/
my_bool cl_m16c_cmd::lock_bit_program(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_LOCK_BIT_PROGRAM,
		(unsigned char)(m_arg_block_highest_addr >> 8),
		(unsigned char)(m_arg_block_highest_addr >> 16),
		0xD0
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::lock_bit_enable(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_ENABLE;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	if(m_arg_serial_com.write(&m_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::lock_bit_disable(cl_serial_com& m_arg_serial_com){
	unsigned char m_cmd_code = M16C_BL_CMD_CODE_LOCK_BIT_DISABLE;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue command
	if(m_arg_serial_com.write(&m_cmd_code, 1) < 1){
		m_last_err_msg = "Error: could not send all bytes to serial com!";
		return FALSE;
	}

	return TRUE;
}

unsigned char cl_m16c_cmd::calc_checksum(my_uint16 m_arg_buf_len, void* m_arg_buf){
	unsigned char m_checksum = 0;
	my_uint32 m_i;

	for(m_i = 0; m_i < m_arg_buf_len; ++m_i){
		m_checksum += ((unsigned char*)m_arg_buf)[m_i];
	}

	return m_checksum;
}

my_bool cl_m16c_cmd::download_calc_checksum(cl_serial_com& m_arg_serial_com, my_uint16 m_arg_buf_len, void* m_arg_buf){
	cl_my_buf m_cmd;
	my_uint32 m_cmd_buf_max_len = 256 + 4;
	my_uint16 m_total_buf_sent = 0;
	my_uint16 m_buf_len_to_send;
	my_uint16 m_header_len = 4;
	my_uint16 m_i;

	m_last_err_msg.clear();

	if(!m_cmd.alloc_buf(m_cmd_buf_max_len)){
		m_last_err_msg = "Error: could not allocate buffer!";
		return FALSE;
	}

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	m_cmd.buf()[1] = (unsigned char)m_arg_buf_len;
	m_cmd.buf()[2] = (unsigned char)(m_arg_buf_len >> 8);
	m_cmd.buf()[3] = calc_checksum(m_arg_buf_len, m_arg_buf);

	Sleep(SLEEP_CMD_INTERVAL);

	while(m_total_buf_sent < m_arg_buf_len){
		if((m_arg_buf_len - m_total_buf_sent) > 256){
			m_buf_len_to_send = 256;
		}else{
			m_buf_len_to_send = m_arg_buf_len;
		}

		memcpy(m_cmd.buf() + m_header_len, (unsigned char*)m_arg_buf + m_total_buf_sent, m_buf_len_to_send);

		/*
		//Issue command
		if(m_arg_serial_com.write(m_cmd.buf(), m_header_len + m_buf_len_to_send) < (DWORD)(m_header_len + m_buf_len_to_send)){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		*/

		//Send one byte at a time
		for(m_i = 0; m_i < (m_header_len + m_buf_len_to_send); ++m_i){
			if(m_arg_serial_com.write(m_cmd.buf() + m_i, 1) < 1){
				m_last_err_msg = "Error: could not send all bytes to serial com!";
				return FALSE;
			}
			#if SLEEP_BYTE_INTERVAL > 0
				Sleep(SLEEP_BYTE_INTERVAL);
			#endif
		}

		if(m_total_buf_sent == 0){
			m_header_len = 0;
		}
		m_total_buf_sent += m_buf_len_to_send;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::download(cl_serial_com& m_arg_serial_com, unsigned char m_arg_checksum, my_uint16 m_arg_buf_len, void* m_arg_buf){
	cl_my_buf m_cmd;
	my_uint32 m_cmd_buf_len = m_arg_buf_len + 4;
	my_uint16 m_i;

	m_last_err_msg.clear();

	if(!m_cmd.alloc_buf(m_cmd_buf_len)){
		m_last_err_msg = "Error: could not allocate buffer!";
		return FALSE;
	}

	m_cmd.buf()[0] = M16C_BL_CMD_CODE_DOWNLOAD;
	m_cmd.buf()[1] = (unsigned char)m_arg_buf_len;
	m_cmd.buf()[2] = (unsigned char)(m_arg_buf_len >> 8);
	m_cmd.buf()[3] = m_arg_checksum;
	memcpy(m_cmd.buf() + 4, m_arg_buf, m_arg_buf_len);

	Sleep(SLEEP_CMD_INTERVAL);

	//Send one byte at a time
	for(m_i = 0; m_i < m_cmd_buf_len; ++m_i){
		if(m_arg_serial_com.write(m_cmd.buf() + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	return TRUE;
}

my_bool cl_m16c_cmd::boot_area_output(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256){
	unsigned char m_cmd[] = {
		M16C_BL_CMD_CODE_BOOT_AREA_OUTPUT,
		(unsigned char)(m_arg_from_addr >> 8),
		(unsigned char)(m_arg_from_addr >> 16)
	};
	my_uint16 m_i;

	m_last_err_msg.clear();

	Sleep(SLEEP_CMD_INTERVAL);

	//Issue page read command
	for(m_i = 0; m_i < sizeof(m_cmd); ++m_i){
		if(m_arg_serial_com.write(m_cmd + m_i, 1) < 1){
			m_last_err_msg = "Error: could not send all bytes to serial com!";
			return FALSE;
		}
		#if SLEEP_BYTE_INTERVAL > 0
			Sleep(SLEEP_BYTE_INTERVAL);
		#endif
	}

	memset(m_arg_buf_256, 0, 256);
	//Read 256 bytes
	if(m_arg_serial_com.read(m_arg_buf_256, 256) < 256){
		m_last_err_msg = "Error: could not receive all bytes from to serial com!";
		return FALSE;
	}

	return TRUE;
}

my_bool cl_m16c_cmd::is_srd_id_verified(){
	if((m_srd[1] & 0x0C) == 0x0C){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_write_state_ready(){
	if(m_srd[0] & 0x80){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_write_state_busy(){
	if((m_srd[0] & 0x80) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_erase_error(){
	if(m_srd[0] & 0x20){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_erase_ok(){
	if((m_srd[0] & 0x20) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_program_ok(){
	if((m_srd[0] & 0x08) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_program_error(){
	if(m_srd[0] & 0x08){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_excessive_ok(){
	if((m_srd[0] & 0x10) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_excessive_error(){
	if(m_srd[0] & 0x10){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_program_and_excessive_ok(){
	if((m_srd[0] & 0x18) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_program_and_excessive_error(){
	if(m_srd[0] & 0x18){
		return TRUE;
	}
	return FALSE;
}
my_bool cl_m16c_cmd::is_srd_rx_time_out_no_error(){
	if((m_srd[1] & 0x02) == 0){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_rx_time_out_error(){
	if(m_srd[1] & 0x02){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_download_checksum_match(){
	if(m_srd[1] & 0x10){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_download_checksum_mismatch(){
	if((m_srd[1] & 0x10) == 0){
		return TRUE;
	}
	return FALSE;
}
my_bool cl_m16c_cmd::is_srd_download_completed(){
	if(m_srd[1] & 0x80){
		return TRUE;
	}
	return FALSE;
}

my_bool cl_m16c_cmd::is_srd_download_not_completed(){
	if((m_srd[1] & 0x80) == 0){
		return TRUE;
	}
	return FALSE;
}
