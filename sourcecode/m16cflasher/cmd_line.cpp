#include "cmd_line.h"
#include <algorithm>

std::string str_toupper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c){ return std::toupper(c); } // correct
	);
	return s;
}

bool parse_param_exist(std::string arg_param, std::string arg_key){
	// Len of param is correct or longer?
	if(arg_param.size() == arg_key.size()){
		// Compares param to key word
		if(arg_param.compare(0, arg_key.size(), arg_key) == 0){
			return true;
		}
	}

	return false;
}

bool parse_param_str(std::string arg_param, std::string arg_key, std::string &arg_value){
	// Len of param is correct or longer?
	if(arg_param.size() >= (arg_key.size() + 1)){
		// Compares param to key word
		if(arg_param.compare(0, arg_key.size(), arg_key) == 0){
			arg_value = arg_param.substr(arg_key.size(), arg_param.size());

			return true;
		}
	}

	return false;
}

bool parse_param_yn(std::string arg_param, std::string arg_key, bool &arg_value){
	// Len of param is correct or longer?
	if(arg_param.size() >= (arg_key.size() + 1)){
		// Compares param to key word
		if(arg_param.compare(0, arg_key.size(), arg_key) == 0){
			if(arg_param.substr(arg_key.size(), 1) == "y"){
				arg_value = true;
			}else{
				arg_value = false;
			}

			return true;
		}
	}

	return false;
}

bool parse_param_hex_str(std::string arg_param, std::string arg_key, std::string &arg_value){
	char ch;
	std::string::size_type i;

	arg_value.clear();
	// Len of param is correct or longer?
	if(arg_param.size() >= (arg_key.size() + 1)){
		// Compares param to key word
		if(arg_param.compare(0, arg_key.size(), arg_key) == 0){
			for(i = 0; i < (arg_param.size() - arg_key.size()); i += 2){
				ch = (char)strtoul(arg_param.substr(arg_key.size() + i, 2).c_str(), NULL, 16);
				arg_value += ch;
			}
			return true;
		}
	}
	return false;
}

