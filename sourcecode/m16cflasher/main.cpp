/*
	M16C flasher for reading/erasing/flashing Renesas M16C MCU using the Serial I/O asynchronous mode with the built-in boot loader.

	Developer: Truong Hy
	Date: 17 Feb 2020
	Language: Visual Studio C++
	Program type: Windows console program
*/

#include "tru_exception.h"
#include "cmd_line.h"
#include "to_string.h"
#include "serial_com.h"
#include "m16c_cmds.h"
#include "my_file.h"
#include "my_buf.h"
#include "m16c_mem_map.h"
#include "app_error_string.h"
#include <stdio.h>
#include <iostream>

void process_read_status(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	m_arg_m16c_cmd.rd_status(m_arg_serial_com);

	std::cout << "SRD = 0x"  << string_utils_ns::to_string_right_hex((unsigned short)m_arg_m16c_cmd.m_srd[1], 2, '0') << string_utils_ns::to_string_right_hex((unsigned short)m_arg_m16c_cmd.m_srd[0], 2, '0') << std::endl;

	if(m_arg_m16c_cmd.m_srd[0] & 0x08){
		std::cout << " Bit3 = 1: Last flash write terminated in error" << std::endl;
	}else{
		std::cout << " Bit3 = 0: Last flash write terminated ok" << std::endl;
	}

	if(m_arg_m16c_cmd.m_srd[0] & 0x10){
		std::cout << " Bit4 = 1: Last flash write excessive terminated in error" << std::endl;
	}else{
		std::cout << " Bit4 = 0: Last flash write excessive terminated ok" << std::endl;
	}

	if(m_arg_m16c_cmd.m_srd[0] & 0x20){
		std::cout << " Bit5 = 1: Last flash erase terminated in error" << std::endl;
	}else{
		std::cout << " Bit5 = 0: Last flash erase terminated ok" << std::endl;
	}

	if(m_arg_m16c_cmd.m_srd[0] & 0x80){
		std::cout << " Bit7 = 1: Write state machine ready" << std::endl;
	}else{
		std::cout << " Bit7 = 0: Write state machine busy" << std::endl;
	}

	if(m_arg_m16c_cmd.m_srd[1] & 0x02){
		std::cout << " Bit9 = 1: Data receive timed out error" << std::endl;
	}else{
		std::cout << " Bit9 = 0: Data receive ok" << std::endl;
	}

	switch(m_arg_m16c_cmd.m_srd[1] & 0x0C){
		case 0x00: std::cout << " Bit10 & 11 = 00: ID not verified" << std::endl; break;
		case 0x04: std::cout << " Bit10 & 11 = 01: ID verification mismatch" << std::endl; break;
		case 0x08: break;  // Reserved
		case 0x0C: std::cout << " Bit10 & 11 = 11: ID verified" << std::endl; break;
	}

	if(m_arg_m16c_cmd.m_srd[1] & 0x10){
		std::cout << " Bit12 = 1: Download program checksum match" << std::endl;
	}else{
		std::cout << " Bit12 = 0: Download program checksum mismatch" << std::endl;
	}

	if(m_arg_m16c_cmd.m_srd[1] & 0x80){
		std::cout << " Bit15 = 1: Download program to RAM completed" << std::endl;
	}else{
		std::cout << " Bit15 = 0: Download program to RAM not completed" << std::endl;
	}
}

void process_id_chk(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	m_arg_m16c_cmd.id_chk(m_arg_serial_com, m_arg_my_params.m_id_addr, m_arg_my_params.m_id_hex_str);
	m_arg_m16c_cmd.rd_status(m_arg_serial_com);

	if((m_arg_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
		std::cout << "ID is verified" << std::endl;
	}else{
		std::cout << "ID mismatch" << std::endl;
	}
}

void process_read_flash_to_file(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	cl_my_buf m_page;
	cl_my_file m_my_file;
	uint32_t m_i;
	size_t m_bytes_written;

	m_page.alloc_buf(256);

	m_my_file.open_file(m_arg_my_params.m_full_file_name, "wb");

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_to_addr, 6, '0') << std::endl;

	for(m_i = m_arg_my_params.m_from_addr; m_i < m_arg_my_params.m_to_addr; m_i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(m_i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_i + 255, 6, '0') << std::endl;

		// Read flash memory
		m_arg_m16c_cmd.page_rd(m_arg_serial_com, m_i, m_page.buf());

		// Write to file
		m_my_file.write_file(m_page.buf(), 256, m_bytes_written);
	}
}

