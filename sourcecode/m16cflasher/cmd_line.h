#ifndef CMD_LINE_H
#define CMD_LINE_H

#include "m16c_cmds.h"
#include "m16c_mem_map.h"
#include <string>

// Main commands
typedef enum{
	CMD_NONE,
	CMD_AUTO_BAUD,
	CMD_VER,
	CMD_STATUS,
	CMD_ID_CHK,
	CMD_READ,
	CMD_BLANK_CHK,
	CMD_ERASE,
	CMD_ERASE_UNLOCKED,
	CMD_READ_LOCK_BIT,
	CMD_PROGRAM_LOCK_BIT,
	CMD_LOCK_BIT_ENABLE,
	CMD_LOCK_BIT_DISABLE,
	CMD_PROGRAM,
	CMD_DOWNLOAD,
	CMD_BOOT_AREA_OUTPUT
}cmdl_cmd_e;

class cl_my_params{
public:
	unsigned char cmd;
	std::string dev_path;
	uint32_t baud_rate;
	uint32_t id_addr;
	std::string id_hex_str;
	uint32_t from_addr;
	uint32_t to_addr;
	std::string full_file_name;
	bool is_blank_chk_before_program;
	bool is_erase_before_program;
	uint32_t num_erase_program_error_retry;
	mcu_group_e mcu_group;

	// Default values for commandline parameters
	cl_my_params() :
		cmd(CMD_NONE),
		baud_rate(DEFAULT_BAUD_RATE),
		id_addr(0),
		from_addr(0),
		to_addr(0),
		is_blank_chk_before_program(false),
		is_erase_before_program(false),
		num_erase_program_error_retry(3),
		mcu_group(GROUP_NONE){
	}
};

template<class T>
bool parse_param_val(std::string param, std::string key, T &value){
	std::string param_substr;

	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			param_substr = param.substr(key.size(), param.size());
			if(!param_substr.empty()){
				//value = str_to_num<T>(param_substr);
				//value = (T)strtol(param_substr.c_str(), NULL, 0);
				value = (T)atof(param_substr.c_str());
			}else{
				value = 0;
			}

			return true;
		}
	}

	return false;
}

template<class T>
bool parse_param_val_int(std::string param, std::string key, T &value){
	std::string param_substr;

	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			param_substr = param.substr(key.size(), param.size());
			if(!param_substr.empty()){
				//value = str_to_num<T>(param_substr);
				value = (T)strtol(param_substr.c_str(), NULL, 0);
				//value = (T)atof(param_substr.c_str());
			}else{
				value = 0;
			}

			return true;
		}
	}

	return false;
}

template<class T>
bool parse_param_val_uint(std::string param, std::string key, T &value){
	std::string param_substr;

	// Len of param is correct or longer?
	if(param.size() >= (key.size() + 1)){
		// Compares param to key word
		if(param.compare(0, key.size(), key) == 0){
			param_substr = param.substr(key.size(), param.size());
			if(!param_substr.empty()){
				//value = str_to_num<T>(param_substr);
				value = (T)strtoul(param_substr.c_str(), NULL, 0);
				//value = (T)atof(param_substr.c_str());
			}else{
				value = 0;
			}

			return true;
		}
	}

	return false;
}

bool parse_param_exist(std::string arg_param, std::string arg_key_word);
bool parse_param_str(std::string arg_param, std::string arg_key_word, std::string &arg_data_str);
bool parse_param_yn(std::string arg_param, std::string arg_key_word, bool &arg_yn);
bool parse_param_hex_str(std::string arg_param, std::string arg_key_word, std::string &arg_str);
void usage(char *arg_0);
bool parse_params_search(char *arg_cmdl_param, cl_my_params &arg_my_params);
void parse_params(int arg_c, char *arg_v[], cl_my_params &arg_my_params);

#endif
