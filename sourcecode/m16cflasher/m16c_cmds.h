#ifndef M16C_CMDS_H
#define M16C_CMDS_H

#include "serial_com.h"
#include "my_buf.h"

#define DEFAULT_BAUD_RATE 57600

#define SLEEP_CMD_INTERVAL 5
#define SLEEP_AUTO_BAUD_INTERVAL 40

#define M16C_BL_CMD_CODE_9600                  0xB0
#define M16C_BL_CMD_CODE_19200                 0xB1
#define M16C_BL_CMD_CODE_38400                 0xB2
#define M16C_BL_CMD_CODE_57600                 0xB3
#define M16C_BL_CMD_CODE_115200                0xB4
#define M16C_BL_CMD_CODE_PAGE_READ             0xFF
#define M16C_BL_CMD_CODE_PAGE_PROGRAM          0x41
#define M16C_BL_CMD_CODE_BLOCK_ERASE           0x20
#define M16C_BL_CMD_CODE_ERASE_ALL_UNLOCKED    0xA7
#define M16C_BL_CMD_CODE_READ_STATUS_REG       0x70
#define M16C_BL_CMD_CODE_CLEAR_STATUS_REG      0x50
#define M16C_BL_CMD_CODE_READ_LOCK_BIT_STATUS  0x71
#define M16C_BL_CMD_CODE_LOCK_BIT_PROGRAM      0x77
#define M16C_BL_CMD_CODE_LOCK_BIT_ENABLE       0x7A
#define M16C_BL_CMD_CODE_LOCK_BIT_DISABLE      0x75
#define M16C_BL_CMD_CODE_ID_CHECK              0xF5
#define M16C_BL_CMD_CODE_DOWNLOAD              0xFA
#define M16C_BL_CMD_CODE_VERSION               0xFB
#define M16C_BL_CMD_CODE_BOOT_AREA_OUTPUT      0xFC

class cl_m16c_cmd{
public:
	unsigned char m_ver[8] = {};
	unsigned char m_srd[2] = {};  //Status Register
	unsigned char m_lock_bit = 0;  //0 = locked, 1 = unlocked

	cl_m16c_cmd();
	unsigned char baud_rate_cmd_code(uint32_t m_arg_baud_rate);
	void auto_baud(serial_com& m_arg_serial_com);
	void set_baud_rate(serial_com& m_arg_serial_com, uint32_t m_arg_baud_rate);
	void rd_version(serial_com& m_arg_serial_com);
	void rd_status(serial_com& m_arg_serial_com);
	void id_chk(serial_com& m_arg_serial_com, uint32_t m_arg_id_addr, std::string m_arg_id_hex_str);
	void page_rd(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256);
	void clear_status(serial_com& m_arg_serial_com);
	void page_program(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256);
	void erase_block(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr);
	void erase_all_unlocked(serial_com& m_arg_serial_com);
	void read_lock_bit(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr);
	void lock_bit_program(serial_com& m_arg_serial_com, uint32_t m_arg_block_highest_addr);
	void lock_bit_enable(serial_com& m_arg_serial_com);
	void lock_bit_disable(serial_com& m_arg_serial_com);
	unsigned char calc_checksum(uint16_t m_arg_buf_len, void* m_arg_buf);
	void download_calc_checksum(serial_com& m_arg_serial_com, uint16_t m_arg_buf_len, void* m_arg_buf);
	void download(serial_com& m_arg_serial_com, unsigned char m_arg_checksum, uint16_t m_arg_buf_len, void* m_arg_buf);
	void boot_area_output(serial_com& m_arg_serial_com, uint32_t m_arg_from_addr, void* m_arg_buf_256);
	bool is_srd_id_verified();
	bool is_srd_write_state_ready();
	bool is_srd_write_state_busy();
	bool is_srd_erase_ok();
	bool is_srd_erase_error();
	bool is_srd_program_ok();
	bool is_srd_program_error();
	bool is_srd_excessive_ok();
	bool is_srd_excessive_error();
	bool is_srd_program_and_excessive_ok();
	bool is_srd_program_and_excessive_error();
	bool is_srd_rx_time_out_no_error();
	bool is_srd_rx_time_out_error();
	bool is_srd_download_checksum_match();
	bool is_srd_download_checksum_mismatch();
	bool is_srd_download_completed();
	bool is_srd_download_not_completed();
};

#endif