/*
	After erase all values will be 0xFF so a chip is blank if all values are 0xFF.
*/
void process_blank_check(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd, bool& m_arg_is_blank){
	uint32_t m_i;
	uint32_t m_j;
	cl_my_buf m_page;

	m_arg_is_blank = false;

	m_page.alloc_buf(256);

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_to_addr, 6, '0') << std::endl;

	for(m_i = m_arg_my_params.m_from_addr; m_i < m_arg_my_params.m_to_addr; m_i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(m_i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_i + 255, 6, '0') << std::endl;

		// Read flash memory
		m_arg_m16c_cmd.page_rd(m_arg_serial_com, m_i, m_page.buf());

		// Check for non-blanks (not 0xFF)
		for(m_j = 0; m_j < 256; ++m_j){
			if(m_page.buf()[m_j] != 0xFF){
				std::cout << "Found non-blank value at address: 0x" << string_utils_ns::to_string_right_hex(m_i | m_j, 6, '0') << ", value: 0x" << string_utils_ns::to_string_right_hex(m_page.buf()[m_j], 2, '0') << std::endl;
				return;
			}
		}
	}
	m_arg_is_blank = true;
	std::cout << "Memory range is blank" << std::endl;
}

void process_clear_status(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	std::cout << "Clearing any previous errors in status register" << std::endl;
	// Clear any previous status register error flags
	m_arg_m16c_cmd.clear_status(m_arg_serial_com);
}

void process_disable_lock_bit(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	std::cout << "Disabling lock bit" << std::endl;
	// Lock bit disable before issuing erase
	m_arg_m16c_cmd.lock_bit_disable(m_arg_serial_com);
}

/*
	After block erase all values in the block will be 0xFF.
*/
void process_erase_blocks(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd, uint32_t m_arg_from_addr, uint32_t m_arg_to_addr){
	std::list<cl_m16c_flash_block> m_flash_block_list;
	std::list<cl_m16c_flash_block>::iterator m_iter;

	// Convert address range to flash block list
	cl_m16c_mem_map::addr_range_to_flash_block_list(m_arg_from_addr, m_arg_to_addr, m_flash_block_list);

	if(m_flash_block_list.size() > 0){
		// Iterate flash block list to erase all specified blocks
		for(m_iter = m_flash_block_list.begin(); m_iter != m_flash_block_list.end(); ++m_iter){
			std::cout <<
				"Erasing block " <<
				m_iter->m_block_name <<
				" (0x" <<
				string_utils_ns::to_string_right_hex(m_iter->m_block_begin, 6, '0') <<
				" to 0x" <<
				string_utils_ns::to_string_right_hex(m_iter->m_block_end, 6, '0') <<
				", " <<
				m_iter->m_block_size <<
				" bytes)" <<
				std::endl;

			// Erase a block
			m_arg_m16c_cmd.erase_block(m_arg_serial_com, m_iter->m_block_end);

			// Wait until write state machine is finished erasing
			do{
				// Read status
				m_arg_m16c_cmd.rd_status(m_arg_serial_com);
			}while(m_arg_m16c_cmd.is_srd_write_state_busy());

			// Erase error?
			if(m_arg_m16c_cmd.is_srd_erase_error()){
				std::cout << "Error: status register indicates erase failed!" << std::endl;
				return;
			}
		}
	}
}

void process_erase_all_unlocked(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	std::cout << "Erasing all unlocked blocks" << std::endl;
	// Erase a block
	m_arg_m16c_cmd.erase_all_unlocked(m_arg_serial_com);

	// Wait until write state machine is finished erasing
	do{
		// Read status
		m_arg_m16c_cmd.rd_status(m_arg_serial_com);
	}while(m_arg_m16c_cmd.is_srd_write_state_busy());

	// Erase error?
	if(m_arg_m16c_cmd.is_srd_erase_error()){
		std::cout << "Error: status register indicates erase failed!" << std::endl;
		return;
	}
}

