#ifndef M16C_MEM_MAP_H
#define M16C_MEM_MAP_H

#include <string>
#include <list>
#include <cstdint>

// For block A this source file numbers it as block 255
#define FLASH_BLOCK_A_NAME   "A"
#define FLASH_BLOCK_A_NR     255
#define FLASH_BLOCK_A_BEGIN  0x00F000
#define FLASH_BLOCK_A_END    0x00FFFF
#define FLASH_BLOCK_A_SIZE   4096
#define FLASH_BLOCK_12_NAME  "12"
#define FLASH_BLOCK_12_NR    12
#define FLASH_BLOCK_12_BEGIN 0x080000
#define FLASH_BLOCK_12_END   0x08FFFF
#define FLASH_BLOCK_12_SIZE  65536
#define FLASH_BLOCK_11_NAME  "11"
#define FLASH_BLOCK_11_NR    11
#define FLASH_BLOCK_11_BEGIN 0x090000
#define FLASH_BLOCK_11_END   0x09FFFF
#define FLASH_BLOCK_11_SIZE  65536
#define FLASH_BLOCK_10_NAME  "10"
#define FLASH_BLOCK_10_NR    10
#define FLASH_BLOCK_10_BEGIN 0x0A0000
#define FLASH_BLOCK_10_END   0x0AFFFF
#define FLASH_BLOCK_10_SIZE  65536
#define FLASH_BLOCK_9_NAME   "9"
#define FLASH_BLOCK_9_NR     9
#define FLASH_BLOCK_9_BEGIN  0x0B0000
#define FLASH_BLOCK_9_END    0x0BFFFF
#define FLASH_BLOCK_9_SIZE   65536
#define FLASH_BLOCK_8_NAME   "8"
#define FLASH_BLOCK_8_NR     8
#define FLASH_BLOCK_8_BEGIN  0x0C0000
#define FLASH_BLOCK_8_END    0x0CFFFF
#define FLASH_BLOCK_8_SIZE   65536
#define FLASH_BLOCK_7_NAME   "7"
#define FLASH_BLOCK_7_NR     7
#define FLASH_BLOCK_7_BEGIN  0x0D0000
#define FLASH_BLOCK_7_END    0x0DFFFF
#define FLASH_BLOCK_7_SIZE   65536
#define FLASH_BLOCK_6_NAME   "6"
#define FLASH_BLOCK_6_NR     6
#define FLASH_BLOCK_6_BEGIN  0x0E0000
#define FLASH_BLOCK_6_END    0x0EFFFF
#define FLASH_BLOCK_6_SIZE   65536
#define FLASH_BLOCK_5_NAME   "5"
#define FLASH_BLOCK_5_NR     5
#define FLASH_BLOCK_5_BEGIN  0x0F0000
#define FLASH_BLOCK_5_END    0x0F7FFF
#define FLASH_BLOCK_5_SIZE   32768
#define FLASH_BLOCK_4_NAME   "4"
#define FLASH_BLOCK_4_NR     4
#define FLASH_BLOCK_4_BEGIN  0x0F8000
#define FLASH_BLOCK_4_END    0x0F9FFF
#define FLASH_BLOCK_4_SIZE   8192
#define FLASH_BLOCK_3_NAME   "3"
#define FLASH_BLOCK_3_NR     3
#define FLASH_BLOCK_3_BEGIN  0x0FA000
#define FLASH_BLOCK_3_END    0x0FBFFF
#define FLASH_BLOCK_3_SIZE   8192
#define FLASH_BLOCK_2_NAME   "2"
#define FLASH_BLOCK_2_NR     2
#define FLASH_BLOCK_2_BEGIN  0x0FC000
#define FLASH_BLOCK_2_END    0x0FDFFF
#define FLASH_BLOCK_2_SIZE   8192
#define FLASH_BLOCK_1_NAME   "1"
#define FLASH_BLOCK_1_NR     1
#define FLASH_BLOCK_1_BEGIN  0x0FE000
#define FLASH_BLOCK_1_END    0x0FEFFF
#define FLASH_BLOCK_1_SIZE   4096
#define FLASH_BLOCK_0_NAME   "0"
#define FLASH_BLOCK_0_NR     0
#define FLASH_BLOCK_0_BEGIN  0x0FF000
#define FLASH_BLOCK_0_END    0x0FFFFF
#define FLASH_BLOCK_0_SIZE   4096
#define TOTAL_FLASH_BLOCKS   14

#define FLASH_MAX_BLOCK_SIZE  65536

class cl_m16c_flash_block{
public:
	std::string m_block_name = "";
	unsigned char m_block_nr = 0;
	uint32_t m_block_begin = 0;
	uint32_t m_block_end = 0;
	uint32_t m_block_size = 0;


	cl_m16c_flash_block(){
	}
	cl_m16c_flash_block(
		std::string m_arg_block_name,
		unsigned char m_arg_block_nr,
		uint32_t m_arg_block_begin,
		uint32_t m_arg_block_end,
		uint32_t m_arg_block_size
	) :
		m_block_name(m_arg_block_name),
		m_block_nr(m_arg_block_nr),
		m_block_begin(m_arg_block_begin),
		m_block_end(m_arg_block_end),
		m_block_size(m_arg_block_size){
	}
};

class cl_m16c_mem_map{
public:
	static const cl_m16c_flash_block m_m16c_flash_blocks[];

	// Converts address to a flash block info
	static bool addr_to_flash_block(uint32_t m_arg_addr, cl_m16c_flash_block& m_arg_flash_block){
		uint32_t m_i;

		for(m_i = 0; m_i < TOTAL_FLASH_BLOCKS; ++m_i){
			if((m_arg_addr >= m_m16c_flash_blocks[m_i].m_block_begin) && (m_arg_addr <= m_m16c_flash_blocks[m_i].m_block_end)){
				m_arg_flash_block = m_m16c_flash_blocks[m_i];
				return true;
			}
		}

		return false;
	}

	// Converts address range to a flash block list info
	static void addr_range_to_flash_block_list(uint32_t m_arg_addr_begin, uint32_t m_arg_addr_end, std::list<cl_m16c_flash_block>& m_arg_flash_block_list){
		cl_m16c_flash_block m_flash_block;
		uint32_t m_i;

		m_arg_flash_block_list.clear();

		for(m_i = 0; m_i < TOTAL_FLASH_BLOCKS; ++m_i){
			if((m_arg_addr_begin <= m_m16c_flash_blocks[m_i].m_block_end) && (m_arg_addr_end >= m_m16c_flash_blocks[m_i].m_block_begin)){
				m_arg_flash_block_list.push_back(m_m16c_flash_blocks[m_i]);
			}
		}
	}
};

#endif
