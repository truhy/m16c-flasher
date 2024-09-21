#ifndef CMD_LINE_H
#define CMD_LINE_H

#include "m16c_cmds.h"
#include <string>

// Main commands
typedef enum{
	CMD_NONE,
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
}cmd_type;

class cl_my_params{
public:
	unsigned char m_cmd;
	std::string m_dev_path;
	uint32_t m_baud_rate;
	uint32_t m_id_addr;
	std::string m_id_hex_str;
	uint32_t m_from_addr;
	uint32_t m_to_addr;
	std::string m_full_file_name;
	bool m_is_blank_chk_before_program;
	bool m_is_erase_before_program;
	unsigned char m_num_erase_program_error_retry;

	cl_my_params() :
		m_cmd(CMD_NONE),
		m_baud_rate(DEFAULT_BAUD_RATE),
		m_id_addr(0),
		m_from_addr(0),
		m_to_addr(0),
		m_is_blank_chk_before_program(false),
		m_is_erase_before_program(false),
		m_num_erase_program_error_retry(1){
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

bool parse_param_exist(std::string m_arg_param, std::string m_arg_key_word);
bool parse_param_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_data_str);
bool parse_param_yn(std::string m_arg_param, std::string m_arg_key_word, bool& m_arg_yn);
bool parse_param_hex_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_str);
void usage(char* m_arg_0);
bool parse_params_search(char* m_arg_cmdl_param, cl_my_params& m_arg_my_params);
void parse_params(int m_arg_c, char* m_arg_v[], cl_my_params& m_arg_my_params);

#endif