void process_program_flash_from_file(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	cl_my_buf m_block;
	std::list<cl_m16c_flash_block> m_flash_block_list;
	std::list<cl_m16c_flash_block>::iterator m_iter;
	cl_my_file m_my_file;
	uint32_t m_i;
	size_t m_bytes_read;
	bool m_is_blank = false;
	unsigned int m_retry_count = 0;
	uint32_t m_num_blocks;
	uint32_t m_file_to_block_start_offset;
	uint32_t m_file_to_block_size;

	m_block.alloc_buf(FLASH_MAX_BLOCK_SIZE);

	// Convert address range to flash block list
	cl_m16c_mem_map::addr_range_to_flash_block_list(m_arg_my_params.m_from_addr, m_arg_my_params.m_to_addr, m_flash_block_list);

	m_my_file.open_file(m_arg_my_params.m_full_file_name, "rb");

	// Need to blank check?
	if(m_arg_my_params.m_is_blank_chk_before_program){
		process_blank_check(m_arg_my_params, m_arg_serial_com, m_arg_m16c_cmd, m_is_blank);
	}

	std::cout << "Total range to program: 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_to_addr, 6, '0') << std::endl;

	// Iterate flash block list
	for(m_iter = m_flash_block_list.begin(); m_iter != m_flash_block_list.end(); ++m_iter){
		// Set block buffer to default blank values 0xFF
		memset(m_block.buf(), 0xFF, FLASH_MAX_BLOCK_SIZE);

		// Calculate file to memory block offset and size
		if(m_iter == m_flash_block_list.begin()){
			// Calculate file offset into the M16C memory block
			m_file_to_block_start_offset = m_iter->m_block_begin - m_arg_my_params.m_from_addr;
			// Calculate file size belonging to the M16C memory block
			if(m_arg_my_params.m_to_addr < m_iter->m_block_end){
				m_file_to_block_size = m_arg_my_params.m_to_addr - m_arg_my_params.m_from_addr + 1;
			}else{
				m_file_to_block_size = m_iter->m_block_end - m_arg_my_params.m_from_addr + 1;
			}
		}else{
			m_file_to_block_start_offset = 0;
			// Calculate file size belonging to the M16C memory block
			if(m_arg_my_params.m_to_addr < m_iter->m_block_end){
				m_file_to_block_size = m_arg_my_params.m_to_addr - m_iter->m_block_begin + 1;
			}else{
				m_file_to_block_size = m_iter->m_block_size;
			}
		}

		// Read a memory block from file
		m_my_file.read_file(m_block.buf() + m_file_to_block_start_offset, m_file_to_block_size, m_bytes_read);

		// Need to erase?
		if(m_arg_my_params.m_is_erase_before_program && !m_is_blank){
			process_erase_blocks(m_arg_my_params, m_arg_serial_com, m_arg_m16c_cmd, m_iter->m_block_begin, m_iter->m_block_end);
		}

		// Iterate block pages (256 bytes at a time) and program (flash)
		std::cout <<
			"Block " <<
			m_iter->m_block_name << " (0x" << string_utils_ns::to_string_right_hex(m_iter->m_block_begin, 6, '0') <<
			" to 0x" << string_utils_ns::to_string_right_hex(m_iter->m_block_end, 6, '0') <<
			", " <<
			m_iter->m_block_size <<
			" bytes)" <<
			std::endl;

		m_num_blocks = m_iter->m_block_size / 256;

		// Iterate a page
		for(m_i = 0; m_i < m_num_blocks; ++m_i){
			do{
				std::cout << "Programming: 0x" << string_utils_ns::to_string_right_hex(m_iter->m_block_begin + m_i * 256, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_iter->m_block_begin + m_i * 256 + 255, 6, '0') << std::endl;

				// Flash memory
				m_arg_m16c_cmd.page_program(m_arg_serial_com, m_iter->m_block_begin + m_i * 256, m_block.buf() + m_i * 256);

				// Wait until write state machine is finished erasing
				do{
					// Read status
					m_arg_m16c_cmd.rd_status(m_arg_serial_com);
				}while(m_arg_m16c_cmd.is_srd_write_state_busy());

				// Program error?
				if(m_arg_m16c_cmd.is_srd_program_error()){
					std::cout << "Error: status register indicates program failed!" << std::endl;
					return;
				}

				// Program excessive error?
				if(m_arg_m16c_cmd.is_srd_excessive_error()){
					std::cout << "Error: status register indicates write excessive (endurance reached)!" << std::endl;

					if(m_retry_count < m_arg_my_params.m_num_erase_program_error_retry){
						++m_retry_count;
						std::cout << "Retrying (" << m_retry_count << " of " << m_arg_my_params.m_num_erase_program_error_retry << ") block " << m_i << std::endl;

						process_clear_status(m_arg_my_params, m_arg_serial_com, m_arg_m16c_cmd);
					}else{
						std::cout << "Max retry of " << m_arg_my_params.m_num_erase_program_error_retry << " reached" << std::endl;
						throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RETRY, app_error_string::messages[APP_ERROR_RETRY], "");
					}
				}
				else
				{
					m_retry_count = 0;
				}
			}while(m_retry_count > 0);
		}
	}
}

