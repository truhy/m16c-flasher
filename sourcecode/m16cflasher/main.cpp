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

void process_read_status(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	arg_m16c_cmd.rd_status(arg_serial_com);

	std::cout << "SRD = 0x"  << string_utils_ns::to_string_right_hex((unsigned short)arg_m16c_cmd.srd[1], 2, '0') << string_utils_ns::to_string_right_hex((unsigned short)arg_m16c_cmd.srd[0], 2, '0') << std::endl;

	if(arg_m16c_cmd.srd[0] & 0x08){
		std::cout << " Bit3 = 1: Last flash write terminated in error" << std::endl;
	}else{
		std::cout << " Bit3 = 0: Last flash write terminated ok" << std::endl;
	}

	if(arg_m16c_cmd.srd[0] & 0x10){
		std::cout << " Bit4 = 1: Last flash write excessive terminated in error" << std::endl;
	}else{
		std::cout << " Bit4 = 0: Last flash write excessive terminated ok" << std::endl;
	}

	if(arg_m16c_cmd.srd[0] & 0x20){
		std::cout << " Bit5 = 1: Last flash erase terminated in error" << std::endl;
	}else{
		std::cout << " Bit5 = 0: Last flash erase terminated ok" << std::endl;
	}

	if(arg_m16c_cmd.srd[0] & 0x80){
		std::cout << " Bit7 = 1: Write state machine ready" << std::endl;
	}else{
		std::cout << " Bit7 = 0: Write state machine busy" << std::endl;
	}

	if(arg_m16c_cmd.srd[1] & 0x02){
		std::cout << " Bit9 = 1: Data receive timed out error" << std::endl;
	}else{
		std::cout << " Bit9 = 0: Data receive ok" << std::endl;
	}

	switch(arg_m16c_cmd.srd[1] & 0x0C){
		case 0x00: std::cout << " Bit10 & 11 = 00: ID not verified" << std::endl; break;
		case 0x04: std::cout << " Bit10 & 11 = 01: ID verification mismatch" << std::endl; break;
		case 0x08: break;  // Reserved
		case 0x0C: std::cout << " Bit10 & 11 = 11: ID verified" << std::endl; break;
	}

	if(arg_m16c_cmd.srd[1] & 0x10){
		std::cout << " Bit12 = 1: Download program checksum match" << std::endl;
	}else{
		std::cout << " Bit12 = 0: Download program checksum mismatch" << std::endl;
	}

	if(arg_m16c_cmd.srd[1] & 0x80){
		std::cout << " Bit15 = 1: Download program to RAM completed" << std::endl;
	}else{
		std::cout << " Bit15 = 0: Download program to RAM not completed" << std::endl;
	}
}

void process_id_chk(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	arg_m16c_cmd.id_chk(arg_serial_com, arg_my_params.id_addr, arg_my_params.id_hex_str);
	arg_m16c_cmd.rd_status(arg_serial_com);

	if((arg_m16c_cmd.srd[1] & 0x0C) == 0x0C){
		std::cout << "ID is verified" << std::endl;
	}else{
		std::cout << "ID mismatch" << std::endl;
	}
}

