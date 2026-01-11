#ifndef M16C_MEM_MAP_H
#define M16C_MEM_MAP_H

#include <string>
#include <list>
#include <cstdint>

typedef enum{
	GROUP_NONE,
	GROUP_M16C26_64K,
	GROUP_M16C26_48K,
	GROUP_M16C26_24K,
	GROUP_M16C26_64K4K,
	GROUP_M16C26_48K4K,
	GROUP_M16C26_24K4K,
	GROUP_M16C28_48K,
	GROUP_M16C28_64K,
	GROUP_M16C28_96K,
	GROUP_M16C28_128K,
	GROUP_M16C28_48K4K,
	GROUP_M16C28_64K4K,
	GROUP_M16C28_96K4K,
	GROUP_M16C28_128K4K,
	GROUP_M16C29_64K,
	GROUP_M16C29_96K,
	GROUP_M16C29_128K,
	GROUP_M16C29_64K4K,
	GROUP_M16C29_96K4K,
	GROUP_M16C29_128K4K,
	GROUP_M16C30_96K,
	GROUP_M16C30_128K,
	GROUP_M16C30_160K,
	GROUP_M16C30_192K,
	GROUP_M16C30_256K,
	GROUP_M16C30_96K4K,
	GROUP_M16C30_128K4K,
	GROUP_M16C30_160K4K,
	GROUP_M16C30_192K4K,
	GROUP_M16C30_256K4K,
	GROUP_M16C62_48K,
	GROUP_M16C62_64K,
	GROUP_M16C62_96K,
	GROUP_M16C62_128K,
	GROUP_M16C62_192K,
	GROUP_M16C62_256K,
	GROUP_M16C62_320K,
	GROUP_M16C62_384K,
	GROUP_M16C62_512K,
	GROUP_M16C62_48K4K,
	GROUP_M16C62_64K4K,
	GROUP_M16C62_96K4K,
	GROUP_M16C62_128K4K,
	GROUP_M16C62_192K4K,
	GROUP_M16C62_256K4K,
	GROUP_M16C62_320K4K,
	GROUP_M16C62_384K4K,
	GROUP_M16C62_512K4K,
	GROUP_M16C63_128K16K4K,
	GROUP_M16C63_256K16K4K,
	GROUP_M16C63_384K16K4K,
	GROUP_M16C63_512K16K4K,
	GROUP_M16C64_128K16K4K,
	GROUP_M16C64_256K16K4K,
	GROUP_M16C64_512K16K4K,
	GROUP_M16C65_128K16K4K,
	GROUP_M16C65_256K16K4K,
	GROUP_M16C65_384K16K4K,
	GROUP_M16C65_512K16K4K,
	GROUP_M16C65_640K16K4K,
	GROUP_M16C65_768K16K4K,
	GROUP_M16C80_128K,
	GROUP_M16C80_256K,
}mcu_group_e;

typedef enum{
	NONE,
	DATA,
	ROM1,
	ROM2,
}mcu_flash_area_e;

class cl_m16c_flash_block{
public:
	mcu_flash_area_e flash_area;
	std::string block_name;
	unsigned char block_nr;
	uint32_t block_begin;
	uint32_t block_end;

	cl_m16c_flash_block() :
		flash_area(NONE),
		block_name(""),
		block_nr(0),
		block_begin(0),
		block_end(0){
	}
	cl_m16c_flash_block(
		mcu_flash_area_e arg_flash_area,
		std::string arg_block_name,
		uint32_t arg_block_nr,
		uint32_t arg_block_begin,
		uint32_t arg_block_end
	) :
		flash_area(arg_flash_area),
		block_name(arg_block_name),
		block_nr(arg_block_nr),
		block_begin(arg_block_begin),
		block_end(arg_block_end){
	}
};

class cl_m16c_mem_map{
public:
	static cl_m16c_flash_block *flash_blocks;
	static uint32_t num_flash_blocks;
	static uint32_t largest_block_size;

	static const cl_m16c_flash_block flash_blocks_m16c26_24k[];
	static const uint32_t num_flash_blocks_m16c26_24k;
	static const cl_m16c_flash_block flash_blocks_m16c26_48k[];
	static const uint32_t num_flash_blocks_m16c26_48k;
	static const cl_m16c_flash_block flash_blocks_m16c26_64k[];
	static const uint32_t num_flash_blocks_m16c26_64k;
	static const cl_m16c_flash_block flash_blocks_m16c26_24k4k[];
	static const uint32_t num_flash_blocks_m16c26_24k4k;
	static const cl_m16c_flash_block flash_blocks_m16c26_48k4k[];
	static const uint32_t num_flash_blocks_m16c26_48k4k;
	static const cl_m16c_flash_block flash_blocks_m16c26_64k4k[];
	static const uint32_t num_flash_blocks_m16c26_64k4k;