void process_download_from_file(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	cl_my_buf m_code;
	cl_my_file m_my_file;
	size_t m_bytes_read;
	long m_file_len;

	m_my_file.open_file(m_arg_my_params.m_full_file_name, "rb");

	m_file_len = m_my_file.length();

	m_code.alloc_buf(m_file_len);

	// Read from file
	m_my_file.read_file(m_code.buf(), m_file_len, m_bytes_read);

	std::cout << "Program downloading (" << m_file_len << " bytes)" << std::endl;
	m_arg_m16c_cmd.download_calc_checksum(m_arg_serial_com, (uint16_t)m_file_len, m_code.buf());  // Download to RAM and run
	std::cout << "Program downloaded" << std::endl;
}

void process_boot_area_output_to_file(cl_my_params& m_arg_my_params, serial_com& m_arg_serial_com, cl_m16c_cmd& m_arg_m16c_cmd){
	cl_my_buf m_page;
	cl_my_file m_my_file;
	uint32_t m_i;
	size_t m_bytes_written;

	m_page.alloc_buf(256);

	m_my_file.open_file(m_arg_my_params.m_full_file_name, "wb");

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_arg_my_params.m_to_addr, 6, '0') << std::endl;

	for(m_i = m_arg_my_params.m_from_addr; m_i < m_arg_my_params.m_to_addr; m_i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(m_i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(m_i + 255, 6, '0') << std::endl;

		// Read boot area
		m_arg_m16c_cmd.boot_area_output(m_arg_serial_com, m_i, m_page.buf());

		// Write to file
		m_my_file.write_file(m_page.buf(), 256, m_bytes_written);
	}
}