void usage(char *arg_0){
	printf("%s v1.3 - Jan 2026\n", arg_0);
	printf("Usage:\n");
	printf(" %s <devparams> <cmdparams>\n", arg_0);
	printf("devparams:\n");
	printf(" path=<s> : serial port path\n");
	printf(" baud=<n> : valid values:\n");
	printf("             9600\n");
	printf("             19200\n");
	printf("             38400\n");
	printf("             57600\n");
	printf("             115200\n");
	printf("\n");
	printf("cmdparams:\n");
	printf("ver              : read bootloader version\n");
	printf("autobaud         : perform auto-baud sequence\n");
	printf("status           : read status\n");
	printf("id_chk           : check id\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf("read             : read memory\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : from address\n");
	printf(" to_addr=<n>      : to address\n");
	printf(" file=<s>         : file\n");
	printf("blank_chk        : blank check\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : from address\n");
	printf(" to_addr=<n>      : to address\n");
	printf("erase            : erase blocks\n");
	printf(" mcu=<s>          : mcu\n");
	printf(" group=<s>        : mcu group\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : from address\n");
	printf(" to_addr=<n>      : to address\n");
	printf("erase_unlocked   : erase all unlocked blocks\n");
	printf(" mcu=<s>          : mcu\n");
	printf(" group=<s>        : mcu group\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf("read_lock_bit    : read lock bit of a block\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : highest address of block\n");
	printf("prog_lock_bit    : program lock bit of a block\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : highest address of block\n");
	printf("lock_on          : lock bit enable\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf("lock_off         : lock bit disable\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf("program          : program flash memory\n");
	printf(" mcu=<s>          : mcu\n");
	printf(" group=<s>        : mcu group\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : from address\n");
	printf(" to_addr=<n>      : to address\n");
	printf(" blank_chk=<y|n>  : enable blank check\n");
	printf(" erase=<y|n>      : enable erase\n");
	printf(" file=<s>         : file\n");
	printf("download         : download to ram and run\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" file=<s>         : file\n");
	printf("ba_output        : boot area output\n");
	printf(" id_addr=<n>      : addr of 1st byte of id\n");
	printf(" id=<hex>         : id hex string\n");
	printf(" from_addr=<n>    : from address\n");
	printf(" to_addr=<n>      : to address\n");
	printf(" file=<s>         : file\n");
	printf("\n");
	printf("mcu=\n");
	printf("  M3026xxx\n");
	printf("  M3028xxx\n");
	printf("  M3029xxx\n");
	printf("  M3030xxx\n");
	printf("  M3062xxx\n");
	printf("  R5F363xx\n");
	printf("  R5F364xx\n");
	printf("  R5F365xx\n");
	printf("  M3080xxx\n");
	printf("group=\n");
	printf("  m16c26_24k\n");
	printf("  m16c26_48k\n");
	printf("  m16c26_64k\n");
	printf("  m16c26_24k4k\n");
	printf("  m16c26_48k4k\n");
	printf("  m16c26_64k4k\n");
	printf("\n");
	printf("  m16c28_48k\n");
	printf("  m16c28_64k\n");
	printf("  m16c28_96k\n");
	printf("  m16c28_128k\n");
	printf("  m16c28_48k4k\n");
	printf("  m16c28_64k4k\n");
	printf("  m16c28_96k4k\n");
	printf("  m16c28_128k4k\n");
	printf("\n");
	printf("  m16c29_64k\n");
	printf("  m16c29_96k\n");
	printf("  m16c29_128k\n");
	printf("  m16c29_64k4k\n");
	printf("  m16c29_96k4k\n");
	printf("  m16c29_128k4k\n");
	printf("\n");
	printf("  m16c30_96k\n");
	printf("  m16c30_128k\n");
	printf("  m16c30_160k\n");
	printf("  m16c30_192k\n");
	printf("  m16c30_256k\n");
	printf("  m16c30_96k4k\n");
	printf("  m16c30_128k4k\n");
	printf("  m16c30_160k4k\n");
	printf("  m16c30_192k4k\n");
	printf("  m16c30_256k4k\n");
	printf("\n");
	printf("  m16c62_48k\n");
	printf("  m16c62_64k\n");
	printf("  m16c62_96k\n");
	printf("  m16c62_128k\n");
	printf("  m16c62_192k\n");
	printf("  m16c62_256k\n");
	printf("  m16c62_320k\n");
	printf("  m16c62_384k\n");
	printf("  m16c62_512k\n");
	printf("  m16c62_48k4k\n");
	printf("  m16c62_64k4k\n");
	printf("  m16c62_96k4k\n");
	printf("  m16c62_128k4k\n");
	printf("  m16c62_192k4k\n");
	printf("  m16c62_256k4k\n");
	printf("  m16c62_320k4k\n");
	printf("  m16c62_384k4k\n");
	printf("  m16c62_512k4k\n");
	printf("\n");
	printf("  m16c63_128k16k4k\n");
	printf("  m16c63_256k16k4k\n");
	printf("  m16c63_384k16k4k\n");
	printf("  m16c63_512k16k4k\n");
	printf("\n");
	printf("  m16c64_128k16k4k\n");
	printf("  m16c64_256K16k4k\n");
	printf("  m16c64_512k16k4k\n");
	printf("\n");
	printf("  m16c65_128k16k4k\n");
	printf("  m16c65_256k16k4k\n");
	printf("  m16c65_384k16k4k\n");
	printf("  m16c65_512k16k4k\n");
	printf("  m16c65_640k16k4k\n");
	printf("  m16c65_768k16k4k\n");
	printf("\n");
	printf("  m16c80_128k\n");
	printf("  m16c80_256k\n");
}

bool parse_params_search(char *arg_cmdl_param, cl_my_params &arg_my_params){
	if(parse_param_exist(arg_cmdl_param, "autobaud")){
		arg_my_params.cmd = CMD_AUTO_BAUD;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "ver")){
		arg_my_params.cmd = CMD_VER;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "status")){
		arg_my_params.cmd = CMD_STATUS;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "id_chk")){
		arg_my_params.cmd = CMD_ID_CHK;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "read")){
		arg_my_params.cmd = CMD_READ;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "blank_chk")){
		arg_my_params.cmd = CMD_BLANK_CHK;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "erase")){
		arg_my_params.cmd = CMD_ERASE;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "erase_unlocked")){
		arg_my_params.cmd = CMD_ERASE_UNLOCKED;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "read_lock_bit")){
		arg_my_params.cmd = CMD_READ_LOCK_BIT;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "prog_lock_bit")){
		arg_my_params.cmd = CMD_PROGRAM_LOCK_BIT;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "lock_on")){
		arg_my_params.cmd = CMD_LOCK_BIT_ENABLE;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "lock_off")){
		arg_my_params.cmd = CMD_LOCK_BIT_DISABLE;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "program")){
		arg_my_params.cmd = CMD_PROGRAM;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "download")){
		arg_my_params.cmd = CMD_DOWNLOAD;
		return true;
	}
	if(parse_param_exist(arg_cmdl_param, "ba_output")){
		arg_my_params.cmd = CMD_BOOT_AREA_OUTPUT;
		return true;
	}
	if(parse_param_str(arg_cmdl_param, "path=", arg_my_params.dev_path)){
		return true;
	}
	if(parse_param_val_uint(arg_cmdl_param, "baud=", arg_my_params.baud_rate)){
		return true;
	}
	if(parse_param_val_uint(arg_cmdl_param, "id_addr=", arg_my_params.id_addr)){
		return true;
	}
	if(parse_param_str(arg_cmdl_param, "id=", arg_my_params.id_hex_str)){
		return true;
	}
	if(parse_param_val_uint(arg_cmdl_param, "from_addr=", arg_my_params.from_addr)){
		return true;
	}
	if(parse_param_val_uint(arg_cmdl_param, "to_addr=", arg_my_params.to_addr)){
		return true;
	}
	if(parse_param_str(arg_cmdl_param, "file=", arg_my_params.full_file_name)){
		return true;
	}
	if(parse_param_yn(arg_cmdl_param, "blank_chk=", arg_my_params.is_blank_chk_before_program)){
		return true;
	}
	if(parse_param_yn(arg_cmdl_param, "erase=", arg_my_params.is_erase_before_program)){
		return true;
	}
	std::string str;
	if(parse_param_str(arg_cmdl_param, "mcu=", str)){
		str = str_toupper(str.substr(0, str.find("-")));
		
		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3026"){
				// Mask ROM
				if(str.substr(6, 2) == "M3"){
					arg_my_params.mcu_group = GROUP_M16C26_24K;
					return true;
				}
				if(str.substr(6, 2) == "M6"){
					arg_my_params.mcu_group = GROUP_M16C26_48K;
					return true;
				}
				if(str.substr(6, 2) == "M8"){
					arg_my_params.mcu_group = GROUP_M16C26_64K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "F3"){
					arg_my_params.mcu_group = GROUP_M16C26_24K4K;
					return true;
				}
				if(str.substr(6, 2) == "F6"){
					arg_my_params.mcu_group = GROUP_M16C26_48K4K;
					return true;
				}
				if(str.substr(6, 2) == "F8"){
					arg_my_params.mcu_group = GROUP_M16C26_64K4K;
					return true;
				}
			}
		}

		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3028"){
				// Mask ROM
				if(str.substr(6, 2) == "M6"){
					arg_my_params.mcu_group = GROUP_M16C28_48K;
					return true;
				}
				if(str.substr(6, 2) == "M8"){
					arg_my_params.mcu_group = GROUP_M16C28_64K;
					return true;
				}
				if(str.substr(6, 2) == "MA"){
					arg_my_params.mcu_group = GROUP_M16C28_96K;
					return true;
				}
				if(str.substr(6, 2) == "MC"){
					arg_my_params.mcu_group = GROUP_M16C28_128K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "F6"){
					arg_my_params.mcu_group = GROUP_M16C28_48K4K;
					return true;
				}
				if(str.substr(6, 2) == "F8"){
					arg_my_params.mcu_group = GROUP_M16C28_64K4K;
					return true;
				}
				if(str.substr(6, 2) == "FA"){
					arg_my_params.mcu_group = GROUP_M16C28_96K4K;
					return true;
				}
				if(str.substr(6, 2) == "FC"){
					arg_my_params.mcu_group = GROUP_M16C28_128K4K;
					return true;
				}
			}
		}
		
		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3029"){
				// Mask ROM
				if(str.substr(6, 2) == "M8"){
					arg_my_params.mcu_group = GROUP_M16C29_64K;
					return true;
				}
				if(str.substr(6, 2) == "MA"){
					arg_my_params.mcu_group = GROUP_M16C29_96K;
					return true;
				}
				if(str.substr(6, 2) == "MC"){
					arg_my_params.mcu_group = GROUP_M16C29_128K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "F8"){
					arg_my_params.mcu_group = GROUP_M16C29_64K4K;
					return true;
				}
				if(str.substr(6, 2) == "FA"){
					arg_my_params.mcu_group = GROUP_M16C29_96K4K;
					return true;
				}
				if(str.substr(6, 2) == "FC"){
					arg_my_params.mcu_group = GROUP_M16C29_128K4K;
					return true;
				}
			}
		}

		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3030"){
				// Mask ROM
				if(str.substr(6, 2) == "MA"){
					arg_my_params.mcu_group = GROUP_M16C30_96K;
					return true;
				}
				if(str.substr(6, 2) == "MC"){
					arg_my_params.mcu_group = GROUP_M16C30_128K;
					return true;
				}
				if(str.substr(6, 2) == "MD"){
					arg_my_params.mcu_group = GROUP_M16C30_160K;
					return true;
				}
				if(str.substr(6, 2) == "ME"){
					arg_my_params.mcu_group = GROUP_M16C30_192K;
					return true;
				}
				if(str.substr(6, 2) == "MG"){
					arg_my_params.mcu_group = GROUP_M16C30_256K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "FA"){
					arg_my_params.mcu_group = GROUP_M16C30_96K4K;
					return true;
				}
				if(str.substr(6, 2) == "FC"){
					arg_my_params.mcu_group = GROUP_M16C30_128K4K;
					return true;
				}
				if(str.substr(6, 2) == "FD"){
					arg_my_params.mcu_group = GROUP_M16C30_160K4K;
					return true;
				}
				if(str.substr(6, 2) == "FE"){
					arg_my_params.mcu_group = GROUP_M16C30_192K4K;
					return true;
				}
				if(str.substr(6, 2) == "FG"){
					arg_my_params.mcu_group = GROUP_M16C30_256K4K;
					return true;
				}
			}
		}
		
		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3062"){
				// Mask ROM
				if(str.substr(6, 2) == "M6"){
					arg_my_params.mcu_group = GROUP_M16C62_48K;
					return true;
				}
				if(str.substr(6, 2) == "M8"){
					arg_my_params.mcu_group = GROUP_M16C62_64K;
					return true;
				}
				if(str.substr(6, 2) == "MA"){
					arg_my_params.mcu_group = GROUP_M16C62_96K;
					return true;
				}
				if(str.substr(6, 2) == "MC"){
					arg_my_params.mcu_group = GROUP_M16C62_128K;
					return true;
				}
				if(str.substr(6, 2) == "ME"){
					arg_my_params.mcu_group = GROUP_M16C62_192K;
					return true;
				}
				if(str.substr(6, 2) == "MG"){
					arg_my_params.mcu_group = GROUP_M16C62_256K;
					return true;
				}
				if(str.substr(6, 2) == "MW"){
					arg_my_params.mcu_group = GROUP_M16C62_320K;
					return true;
				}
				if(str.substr(6, 2) == "MH"){
					arg_my_params.mcu_group = GROUP_M16C62_384K;
					return true;
				}
				if(str.substr(6, 2) == "MJ"){
					arg_my_params.mcu_group = GROUP_M16C62_512K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "F6"){
					arg_my_params.mcu_group = GROUP_M16C62_48K4K;
					return true;
				}
				if(str.substr(6, 2) == "F8"){
					arg_my_params.mcu_group = GROUP_M16C62_64K4K;
					return true;
				}
				if(str.substr(6, 2) == "FA"){
					arg_my_params.mcu_group = GROUP_M16C62_96K4K;
					return true;
				}
				if(str.substr(6, 2) == "FC"){
					arg_my_params.mcu_group = GROUP_M16C62_128K4K;
					return true;
				}
				if(str.substr(6, 2) == "FE"){
					arg_my_params.mcu_group = GROUP_M16C62_192K4K;
					return true;
				}
				if(str.substr(6, 2) == "FG"){
					arg_my_params.mcu_group = GROUP_M16C62_256K4K;
					return true;
				}
				if(str.substr(6, 2) == "FW"){
					arg_my_params.mcu_group = GROUP_M16C62_320K4K;
					return true;
				}
				if(str.substr(6, 2) == "FH"){
					arg_my_params.mcu_group = GROUP_M16C62_384K4K;
					return true;
				}
				if(str.substr(6, 2) == "FJ"){
					arg_my_params.mcu_group = GROUP_M16C62_512K4K;
					return true;
				}
			}
		}

		if(str.length() >= 8){
			if(str.substr(0, 6) == "R5F363"){
				// Flash memory
				if(str.substr(7, 1) == "6"){
					arg_my_params.mcu_group = GROUP_M16C63_128K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "E"){
					arg_my_params.mcu_group = GROUP_M16C63_256K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "K"){
					arg_my_params.mcu_group = GROUP_M16C63_384K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "K"){
					arg_my_params.mcu_group = GROUP_M16C63_512K16K4K;
					return true;
				}
			}
		}

		if(str.length() >= 8){
			if(str.substr(0, 6) == "R5F364"){
				// Flash memory
				if(str.substr(7, 1) == "6"){
					arg_my_params.mcu_group = GROUP_M16C64_128K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "D"){
					arg_my_params.mcu_group = GROUP_M16C64_256K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "M"){
					arg_my_params.mcu_group = GROUP_M16C64_512K16K4K;
					return true;
				}
			}
		}
		
		if(str.length() >= 8){
			if(str.substr(0, 6) == "R5F365"){
				// Flash memory
				if(str.substr(7, 1) == "6"){
					arg_my_params.mcu_group = GROUP_M16C65_128K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "E"){
					arg_my_params.mcu_group = GROUP_M16C65_256K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "K"){
					arg_my_params.mcu_group = GROUP_M16C65_384K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "M"){
					arg_my_params.mcu_group = GROUP_M16C65_512K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "N"){
					arg_my_params.mcu_group = GROUP_M16C65_512K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "R"){
					arg_my_params.mcu_group = GROUP_M16C65_640K16K4K;
					return true;
				}
				if(str.substr(7, 1) == "T"){
					arg_my_params.mcu_group = GROUP_M16C65_768K16K4K;
					return true;
				}
			}
		}

		if(str.length() >= 8){
			if(str.substr(0, 5) == "M3080"){
				// Mask ROM
				if(str.substr(6, 2) == "MC"){
					arg_my_params.mcu_group = GROUP_M16C80_128K;
					return true;
				}
				if(str.substr(6, 2) == "MG"){
					arg_my_params.mcu_group = GROUP_M16C80_256K;
					return true;
				}
				// Flash memory
				if(str.substr(6, 2) == "FC"){
					arg_my_params.mcu_group = GROUP_M16C80_128K;
					return true;
				}
				if(str.substr(6, 2) == "FG"){
					arg_my_params.mcu_group = GROUP_M16C80_256K;
					return true;
				}
			}
		}
	}
	if(parse_param_str(arg_cmdl_param, "mcu_group=", str)){
		if(str == "m16c26_24k"){
			arg_my_params.mcu_group = GROUP_M16C26_24K;
			return true;
		}
		if(str == "m16c26_48k"){
			arg_my_params.mcu_group = GROUP_M16C26_48K;
			return true;
		}
		if(str == "m16c26_64k"){
			arg_my_params.mcu_group = GROUP_M16C26_64K;
			return true;
		}
		if(str == "m16c26_24k4k"){
			arg_my_params.mcu_group = GROUP_M16C26_24K4K;
			return true;
		}
		if(str == "m16c26_48k4k"){
			arg_my_params.mcu_group = GROUP_M16C26_48K4K;
			return true;
		}
		if(str == "m16c26_64k4k"){
			arg_my_params.mcu_group = GROUP_M16C26_64K4K;
			return true;
		}

		if(str == "m16c28_48k"){
			arg_my_params.mcu_group = GROUP_M16C28_48K;
			return true;
		}
		if(str == "m16c28_64k"){
			arg_my_params.mcu_group = GROUP_M16C28_64K;
			return true;
		}
		if(str == "m16c28_96k"){
			arg_my_params.mcu_group = GROUP_M16C28_96K;
			return true;
		}
		if(str == "m16c28_128k"){
			arg_my_params.mcu_group = GROUP_M16C28_128K;
			return true;
		}
		if(str == "m16c28_48k4k"){
			arg_my_params.mcu_group = GROUP_M16C28_48K4K;
			return true;
		}
		if(str == "m16c28_64k4k"){
			arg_my_params.mcu_group = GROUP_M16C28_64K4K;
			return true;
		}
		if(str == "m16c28_96k4k"){
			arg_my_params.mcu_group = GROUP_M16C28_96K4K;
			return true;
		}
		if(str == "m16c28_128k4k"){
			arg_my_params.mcu_group = GROUP_M16C28_128K4K;
			return true;
		}
		
		if(str == "m16c29_64k"){
			arg_my_params.mcu_group = GROUP_M16C29_64K;
			return true;
		}
		if(str == "m16c29_96k"){
			arg_my_params.mcu_group = GROUP_M16C29_96K;
			return true;
		}
		if(str == "m16c29_128k"){
			arg_my_params.mcu_group = GROUP_M16C29_128K;
			return true;
		}
		if(str == "m16c29_64k4k"){
			arg_my_params.mcu_group = GROUP_M16C29_64K4K;
			return true;
		}
		if(str == "m16c29_96k4k"){
			arg_my_params.mcu_group = GROUP_M16C29_96K4K;
			return true;
		}
		if(str == "m16c29_128k4k"){
			arg_my_params.mcu_group = GROUP_M16C29_128K4K;
			return true;
		}
		
		if(str == "m16c30_96k"){
			arg_my_params.mcu_group = GROUP_M16C30_96K;
			return true;
		}
		if(str == "m16c30_128k"){
			arg_my_params.mcu_group = GROUP_M16C30_128K;
			return true;
		}
		if(str == "m16c30_160k"){
			arg_my_params.mcu_group = GROUP_M16C30_160K;
			return true;
		}
		if(str == "m16c30_192k"){
			arg_my_params.mcu_group = GROUP_M16C30_192K;
			return true;
		}
		if(str == "m16c30_256k"){
			arg_my_params.mcu_group = GROUP_M16C30_256K;
			return true;
		}
		if(str == "m16c30_96k4k"){
			arg_my_params.mcu_group = GROUP_M16C30_96K4K;
			return true;
		}
		if(str == "m16c30_128k4k"){
			arg_my_params.mcu_group = GROUP_M16C30_128K4K;
			return true;
		}
		if(str == "m16c30_160k4k"){
			arg_my_params.mcu_group = GROUP_M16C30_160K4K;
			return true;
		}
		if(str == "m16c30_192k4k"){
			arg_my_params.mcu_group = GROUP_M16C30_192K4K;
			return true;
		}
		if(str == "m16c30_256k4k"){
			arg_my_params.mcu_group = GROUP_M16C30_256K4K;
			return true;
		}
		
		if(str == "m16c62_48k"){
			arg_my_params.mcu_group = GROUP_M16C62_48K;
			return true;
		}
		if(str == "m16c62_64k"){
			arg_my_params.mcu_group = GROUP_M16C62_64K;
			return true;
		}
		if(str == "m16c62_96k"){
			arg_my_params.mcu_group = GROUP_M16C62_96K;
			return true;
		}
		if(str == "m16c62_128k"){
			arg_my_params.mcu_group = GROUP_M16C62_128K;
			return true;
		}
		if(str == "m16c62_192k"){
			arg_my_params.mcu_group = GROUP_M16C62_192K;
			return true;
		}
		if(str == "m16c62_256k"){
			arg_my_params.mcu_group = GROUP_M16C62_256K;
			return true;
		}
		if(str == "m16c62_320k"){
			arg_my_params.mcu_group = GROUP_M16C62_320K;
			return true;
		}
		if(str == "m16c62_384k"){
			arg_my_params.mcu_group = GROUP_M16C62_384K;
			return true;
		}
		if(str == "m16c62_512k"){
			arg_my_params.mcu_group = GROUP_M16C62_512K;
			return true;
		}
		if(str == "m16c62_48k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_48K4K;
			return true;
		}
		if(str == "m16c62_64k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_64K4K;
			return true;
		}
		if(str == "m16c62_96k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_96K4K;
			return true;
		}
		if(str == "m16c62_128k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_128K4K;
			return true;
		}
		if(str == "m16c62_192k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_192K4K;
			return true;
		}
		if(str == "m16c62_256k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_256K4K;
			return true;
		}
		if(str == "m16c62_320k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_320K4K;
			return true;
		}
		if(str == "m16c62_384k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_384K4K;
			return true;
		}
		if(str == "m16c62_512k4k"){
			arg_my_params.mcu_group = GROUP_M16C62_512K4K;
			return true;
		}
		
		if(str == "m16c63_128k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C63_128K16K4K;
			return true;
		}
		if(str == "m16c63_256k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C63_256K16K4K;
			return true;
		}
		if(str == "m16c63_384k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C63_384K16K4K;
			return true;
		}
		if(str == "m16c63_512k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C63_512K16K4K;
			return true;
		}

		if(str == "m16c64_128k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C64_128K16K4K;
			return true;
		}
		if(str == "m16c64_256k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C64_256K16K4K;
			return true;
		}
		if(str == "m16c64_512k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C64_512K16K4K;
			return true;
		}

		if(str == "m16c65_128k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_128K16K4K;
			return true;
		}
		if(str == "m16c65_256k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_256K16K4K;
			return true;
		}
		if(str == "m16c65_384k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_384K16K4K;
			return true;
		}
		if(str == "m16c65_512k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_512K16K4K;
			return true;
		}
		if(str == "m16c65_640k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_640K16K4K;
			return true;
		}
		if(str == "m16c65_768k16k4k"){
			arg_my_params.mcu_group = GROUP_M16C65_768K16K4K;
			return true;
		}

		if(str == "m16c80_128k"){
			arg_my_params.mcu_group = GROUP_M16C80_128K;
			return true;
		}
		if(str == "m16c80_256k"){
			arg_my_params.mcu_group = GROUP_M16C80_256K;
			return true;
		}
	}

	return false;
}

void parse_params(int arg_c, char *arg_v[], cl_my_params &arg_my_params){
	// Iterate to search for parameters
	for(int i = 1; i < arg_c; i++){
		parse_params_search(arg_v[i], arg_my_params);
	}
}
