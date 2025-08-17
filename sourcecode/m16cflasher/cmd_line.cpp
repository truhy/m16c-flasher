#include "cmd_line.h"

bool parse_param_exist(std::string param, std::string key){
	// Len of param is correct or longer?
	if(param.size() == key.size()){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			return true;
		}
	}

	return false;
}

bool parse_param_str(std::string param, std::string key, std::string &value){
	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			value = param.substr(key.size(), param.size());

			return true;
		}
	}

	return false;
}

bool parse_param_yn(std::string param, std::string key, bool &value){
	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			if(param.substr(key.size(), 1) == "y"){
				value = true;
			}else{
				value = false;
			}

			return true;
		}
	}

	return false;
}

bool parse_param_hex_str(std::string param, std::string key, std::string &value){
	char ch;
	std::string::size_type i;

	value.clear();
	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			for(i = 0; i < (param.size() - key.size()); i += 2){
				ch = (char)strtoul(param.substr(key.size() + i, 2).c_str(), NULL, 16);
				value += ch;
			}
			return true;
		}
	}
	return false;
}

void usage(char* m_arg_0){
	printf("%s v1.1 - Sept 2024\n", m_arg_0);
	printf("Usage:\n");
	printf(" %s <devparams> <cmdparams>\n", m_arg_0);
	printf("devparams:\n");
	printf("  path=<s> : serial port path\n");
	printf("  baud=<n> : valid values:\n");
	printf("              9600\n");
	printf("              19200\n");
	printf("              38400\n");
	printf("              57600\n");
	printf("              115200\n");
	printf("\n");
	printf("cmdparams:\n");
	printf("ver              : read bootloader version\n");
	printf("autobaud         : perform auto-baud sequence\n");
	printf("status           : read status\n");
	printf("id_chk           : check id\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("read             : read memory\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : from address\n");
	printf("  to_addr=<n>     : to address\n");
	printf("  file=<s>        : file\n");
	printf("blank_chk        : blank check\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : from address\n");
	printf("  to_addr=<n>     : to address\n");
	printf("erase            : erase blocks\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : from address\n");
	printf("  to_addr=<n>     : to address\n");
	printf("erase_unlocked   : erase all unlocked blocks\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("read_lock_bit    : read lock bit of a block\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : highest address of block\n");
	printf("prog_lock_bit    : program lock bit of a block\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : highest address of block\n");
	printf("lock_on          : lock bit enable\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("lock_off         : lock bit disable\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("program          : program flash memory\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : from address\n");
	printf("  to_addr=<n>     : to address\n");
	printf("  blank_chk=<y|n> : enable blank check\n");
	printf("  erase=<y|n>     : enable erase\n");
	printf("  file=<s>        : file\n");
	printf("download         : download to ram and run\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  file=<s>        : file\n");
	printf("ba_output        : boot area output\n");
	printf("  id_addr=<n>     : addr of 1st byte of id\n");
	printf("  id=<hex>        : id hex string\n");
	printf("  from_addr=<n>   : from address\n");
	printf("  to_addr=<n>     : to address\n");
	printf("  file=<s>        : file\n");
}

bool parse_params_search(char* m_arg_cmdl_param, cl_my_params& m_arg_my_params){
	if(parse_param_exist(m_arg_cmdl_param, "autobaud")){
		m_arg_my_params.m_cmd = CMD_AUTO_BAUD;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "ver")){
		m_arg_my_params.m_cmd = CMD_VER;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "status")){
		m_arg_my_params.m_cmd = CMD_STATUS;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "id_chk")){
		m_arg_my_params.m_cmd = CMD_ID_CHK;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "read")){
		m_arg_my_params.m_cmd = CMD_READ;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "blank_chk")){
		m_arg_my_params.m_cmd = CMD_BLANK_CHK;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "erase")){
		m_arg_my_params.m_cmd = CMD_ERASE;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "erase_unlocked")){
		m_arg_my_params.m_cmd = CMD_ERASE_UNLOCKED;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "read_lock_bit")){
		m_arg_my_params.m_cmd = CMD_READ_LOCK_BIT;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "prog_lock_bit")){
		m_arg_my_params.m_cmd = CMD_PROGRAM_LOCK_BIT;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "lock_on")){
		m_arg_my_params.m_cmd = CMD_LOCK_BIT_ENABLE;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "lock_off")){
		m_arg_my_params.m_cmd = CMD_LOCK_BIT_DISABLE;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "program")){
		m_arg_my_params.m_cmd = CMD_PROGRAM;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "download")){
		m_arg_my_params.m_cmd = CMD_DOWNLOAD;
		return true;
	}
	if(parse_param_exist(m_arg_cmdl_param, "ba_output")){
		m_arg_my_params.m_cmd = CMD_BOOT_AREA_OUTPUT;
		return true;
	}
	if(parse_param_str(m_arg_cmdl_param, "path=", m_arg_my_params.m_dev_path)){
		return true;
	}
	if(parse_param_val_uint(m_arg_cmdl_param, "baud=", m_arg_my_params.m_baud_rate)){
		return true;
	}
	if(parse_param_val_uint(m_arg_cmdl_param, "id_addr=", m_arg_my_params.m_id_addr)){
		return true;
	}
	if(parse_param_str(m_arg_cmdl_param, "id=", m_arg_my_params.m_id_hex_str)){
		return true;
	}
	if(parse_param_val_uint(m_arg_cmdl_param, "from_addr=", m_arg_my_params.m_from_addr)){
		return true;
	}
	if(parse_param_val_uint(m_arg_cmdl_param, "to_addr=", m_arg_my_params.m_to_addr)){
		return true;
	}
	if(parse_param_str(m_arg_cmdl_param, "file=", m_arg_my_params.m_full_file_name)){
		return true;
	}
	if(parse_param_yn(m_arg_cmdl_param, "blank_chk=", m_arg_my_params.m_is_blank_chk_before_program)){
		return true;
	}
	if(parse_param_yn(m_arg_cmdl_param, "erase=", m_arg_my_params.m_is_erase_before_program)){
		return true;
	}

	return false;
}

void parse_params(int m_arg_c, char* m_arg_v[], cl_my_params& m_arg_my_params){
	// Iterate to search for parameters
	for(int m_i = 1; m_i < m_arg_c; m_i++){
		parse_params_search(m_arg_v[m_i], m_arg_my_params);
	}
}