void process_cmd_line(cl_my_params& m_arg_my_params){
	serial_com m_serial_com;
	cl_m16c_cmd m_m16c_cmd;
	std::string m_ver_str;
	bool m_is_blank;

	m_serial_com.open_handle(m_arg_my_params.m_dev_path);  // Open serial COM port

	if (m_arg_my_params.m_cmd != CMD_AUTO_BAUD) {
		m_serial_com.set_timeout(5000);
		// Set serial com device baudrate and parameters
		m_serial_com.set_params(m_arg_my_params.m_baud_rate, 8, NOPARITY, ONESTOPBIT, false);

		// Set MCU baudrate
		m_m16c_cmd.set_baud_rate(m_serial_com, m_arg_my_params.m_baud_rate);
	}

	switch(m_arg_my_params.m_cmd){
		case CMD_AUTO_BAUD:
			m_serial_com.set_timeout(500);
			m_serial_com.set_params(9600, 8, NOPARITY, ONESTOPBIT, false);
			m_m16c_cmd.auto_baud(m_serial_com);
			m_m16c_cmd.set_baud_rate(m_serial_com, m_arg_my_params.m_baud_rate);
			std::cout << "OK: " << m_arg_my_params.m_baud_rate << std::endl;
			break;
		case CMD_VER:
			m_m16c_cmd.rd_version(m_serial_com);
			m_ver_str.assign((char*)m_m16c_cmd.m_ver, 8);
			std::cout << m_ver_str << std::endl;
			break;
		case CMD_STATUS:
			process_read_status(m_arg_my_params, m_serial_com, m_m16c_cmd);
			break;
		case CMD_ID_CHK:
			process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			break;
		case CMD_READ:
			m_arg_my_params.m_from_addr = m_arg_my_params.m_from_addr & 0xFFFFFF00;
			m_arg_my_params.m_to_addr = m_arg_my_params.m_to_addr | 0xFF;

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_read_flash_to_file(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			break;
		case CMD_BLANK_CHK:
			m_arg_my_params.m_from_addr = m_arg_my_params.m_from_addr & 0xFFFFFF00;
			m_arg_my_params.m_to_addr = m_arg_my_params.m_to_addr | 0xFF;

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_blank_check(m_arg_my_params, m_serial_com, m_m16c_cmd, m_is_blank);
			}

			break;
		case CMD_ERASE:
			m_arg_my_params.m_from_addr = m_arg_my_params.m_from_addr & 0xFFFFFF00;
			m_arg_my_params.m_to_addr = m_arg_my_params.m_to_addr | 0xFF;

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_clear_status(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_disable_lock_bit(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_erase_blocks(m_arg_my_params, m_serial_com, m_m16c_cmd, m_arg_my_params.m_from_addr, m_arg_my_params.m_to_addr);
			}

			break;
		case CMD_ERASE_UNLOCKED:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_clear_status(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_disable_lock_bit(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_erase_all_unlocked(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			break;
		case CMD_READ_LOCK_BIT:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				m_m16c_cmd.read_lock_bit(m_serial_com, m_arg_my_params.m_from_addr);
				if(m_m16c_cmd.m_lock_bit == 0){
					std::cout << "Lock bit = 0, block is flagged as locked" << std::endl;
				}else{
					std::cout << "Lock bit = 1, block is flagged as not locked" << std::endl;
				}
			}

			break;
		case CMD_PROGRAM_LOCK_BIT:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_clear_status(m_arg_my_params, m_serial_com, m_m16c_cmd);
				m_m16c_cmd.lock_bit_program(m_serial_com, m_arg_my_params.m_from_addr);
				std::cout << "Lock bit is programmed flagged locked" << std::endl;
			}

			break;
		case CMD_LOCK_BIT_ENABLE:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				m_m16c_cmd.lock_bit_enable(m_serial_com);
				std::cout << "Lock bit enabled" << std::endl;
			}

			break;
		case CMD_LOCK_BIT_DISABLE:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				m_m16c_cmd.lock_bit_disable(m_serial_com);
				std::cout << "Lock bit disabled" << std::endl;
			}

			break;
		case CMD_PROGRAM:
			m_arg_my_params.m_from_addr = m_arg_my_params.m_from_addr & 0xFFFFFF00;
			m_arg_my_params.m_to_addr = m_arg_my_params.m_to_addr | 0xFF;

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_clear_status(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_disable_lock_bit(m_arg_my_params, m_serial_com, m_m16c_cmd);
				process_program_flash_from_file(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			break;
		case CMD_DOWNLOAD:
			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_download_from_file(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			break;
		case CMD_BOOT_AREA_OUTPUT:
			m_arg_my_params.m_from_addr = m_arg_my_params.m_from_addr & 0xFFFFFF00;
			m_arg_my_params.m_to_addr = m_arg_my_params.m_to_addr | 0xFF;

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			m_m16c_cmd.rd_status(m_serial_com);
			if((m_m16c_cmd.m_srd[1] & 0x0C) == 0x0C){
				process_boot_area_output_to_file(m_arg_my_params, m_serial_com, m_m16c_cmd);
			}

			break;
	}
}

int main(int m_arg_c, char* m_arg_v[]){
	cl_my_params m_my_params;

	try{
		if(m_arg_c > 1){
			parse_params(m_arg_c, m_arg_v, m_my_params);
			process_cmd_line(m_my_params);
		}else{
			usage(m_arg_v[0]);
		}
	}catch(tru_exception &ex){
		std::cout << "\nError: " << ex.get_error() << std::endl;
		return ex.get_code();
	}

	return 0;
}