void process_read_flash_to_file(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	cl_my_buf page;
	cl_my_file my_file;
	uint32_t i;
	size_t bytes_written;

	page.alloc_buf(256);

	my_file.open_file(arg_my_params.full_file_name, "wb");

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(arg_my_params.from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(arg_my_params.to_addr, 6, '0') << std::endl;

	for(i = arg_my_params.from_addr; i < arg_my_params.to_addr; i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(i + 255, 6, '0') << std::endl;

		// Read flash memory
		arg_m16c_cmd.page_rd(arg_serial_com, i, page.buf_ptr());

		// Write to file
		my_file.write_file(page.buf_ptr(), 256, bytes_written);
	}
}

/*
	After erase all values will be 0xFF so a chip is blank if all values are 0xFF.
*/
void process_blank_check(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd, bool &arg_is_blank){
	uint32_t i;
	uint32_t j;
	cl_my_buf page;

	arg_is_blank = false;

	page.alloc_buf(256);

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(arg_my_params.from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(arg_my_params.to_addr, 6, '0') << std::endl;

	for(i = arg_my_params.from_addr; i < arg_my_params.to_addr; i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(i + 255, 6, '0') << std::endl;

		// Read flash memory
		arg_m16c_cmd.page_rd(arg_serial_com, i, page.buf_ptr());

		// Check for non-blanks (not 0xFF)
		for(j = 0; j < 256; ++j){
			if(page.buf_ptr()[j] != 0xFF){
				std::cout << "Found non-blank value at address: 0x" << string_utils_ns::to_string_right_hex(i | j, 6, '0') << ", value: 0x" << string_utils_ns::to_string_right_hex(page.buf_ptr()[j], 2, '0') << std::endl;
				return;
			}
		}
	}
	arg_is_blank = true;
	std::cout << "Memory range is blank" << std::endl;
}

void process_clear_status(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	std::cout << "Clearing any previous errors in status register" << std::endl;
	// Clear any previous status register error flags
	arg_m16c_cmd.clear_status(arg_serial_com);
}

void process_disable_lock_bit(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	std::cout << "Disabling lock bit" << std::endl;
	// Lock bit disable before issuing erase
	arg_m16c_cmd.lock_bit_disable(arg_serial_com);
}

/*
	After block erase all values in the block will be 0xFF.
*/
void process_erase_blocks(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd, uint32_t arg_from_addr, uint32_t arg_to_addr){
	std::list<cl_m16c_flash_block> flash_block_list;
	std::list<cl_m16c_flash_block>::iterator iter;

	// Convert address range to flash block list
	cl_m16c_mem_map::addr_range_to_flash_block_list(arg_from_addr, arg_to_addr, flash_block_list);

	if(flash_block_list.size() > 0){
		// Iterate flash block list to erase all specified blocks
		for(iter = flash_block_list.begin(); iter != flash_block_list.end(); ++iter){
			std::cout <<
				"Erasing block " << iter->block_name <<
				" (0x" <<
				string_utils_ns::to_string_right_hex(iter->block_begin, 6, '0') <<
				" to 0x" <<
				string_utils_ns::to_string_right_hex(iter->block_end, 6, '0') <<
				", " <<
				(iter->block_end - iter->block_begin + 1) <<
				" bytes)" <<
				std::endl;

			// Erase a block
			arg_m16c_cmd.erase_block(arg_serial_com, iter->block_end);

			// Wait until write state machine is finished erasing
			do{
				// Read status
				arg_m16c_cmd.rd_status(arg_serial_com);
			}while(arg_m16c_cmd.is_srd_write_state_busy());

			// Erase error?
			if(arg_m16c_cmd.is_srd_erase_error()){
				std::cout << "Error: status register indicates erase failed!" << std::endl;
				return;
			}
		}
	}
}

void process_erase_all_unlocked(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	std::cout << "Erasing all unlocked blocks" << std::endl;
	// Erase a block
	arg_m16c_cmd.erase_all_unlocked(arg_serial_com);

	// Wait until write state machine is finished erasing
	do{
		// Read status
		arg_m16c_cmd.rd_status(arg_serial_com);
	}while(arg_m16c_cmd.is_srd_write_state_busy());

	// Erase error?
	if(arg_m16c_cmd.is_srd_erase_error()){
		std::cout << "Error: status register indicates erase failed!" << std::endl;
		return;
	}
}

void process_program_flash_from_file(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	cl_my_buf block;
	std::list<cl_m16c_flash_block> flash_block_list;
	std::list<cl_m16c_flash_block>::iterator iter;
	cl_my_file my_file;
	uint32_t i;
	size_t bytes_read;
	bool is_blank = false;
	uint32_t retry_count = 0;
	uint32_t num_blocks;
	uint32_t file_to_block_start_offset;
	uint32_t file_to_block_size;

	std::cout << "Allocating " << cl_m16c_mem_map::largest_block_size << " bytes buffer" << std::endl;
	block.alloc_buf(cl_m16c_mem_map::largest_block_size);

	// Convert address range to flash block list
	cl_m16c_mem_map::addr_range_to_flash_block_list(arg_my_params.from_addr, arg_my_params.to_addr, flash_block_list);

	my_file.open_file(arg_my_params.full_file_name, "rb");

	// Need to blank check?
	if(arg_my_params.is_blank_chk_before_program){
		process_blank_check(arg_my_params, arg_serial_com, arg_m16c_cmd, is_blank);
	}

	std::cout << "Total range to program: 0x" << string_utils_ns::to_string_right_hex(arg_my_params.from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(arg_my_params.to_addr, 6, '0') << std::endl;

	// Iterate flash block list
	for(iter = flash_block_list.begin(); iter != flash_block_list.end(); ++iter){
		// Set block buffer to default blank values 0xFF
		memset(block.buf_ptr(), 0xFF, cl_m16c_mem_map::largest_block_size);

		// Calculate file to memory block offset and size
		if(iter == flash_block_list.begin()){
			// Calculate file offset into the M16C memory block
			file_to_block_start_offset = iter->block_begin - arg_my_params.from_addr;
			// Calculate file size belonging to the M16C memory block
			if(arg_my_params.to_addr < iter->block_end){
				file_to_block_size = arg_my_params.to_addr - arg_my_params.from_addr + 1;
			}else{
				file_to_block_size = iter->block_end - arg_my_params.from_addr + 1;
			}
		}else{
			file_to_block_start_offset = 0;
			// Calculate file size belonging to the M16C memory block
			if(arg_my_params.to_addr < iter->block_end){
				file_to_block_size = arg_my_params.to_addr - iter->block_begin + 1;
			}else{
				file_to_block_size = iter->block_end - iter->block_begin + 1;
			}
		}

		std::cout << "Reading file " << file_to_block_size << " bytes into buffer" << std::endl;
		// Read a memory block from file
		my_file.read_file(block.buf_ptr() + file_to_block_start_offset, file_to_block_size, bytes_read);

		// Need to erase?
		if(arg_my_params.is_erase_before_program && !is_blank){
			process_erase_blocks(arg_my_params, arg_serial_com, arg_m16c_cmd, iter->block_begin, iter->block_end);
		}

		// Iterate block pages (256 bytes at a time) and program (flash)
		std::cout <<
			"Block " << iter->block_name <<
			" (0x" << string_utils_ns::to_string_right_hex(iter->block_begin, 6, '0') <<
			" to 0x" << string_utils_ns::to_string_right_hex(iter->block_end, 6, '0') <<
			", " <<
			file_to_block_size <<
			" bytes)" <<
			std::endl;

		num_blocks = file_to_block_size / 256;

		// Iterate a page
		for(i = 0; i < num_blocks; ++i){
			do{
				std::cout << "Programming: 0x" << string_utils_ns::to_string_right_hex(iter->block_begin + i * 256, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(iter->block_begin + i * 256 + 255, 6, '0') << std::endl;

				// Flash memory
				arg_m16c_cmd.page_program(arg_serial_com, iter->block_begin + i * 256, block.buf_ptr() + i * 256);

				// Wait until write state machine is finished erasing
				do{
					// Read status
					arg_m16c_cmd.rd_status(arg_serial_com);
				}while(arg_m16c_cmd.is_srd_write_state_busy());

				// Program error?
				if(arg_m16c_cmd.is_srd_program_error()){
					std::cout << "Error: status register indicates program failed!" << std::endl;
					return;
				}

				// Program excessive error?
				if(arg_m16c_cmd.is_srd_excessive_error()){
					std::cout << "Error: status register indicates write excessive (endurance reached)!" << std::endl;

					if(retry_count < arg_my_params.num_erase_program_error_retry){
						++retry_count;
						std::cout << "Retrying (" << retry_count << " of " << arg_my_params.num_erase_program_error_retry << ") block " << i << std::endl;

						process_clear_status(arg_my_params, arg_serial_com, arg_m16c_cmd);
					}else{
						std::cout << "Max retry of " << arg_my_params.num_erase_program_error_retry << " reached" << std::endl;
						throw tru_exception(__func__, TRU_EXCEPT_SRC_VEN, APP_ERROR_RETRY, app_error_string::messages[APP_ERROR_RETRY], "");
					}
				}
				else
				{
					retry_count = 0;
				}
			}while(retry_count > 0);
		}
	}
}

void process_download_from_file(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	cl_my_buf code;
	cl_my_file my_file;
	size_t bytes_read;
	long file_len;

	my_file.open_file(arg_my_params.full_file_name, "rb");

	file_len = my_file.length();

	code.alloc_buf(file_len);

	// Read from file
	my_file.read_file(code.buf_ptr(), file_len, bytes_read);

	std::cout << "Program downloading (" << file_len << " bytes)" << std::endl;
	arg_m16c_cmd.download_calc_checksum(arg_serial_com, (uint16_t)file_len, code.buf_ptr());  // Download to RAM and run
	std::cout << "Program downloaded" << std::endl;
}

void process_boot_area_output_to_file(cl_my_params &arg_my_params, serial_com &arg_serial_com, cl_m16c_cmd &arg_m16c_cmd){
	cl_my_buf page;
	cl_my_file my_file;
	uint32_t i;
	size_t bytes_written;

	page.alloc_buf(256);

	my_file.open_file(arg_my_params.full_file_name, "wb");

	std::cout << "Read range: 0x" << string_utils_ns::to_string_right_hex(arg_my_params.from_addr, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(arg_my_params.to_addr, 6, '0') << std::endl;

	for(i = arg_my_params.from_addr; i < arg_my_params.to_addr; i += 256){
		std::cout << "Reading: 0x" << string_utils_ns::to_string_right_hex(i, 6, '0') << " to 0x" << string_utils_ns::to_string_right_hex(i + 255, 6, '0') << std::endl;

		// Read boot area
		arg_m16c_cmd.boot_area_output(arg_serial_com, i, page.buf_ptr());

		// Write to file
		my_file.write_file(page.buf_ptr(), 256, bytes_written);
	}
}

void process_cmd_line(cl_my_params &arg_my_params){
	serial_com serial_com;
	cl_m16c_cmd m16c_cmd;
	std::string ver_str;
	bool is_blank;

	serial_com.open_handle(arg_my_params.dev_path);  // Open serial COM port

	if(arg_my_params.cmd != CMD_AUTO_BAUD) {
		serial_com.set_timeout(5000);
		serial_com.set_params(arg_my_params.baud_rate, 8, NOPARITY, ONESTOPBIT, false);  // Set serial com device baudrate and parameters
		m16c_cmd.set_baud_rate(serial_com, arg_my_params.baud_rate);  // Set MCU baudrate
	}

	switch(arg_my_params.cmd){
		case CMD_ERASE:
		case CMD_ERASE_UNLOCKED:
		case CMD_PROGRAM:
			if(arg_my_params.mcu_group == GROUP_NONE){
				std::cout << "MCU or MCU group must be specified when programming/erasing!" << std::endl;
				return;
			}
			break;
	}

	cl_m16c_mem_map::select_group(arg_my_params.mcu_group);

	switch(arg_my_params.cmd){
		case CMD_AUTO_BAUD:
			serial_com.set_timeout(500);
			serial_com.set_params(9600, 8, NOPARITY, ONESTOPBIT, false);
			m16c_cmd.auto_baud(serial_com);
			m16c_cmd.set_baud_rate(serial_com, arg_my_params.baud_rate);
			std::cout << "OK: " << arg_my_params.baud_rate << std::endl;
			break;
		case CMD_VER:
			m16c_cmd.rd_version(serial_com);
			ver_str.assign((char*)m16c_cmd.ver, 8);
			std::cout << ver_str << std::endl;
			break;
		case CMD_STATUS:
			process_read_status(arg_my_params, serial_com, m16c_cmd);
			break;
		case CMD_ID_CHK:
			process_id_chk(arg_my_params, serial_com, m16c_cmd);
			break;
		case CMD_READ:
			arg_my_params.from_addr = arg_my_params.from_addr & 0xFFFFFF00;
			arg_my_params.to_addr = arg_my_params.to_addr | 0xFF;

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_read_flash_to_file(arg_my_params, serial_com, m16c_cmd);
			}

			break;
		case CMD_BLANK_CHK:
			arg_my_params.from_addr = arg_my_params.from_addr & 0xFFFFFF00;
			arg_my_params.to_addr = arg_my_params.to_addr | 0xFF;

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_blank_check(arg_my_params, serial_com, m16c_cmd, is_blank);
			}

			break;
		case CMD_ERASE:
			arg_my_params.from_addr = arg_my_params.from_addr & 0xFFFFFF00;
			arg_my_params.to_addr = arg_my_params.to_addr | 0xFF;

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_clear_status(arg_my_params, serial_com, m16c_cmd);
				process_disable_lock_bit(arg_my_params, serial_com, m16c_cmd);
				process_erase_blocks(arg_my_params, serial_com, m16c_cmd, arg_my_params.from_addr, arg_my_params.to_addr);
			}

			break;
		case CMD_ERASE_UNLOCKED:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_clear_status(arg_my_params, serial_com, m16c_cmd);
				process_disable_lock_bit(arg_my_params, serial_com, m16c_cmd);
				process_erase_all_unlocked(arg_my_params, serial_com, m16c_cmd);
			}

			break;
		case CMD_READ_LOCK_BIT:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				m16c_cmd.read_lock_bit(serial_com, arg_my_params.from_addr);
				if(m16c_cmd.lock_bit == 0){
					std::cout << "Lock bit = 0, block is flagged as locked" << std::endl;
				}else{
					std::cout << "Lock bit = 1, block is flagged as not locked" << std::endl;
				}
			}

			break;
		case CMD_PROGRAM_LOCK_BIT:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_clear_status(arg_my_params, serial_com, m16c_cmd);
				m16c_cmd.lock_bit_program(serial_com, arg_my_params.from_addr);
				std::cout << "Lock bit is programmed flagged locked" << std::endl;
			}

			break;
		case CMD_LOCK_BIT_ENABLE:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				m16c_cmd.lock_bit_enable(serial_com);
				std::cout << "Lock bit enabled" << std::endl;
			}

			break;
		case CMD_LOCK_BIT_DISABLE:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				m16c_cmd.lock_bit_disable(serial_com);
				std::cout << "Lock bit disabled" << std::endl;
			}

			break;
		case CMD_PROGRAM:
			arg_my_params.from_addr = arg_my_params.from_addr & 0xFFFFFF00;
			arg_my_params.to_addr = arg_my_params.to_addr | 0xFF;

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_clear_status(arg_my_params, serial_com, m16c_cmd);
				process_disable_lock_bit(arg_my_params, serial_com, m16c_cmd);
				process_program_flash_from_file(arg_my_params, serial_com, m16c_cmd);
			}

			break;
		case CMD_DOWNLOAD:
			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_download_from_file(arg_my_params, serial_com, m16c_cmd);
			}

			break;
		case CMD_BOOT_AREA_OUTPUT:
			arg_my_params.from_addr = arg_my_params.from_addr & 0xFFFFFF00;
			arg_my_params.to_addr = arg_my_params.to_addr | 0xFF;

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				std::cout << "ID is already previously matching, skipping ID check" << std::endl;
			}else{
				process_id_chk(arg_my_params, serial_com, m16c_cmd);
			}

			m16c_cmd.rd_status(serial_com);
			if((m16c_cmd.srd[1] & 0x0C) == 0x0C){
				process_boot_area_output_to_file(arg_my_params, serial_com, m16c_cmd);
			}

			break;
	}
}

int main(int arg_c, char *arg_v[]){
	cl_my_params my_params;

	try{
		if(arg_c > 1){
			parse_params(arg_c, arg_v, my_params);
			process_cmd_line(my_params);
		}else{
			usage(arg_v[0]);
		}
	}catch(tru_exception &ex){
		std::cout << "\nError: " << ex.get_error() << std::endl;
		return ex.get_code();
	}

	return 0;
}