	static const cl_m16c_flash_block flash_blocks_m16c28_48k[];
	static const uint32_t num_flash_blocks_m16c28_48k;
	static const cl_m16c_flash_block flash_blocks_m16c28_64k[];
	static const uint32_t num_flash_blocks_m16c28_64k;
	static const cl_m16c_flash_block flash_blocks_m16c28_96k[];
	static const uint32_t num_flash_blocks_m16c28_96k;
	static const cl_m16c_flash_block flash_blocks_m16c28_128k[];
	static const uint32_t num_flash_blocks_m16c28_128k;
	static const cl_m16c_flash_block flash_blocks_m16c28_48k4k[];
	static const uint32_t num_flash_blocks_m16c28_48k4k;
	static const cl_m16c_flash_block flash_blocks_m16c28_64k4k[];
	static const uint32_t num_flash_blocks_m16c28_64k4k;
	static const cl_m16c_flash_block flash_blocks_m16c28_96k4k[];
	static const uint32_t num_flash_blocks_m16c28_96k4k;
	static const cl_m16c_flash_block flash_blocks_m16c28_128k4k[];
	static const uint32_t num_flash_blocks_m16c28_128k4k;

	static const cl_m16c_flash_block flash_blocks_m16c29_64k[];
	static const uint32_t num_flash_blocks_m16c29_64k;
	static const cl_m16c_flash_block flash_blocks_m16c29_96k[];
	static const uint32_t num_flash_blocks_m16c29_96k;
	static const cl_m16c_flash_block flash_blocks_m16c29_128k[];
	static const uint32_t num_flash_blocks_m16c29_128k;
	static const cl_m16c_flash_block flash_blocks_m16c29_64k4k[];
	static const uint32_t num_flash_blocks_m16c29_64k4k;
	static const cl_m16c_flash_block flash_blocks_m16c29_96k4k[];
	static const uint32_t num_flash_blocks_m16c29_96k4k;
	static const cl_m16c_flash_block flash_blocks_m16c29_128k4k[];
	static const uint32_t num_flash_blocks_m16c29_128k4k;

	static const cl_m16c_flash_block flash_blocks_m16c30_96k[];
	static const uint32_t num_flash_blocks_m16c30_96k;
	static const cl_m16c_flash_block flash_blocks_m16c30_128k[];
	static const uint32_t num_flash_blocks_m16c30_128k;
	static const cl_m16c_flash_block flash_blocks_m16c30_160k[];
	static const uint32_t num_flash_blocks_m16c30_160k;
	static const cl_m16c_flash_block flash_blocks_m16c30_192k[];
	static const uint32_t num_flash_blocks_m16c30_192k;
	static const cl_m16c_flash_block flash_blocks_m16c30_256k[];
	static const uint32_t num_flash_blocks_m16c30_256k;
	static const cl_m16c_flash_block flash_blocks_m16c30_96k4k[];
	static const uint32_t num_flash_blocks_m16c30_96k4k;
	static const cl_m16c_flash_block flash_blocks_m16c30_128k4k[];
	static const uint32_t num_flash_blocks_m16c30_128k4k;
	static const cl_m16c_flash_block flash_blocks_m16c30_160k4k[];
	static const uint32_t num_flash_blocks_m16c30_160k4k;
	static const cl_m16c_flash_block flash_blocks_m16c30_192k4k[];
	static const uint32_t num_flash_blocks_m16c30_192k4k;
	static const cl_m16c_flash_block flash_blocks_m16c30_256k4k[];
	static const uint32_t num_flash_blocks_m16c30_256k4k;
	
