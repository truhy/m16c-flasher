#include "m16c_mem_map.h"

// Represents the MCU group full capacity flash memory blocks

cl_m16c_flash_block *cl_m16c_mem_map::flash_blocks = NULL;
uint32_t cl_m16c_mem_map::num_flash_blocks = 0;
uint32_t cl_m16c_mem_map::largest_block_size = 0;

// M16C/26 group 24K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_24k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_24k = sizeof(flash_blocks_m16c26_24k) / sizeof(cl_m16c_flash_block);

// M16C/26 group 48K fmask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_48k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_48k = sizeof(flash_blocks_m16c26_48k) / sizeof(cl_m16c_flash_block);

// M16C/26 group 64K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_64k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_64k = sizeof(flash_blocks_m16c26_64k) / sizeof(cl_m16c_flash_block);

// M16C/26 group 24K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_24k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_24k4k = sizeof(flash_blocks_m16c26_24k4k) / sizeof(cl_m16c_flash_block);

// M16C/26 group 48K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_48k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_48k4k = sizeof(flash_blocks_m16c26_48k4k) / sizeof(cl_m16c_flash_block);

// M16C/26 group 64K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c26_64k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c26_64k4k = sizeof(flash_blocks_m16c26_64k4k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 48K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_48k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_48k = sizeof(flash_blocks_m16c28_48k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 64K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_64k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_64k = sizeof(flash_blocks_m16c28_64k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 96K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_96k[] = {
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_96k = sizeof(flash_blocks_m16c28_96k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 128K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_128k[] = {
	cl_m16c_flash_block(ROM1, "5", 5, 0x0e0000, 0x0e7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_128k = sizeof(flash_blocks_m16c28_128k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 48K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_48k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_48k4k = sizeof(flash_blocks_m16c28_48k4k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 64K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_64k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_64k4k = sizeof(flash_blocks_m16c28_64k4k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 96K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_96k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_96k4k = sizeof(flash_blocks_m16c28_96k4k) / sizeof(cl_m16c_flash_block);

// M16C/28 group 128K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c28_128k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0e0000, 0x0e7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c28_128k4k = sizeof(flash_blocks_m16c28_128k4k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 64K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_64k[] = {
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_64k = sizeof(flash_blocks_m16c29_64k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 96K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_96k[] = {
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_96k = sizeof(flash_blocks_m16c29_96k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 128K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_128k[] = {
	cl_m16c_flash_block(ROM1, "5", 5, 0x0e0000, 0x0e7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_128k = sizeof(flash_blocks_m16c29_128k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 64K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_64k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_64k4k = sizeof(flash_blocks_m16c29_64k4k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 96K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_96k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_96k4k = sizeof(flash_blocks_m16c29_96k4k) / sizeof(cl_m16c_flash_block);

// M16C/29 group 128K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c29_128k4k[] = {
	cl_m16c_flash_block(DATA, "B", 129, 0x00f000, 0x00f7ff),
	cl_m16c_flash_block(DATA, "A", 128, 0x00f800, 0x00ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0e0000, 0x0e7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0f8000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0fe000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c29_128k4k = sizeof(flash_blocks_m16c29_128k4k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 96K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_96k[] = {
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_96k = sizeof(flash_blocks_m16c30_96k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 128K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_128k[] = {
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_128k = sizeof(flash_blocks_m16c30_128k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 160K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_160k[] = {
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d8000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_160k = sizeof(flash_blocks_m16c30_160k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 192K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_192k[] = {
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_192k = sizeof(flash_blocks_m16c30_192k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 256K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_256k[] = {
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_256k = sizeof(flash_blocks_m16c30_256k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 96K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_96k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_96k4k = sizeof(flash_blocks_m16c30_96k4k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 128K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_128k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_128k4k = sizeof(flash_blocks_m16c30_128k4k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 160K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_160k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d8000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_160k4k = sizeof(flash_blocks_m16c30_160k4k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 192K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_192k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_192k4k = sizeof(flash_blocks_m16c30_192k4k) / sizeof(cl_m16c_flash_block);

// M16C/30 group 256K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c30_256k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c30_256k4k = sizeof(flash_blocks_m16c30_256k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 48K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_48k[] = {
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_48k = sizeof(flash_blocks_m16c62_48k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 64K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_64k[] = {
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_64k = sizeof(flash_blocks_m16c62_64k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 96K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_96k[] = {
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_96k = sizeof(flash_blocks_m16c62_96k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 128K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_128k[] = {
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_128k = sizeof(flash_blocks_m16c62_128k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 192K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_192k[] = {
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_192k = sizeof(flash_blocks_m16c62_192k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 256K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_256k[] = {
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_256k = sizeof(flash_blocks_m16c62_256k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 320K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_320k[] = {
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_320k = sizeof(flash_blocks_m16c62_320k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 384K fmask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_384k[] = {
	cl_m16c_flash_block(ROM1, "10", 10, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_384k = sizeof(flash_blocks_m16c62_384k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 512K mask ROM blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_512k[] = {
	cl_m16c_flash_block(ROM1, "12", 12, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "11", 11, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "10", 10, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_512k = sizeof(flash_blocks_m16c62_512k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 48K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_48k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f4000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_48k4k = sizeof(flash_blocks_m16c62_48k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 64K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_64k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_64k4k = sizeof(flash_blocks_m16c62_64k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 96K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_96k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e8000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_96k4k = sizeof(flash_blocks_m16c62_96k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 128K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_128k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_128k4k = sizeof(flash_blocks_m16c62_128k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 192K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_192k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_192k4k = sizeof(flash_blocks_m16c62_192k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 256K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_256k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_256k4k = sizeof(flash_blocks_m16c62_256k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 320K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_320k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_320k4k = sizeof(flash_blocks_m16c62_320k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 384K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_384k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "10", 10, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_384k4k = sizeof(flash_blocks_m16c62_384k4k) / sizeof(cl_m16c_flash_block);

// M16C/62 group 512K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c62_512k4k[] = {
	cl_m16c_flash_block(DATA, "A", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM1, "12", 12, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "11", 11, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "10", 10, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0f0000, 0x0f7fff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0f8000, 0x0f9fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0fa000, 0x0fbfff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0fc000, 0x0fdfff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0fe000, 0x0fefff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0ff000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c62_512k4k = sizeof(flash_blocks_m16c62_512k4k) / sizeof(cl_m16c_flash_block);

// M16C/63 group 128K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c63_128k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c63_128k16k4k = sizeof(flash_blocks_m16c63_128k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/63 group 256K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c63_256k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c63_256k16k4k = sizeof(flash_blocks_m16c63_256k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/63 group 384K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c63_384k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c63_384k16k4k = sizeof(flash_blocks_m16c63_384k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/63 group 512K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c63_512k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c63_512k16k4k = sizeof(flash_blocks_m16c63_512k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/64 group 128K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c64_128k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c64_128k16k4k = sizeof(flash_blocks_m16c64_128k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/64 group 256K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c64_256k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c64_256k16k4k = sizeof(flash_blocks_m16c64_256k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/64 group 512K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c64_512k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c64_512k16k4k = sizeof(flash_blocks_m16c64_512k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 128K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_128k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_128k16k4k = sizeof(flash_blocks_m16c65_128k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 256K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_256k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_256k16k4k = sizeof(flash_blocks_m16c65_256k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 384K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_384k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_384k16k4k = sizeof(flash_blocks_m16c65_384k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 512K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_512k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_512k16k4k = sizeof(flash_blocks_m16c65_512k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 640K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_640k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x060000, 0x06ffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x070000, 0x07ffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_640k16k4k = sizeof(flash_blocks_m16c65_640k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/65 group 768K+16K+4K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c65_768k16k4k[] = {
	cl_m16c_flash_block(DATA, "A", 129, 0x00e000, 0x00efff),
	cl_m16c_flash_block(DATA, "B", 128, 0x00f000, 0x00ffff),
	cl_m16c_flash_block(ROM2, "64", 64, 0x010000, 0x013fff),
	cl_m16c_flash_block(ROM1, "11", 11, 0x040000, 0x04ffff),
	cl_m16c_flash_block(ROM1, "10", 10, 0x050000, 0x05ffff),
	cl_m16c_flash_block(ROM1, "9", 9, 0x060000, 0x06ffff),
	cl_m16c_flash_block(ROM1, "8", 8, 0x070000, 0x07ffff),
	cl_m16c_flash_block(ROM1, "7", 7, 0x080000, 0x08ffff),
	cl_m16c_flash_block(ROM1, "6", 6, 0x090000, 0x09ffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0x0a0000, 0x0affff),
	cl_m16c_flash_block(ROM1, "4", 4, 0x0b0000, 0x0bffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0x0c0000, 0x0cffff),
	cl_m16c_flash_block(ROM1, "2", 2, 0x0d0000, 0x0dffff),
	cl_m16c_flash_block(ROM1, "1", 1, 0x0e0000, 0x0effff),
	cl_m16c_flash_block(ROM1, "0", 0, 0x0f0000, 0x0fffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c65_768k16k4k = sizeof(flash_blocks_m16c65_768k16k4k) / sizeof(cl_m16c_flash_block);

// M16C/80 group 128K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c80_128k[] = {
	cl_m16c_flash_block(ROM1, "4", 4, 0xfe0000, 0xfeffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0xff0000, 0xff7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0xff8000, 0xff9fff),
	cl_m16c_flash_block(ROM1, "1", 1, 0xfa0000, 0xffbfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0xfc0000, 0xffffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c80_128k = sizeof(flash_blocks_m16c80_128k) / sizeof(cl_m16c_flash_block);

// M16C/80 group 256K flash memory blocks
const cl_m16c_flash_block cl_m16c_mem_map::flash_blocks_m16c80_256k[] = {
	cl_m16c_flash_block(ROM1, "6", 6, 0xfc0000, 0xfeffff),
	cl_m16c_flash_block(ROM1, "5", 5, 0xfd0000, 0xfdffff),
	cl_m16c_flash_block(ROM1, "4", 4, 0xfe0000, 0xfeffff),
	cl_m16c_flash_block(ROM1, "3", 3, 0xff0000, 0xff7fff),
	cl_m16c_flash_block(ROM1, "2", 2, 0xff8000, 0xff9fff),
	cl_m16c_flash_block(ROM1, "1", 1, 0xfa0000, 0xffbfff),
	cl_m16c_flash_block(ROM1, "0", 0, 0xfc0000, 0xffffff)
};
const uint32_t cl_m16c_mem_map::num_flash_blocks_m16c80_256k = sizeof(flash_blocks_m16c80_256k) / sizeof(cl_m16c_flash_block);

void cl_m16c_mem_map::select_group(mcu_group_e group){
	switch(group){
		case GROUP_M16C26_24K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_24k;
			num_flash_blocks = num_flash_blocks_m16c26_24k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C26_48K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_48k;
			num_flash_blocks = num_flash_blocks_m16c26_48k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C26_64K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_64k;
			num_flash_blocks = num_flash_blocks_m16c26_64k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C26_24K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_24k4k;
			num_flash_blocks = num_flash_blocks_m16c26_24k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C26_48K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_48k4k;
			num_flash_blocks = num_flash_blocks_m16c26_48k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C26_64K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c26_64k4k;
			num_flash_blocks = num_flash_blocks_m16c26_64k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_48K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_48k;
			num_flash_blocks = num_flash_blocks_m16c28_48k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_64K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_64k;
			num_flash_blocks = num_flash_blocks_m16c28_64k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_96K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_96k;
			num_flash_blocks = num_flash_blocks_m16c28_96k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_128K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_128k;
			num_flash_blocks = num_flash_blocks_m16c28_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_48K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_48k4k;
			num_flash_blocks = num_flash_blocks_m16c28_48k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_64K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_64k4k;
			num_flash_blocks = num_flash_blocks_m16c28_64k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_96K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_96k4k;
			num_flash_blocks = num_flash_blocks_m16c28_96k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C28_128K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c28_128k4k;
			num_flash_blocks = num_flash_blocks_m16c28_128k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_64K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_64k;
			num_flash_blocks = num_flash_blocks_m16c29_64k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_96K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_96k;
			num_flash_blocks = num_flash_blocks_m16c29_96k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_128K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_128k;
			num_flash_blocks = num_flash_blocks_m16c29_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_64K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_64k4k;
			num_flash_blocks = num_flash_blocks_m16c29_64k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_96K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_96k4k;
			num_flash_blocks = num_flash_blocks_m16c29_96k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C29_128K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c29_128k4k;
			num_flash_blocks = num_flash_blocks_m16c29_128k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_96K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_96k;
			num_flash_blocks = num_flash_blocks_m16c30_96k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_128K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_128k;
			num_flash_blocks = num_flash_blocks_m16c30_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_160K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_160k;
			num_flash_blocks = num_flash_blocks_m16c30_160k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_192K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_192k;
			num_flash_blocks = num_flash_blocks_m16c30_192k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_256K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_256k;
			num_flash_blocks = num_flash_blocks_m16c30_256k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_96K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_96k4k;
			num_flash_blocks = num_flash_blocks_m16c30_96k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_128K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_128k4k;
			num_flash_blocks = num_flash_blocks_m16c30_128k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_160K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_160k4k;
			num_flash_blocks = num_flash_blocks_m16c30_160k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_192K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_192k4k;
			num_flash_blocks = num_flash_blocks_m16c30_192k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C30_256K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c30_256k4k;
			num_flash_blocks = num_flash_blocks_m16c30_256k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_48K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_48k;
			num_flash_blocks = num_flash_blocks_m16c62_48k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_64K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_64k;
			num_flash_blocks = num_flash_blocks_m16c62_64k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_96K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_96k;
			num_flash_blocks = num_flash_blocks_m16c62_96k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_128K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_128k;
			num_flash_blocks = num_flash_blocks_m16c62_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_192K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_192k;
			num_flash_blocks = num_flash_blocks_m16c62_192k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_256K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_256k;
			num_flash_blocks = num_flash_blocks_m16c62_256k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_320K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_320k;
			num_flash_blocks = num_flash_blocks_m16c62_320k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_384K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_384k;
			num_flash_blocks = num_flash_blocks_m16c62_384k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_512K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_512k;
			num_flash_blocks = num_flash_blocks_m16c62_512k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_48K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_48k4k;
			num_flash_blocks = num_flash_blocks_m16c62_48k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_64K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_64k4k;
			num_flash_blocks = num_flash_blocks_m16c62_64k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_96K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_96k4k;
			num_flash_blocks = num_flash_blocks_m16c62_96k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_128K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_128k;
			num_flash_blocks = num_flash_blocks_m16c62_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_192K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_192k4k;
			num_flash_blocks = num_flash_blocks_m16c62_192k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_256K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_256k4k;
			num_flash_blocks = num_flash_blocks_m16c62_256k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_320K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_320k4k;
			num_flash_blocks = num_flash_blocks_m16c62_320k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_384K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_384k4k;
			num_flash_blocks = num_flash_blocks_m16c62_384k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C62_512K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c62_512k4k;
			num_flash_blocks = num_flash_blocks_m16c62_512k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C63_128K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c63_128k16k4k;
			num_flash_blocks = num_flash_blocks_m16c63_128k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C63_256K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c63_256k16k4k;
			num_flash_blocks = num_flash_blocks_m16c63_256k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C63_384K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c63_384k16k4k;
			num_flash_blocks = num_flash_blocks_m16c63_384k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C63_512K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c63_512k16k4k;
			num_flash_blocks = num_flash_blocks_m16c63_512k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C64_128K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c64_128k16k4k;
			num_flash_blocks = num_flash_blocks_m16c64_128k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C64_256K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c64_256k16k4k;
			num_flash_blocks = num_flash_blocks_m16c64_256k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C64_512K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c64_512k16k4k;
			num_flash_blocks = num_flash_blocks_m16c64_512k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_128K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_128k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_128k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_256K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_256k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_256k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_384K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_384k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_384k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_512K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_512k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_512k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_640K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_640k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_640k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C65_768K16K4K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c65_768k16k4k;
			num_flash_blocks = num_flash_blocks_m16c65_768k16k4k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C80_128K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c80_128k;
			num_flash_blocks = num_flash_blocks_m16c80_128k;
			set_largest_flash_block_size();
			break;
		case GROUP_M16C80_256K:
			flash_blocks = (cl_m16c_flash_block *)flash_blocks_m16c80_256k;
			num_flash_blocks = num_flash_blocks_m16c80_256k;
			set_largest_flash_block_size();
			break;
		case GROUP_NONE:
		default:
			break;
	}
}
