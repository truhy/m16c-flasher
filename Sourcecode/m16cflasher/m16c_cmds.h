#ifndef M16C_CMDS_H
#define M16C_CMDS_H

#include "serial_com.h"
#include "my_buf.h"

#define DEFAULT_BAUD_RATE 9600

//#define SLEEP_BYTE_INTERVAL 15
#define SLEEP_BYTE_INTERVAL 0
#define SLEEP_CMD_INTERVAL 50

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
protected:
	std::string m_last_err_msg;

public:
	unsigned char m_ver[8];
	unsigned char m_srd[2];  //Status Register
	unsigned char m_lock_bit;  //0 = locked, 1 = unlocked

	cl_m16c_cmd();
	std::string last_err_msg();
	size_t last_err_msg_len();
	unsigned char baud_rate_cmd_code(my_uint32 m_arg_baud_rate);
	my_bool init_baud_rate(cl_serial_com& m_arg_serial_com);
	my_bool set_baud_rate(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_baud_rate);
	my_bool rd_version(cl_serial_com& m_arg_serial_com);
	my_bool rd_status(cl_serial_com& m_arg_serial_com);
	my_bool id_chk(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_id_addr, std::string m_arg_id_hex_str);
	my_bool page_rd(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256);
	my_bool clear_status(cl_serial_com& m_arg_serial_com);
	my_bool page_program(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256);
	my_bool erase_block(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr);
	my_bool erase_all_unlocked(cl_serial_com& m_arg_serial_com);
	my_bool read_lock_bit(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr);
	my_bool lock_bit_program(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_block_highest_addr);
	my_bool lock_bit_enable(cl_serial_com& m_arg_serial_com);
	my_bool lock_bit_disable(cl_serial_com& m_arg_serial_com);
	unsigned char calc_checksum(my_uint16 m_arg_buf_len, void* m_arg_buf);
	my_bool download_calc_checksum(cl_serial_com& m_arg_serial_com, my_uint16 m_arg_buf_len, void* m_arg_buf);
	my_bool download(cl_serial_com& m_arg_serial_com, unsigned char m_arg_checksum, my_uint16 m_arg_buf_len, void* m_arg_buf);
	my_bool boot_area_output(cl_serial_com& m_arg_serial_com, my_uint32 m_arg_from_addr, void* m_arg_buf_256);
	my_bool is_srd_id_verified();
	my_bool is_srd_write_state_ready();
	my_bool is_srd_write_state_busy();
	my_bool is_srd_erase_ok();
	my_bool is_srd_erase_error();
	my_bool is_srd_program_ok();
	my_bool is_srd_program_error();
	my_bool is_srd_excessive_ok();
	my_bool is_srd_excessive_error();
	my_bool is_srd_program_and_excessive_ok();
	my_bool is_srd_program_and_excessive_error();
	my_bool is_srd_rx_time_out_no_error();
	my_bool is_srd_rx_time_out_error();
	my_bool is_srd_download_checksum_match();
	my_bool is_srd_download_checksum_mismatch();
	my_bool is_srd_download_completed();
	my_bool is_srd_download_not_completed();
};

#endif