	static const cl_m16c_flash_block flash_blocks_m16c62_48k[];
	static const uint32_t num_flash_blocks_m16c62_48k;
	static const cl_m16c_flash_block flash_blocks_m16c62_64k[];
	static const uint32_t num_flash_blocks_m16c62_64k;
	static const cl_m16c_flash_block flash_blocks_m16c62_96k[];
	static const uint32_t num_flash_blocks_m16c62_96k;
	static const cl_m16c_flash_block flash_blocks_m16c62_128k[];
	static const uint32_t num_flash_blocks_m16c62_128k;
	static const cl_m16c_flash_block flash_blocks_m16c62_192k[];
	static const uint32_t num_flash_blocks_m16c62_192k;
	static const cl_m16c_flash_block flash_blocks_m16c62_256k[];
	static const uint32_t num_flash_blocks_m16c62_256k;
	static const cl_m16c_flash_block flash_blocks_m16c62_320k[];
	static const uint32_t num_flash_blocks_m16c62_320k;
	static const cl_m16c_flash_block flash_blocks_m16c62_384k[];
	static const uint32_t num_flash_blocks_m16c62_384k;
	static const cl_m16c_flash_block flash_blocks_m16c62_512k[];
	static const uint32_t num_flash_blocks_m16c62_512k;
	static const cl_m16c_flash_block flash_blocks_m16c62_48k4k[];
	static const uint32_t num_flash_blocks_m16c62_48k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_64k4k[];
	static const uint32_t num_flash_blocks_m16c62_64k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_96k4k[];
	static const uint32_t num_flash_blocks_m16c62_96k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_128k4k[];
	static const uint32_t num_flash_blocks_m16c62_128k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_192k4k[];
	static const uint32_t num_flash_blocks_m16c62_192k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_256k4k[];
	static const uint32_t num_flash_blocks_m16c62_256k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_320k4k[];
	static const uint32_t num_flash_blocks_m16c62_320k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_384k4k[];
	static const uint32_t num_flash_blocks_m16c62_384k4k;
	static const cl_m16c_flash_block flash_blocks_m16c62_512k4k[];
	static const uint32_t num_flash_blocks_m16c62_512k4k;

	static const cl_m16c_flash_block flash_blocks_m16c63_128k16k4k[];
	static const uint32_t num_flash_blocks_m16c63_128k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c63_256k16k4k[];
	static const uint32_t num_flash_blocks_m16c63_256k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c63_384k16k4k[];
	static const uint32_t num_flash_blocks_m16c63_384k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c63_512k16k4k[];
	static const uint32_t num_flash_blocks_m16c63_512k16k4k;

	static const cl_m16c_flash_block flash_blocks_m16c64_128k16k4k[];
	static const uint32_t num_flash_blocks_m16c64_128k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c64_256k16k4k[];
	static const uint32_t num_flash_blocks_m16c64_256k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c64_512k16k4k[];
	static const uint32_t num_flash_blocks_m16c64_512k16k4k;

	static const cl_m16c_flash_block flash_blocks_m16c65_128k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_128k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c65_256k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_256k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c65_384k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_384k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c65_512k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_512k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c65_640k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_640k16k4k;
	static const cl_m16c_flash_block flash_blocks_m16c65_768k16k4k[];
	static const uint32_t num_flash_blocks_m16c65_768k16k4k;

	static const cl_m16c_flash_block flash_blocks_m16c80_256k[];
	static const uint32_t num_flash_blocks_m16c80_256k;

	static const cl_m16c_flash_block flash_blocks_m16c80_128k[];
	static const uint32_t num_flash_blocks_m16c80_128k;

	static void set_largest_flash_block_size(){
		largest_block_size = 0;
		for(uint32_t i = 0; i < num_flash_blocks; ++i){
			uint32_t s = flash_blocks[i].block_end - flash_blocks[i].block_begin + 1;
			if(s > largest_block_size) largest_block_size = s;
		}
	}

	static void select_group(mcu_group_e group);

	// Convert address to a flash block
	static bool addr_to_flash_block(uint32_t arg_addr, cl_m16c_flash_block &arg_flash_block){
		uint32_t i;

		for(i = 0; i < num_flash_blocks; ++i){
			if((arg_addr >= flash_blocks[i].block_begin) && (arg_addr <= flash_blocks[i].block_end)){
				arg_flash_block = flash_blocks[i];
				return true;
			}
		}

		return false;
	}

	// Convert address range to a list of flash blocks
	static void addr_range_to_flash_block_list(uint32_t arg_addr_begin, uint32_t arg_addr_end, std::list<cl_m16c_flash_block> &arg_flash_block_list){
		cl_m16c_flash_block flash_block;
		uint32_t i;

		arg_flash_block_list.clear();

		for(i = 0; i < num_flash_blocks; i++){
			if(arg_addr_begin <= flash_blocks[i].block_end && arg_addr_end >= flash_blocks[i].block_begin){
				arg_flash_block_list.push_back(flash_blocks[i]);
			}
		}
	}
};

#endif
