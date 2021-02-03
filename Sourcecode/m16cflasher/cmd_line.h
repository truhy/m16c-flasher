#ifndef CMD_LINE_H
#define CMD_LINE_H

#include "my_types.h"
#include "m16c_cmds.h"
#include <string>

//Main commands
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
	unsigned char m_com_port_nr;
	my_uint32 m_baud_rate;
	my_uint32 m_id_addr;
	std::string m_id_hex_str;
	my_uint32 m_from_addr;
	my_uint32 m_to_addr;
	std::string m_full_file_name;
	my_bool m_is_blank_chk_before_program;
	my_bool m_is_erase_before_program;
	unsigned char m_num_erase_program_error_retry;

	cl_my_params() :
		m_cmd(CMD_NONE),
		m_com_port_nr(1),
		m_baud_rate(DEFAULT_BAUD_RATE),
		m_id_addr(0),
		m_from_addr(0),
		m_to_addr(0),
		m_is_blank_chk_before_program(FALSE),
		m_is_erase_before_program(FALSE),
		m_num_erase_program_error_retry(1){
	}
};

template<class T>
my_bool parse_param_val(std::string m_arg_param, std::string m_arg_key_word, T& m_arg_val){
    std::string m_param_val_str;

    //Len of param is correct or longer?
    if(m_arg_param.size() >= (m_arg_key_word.size() + 1)){
        //Compares param to key word.
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            m_param_val_str = m_arg_param.substr(m_arg_key_word.size(), m_arg_param.size());
            if(!m_param_val_str.empty()){
                //m_arg_val = str_to_num<T>(m_param_val_str);
                //m_arg_val = (T)strtol(m_param_val_str.c_str(), NULL, 0);
                m_arg_val = (T)atof(m_param_val_str.c_str());
            }else{
                m_arg_val = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
}

template<class T>
my_bool parse_param_val_int(std::string m_arg_param, std::string m_arg_key_word, T& m_arg_val){
    std::string m_param_val_str;

    //Len of param is correct or longer?
    if(m_arg_param.size() >= (m_arg_key_word.size() + 1)){
        //Compares param to key word.
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            m_param_val_str = m_arg_param.substr(m_arg_key_word.size(), m_arg_param.size());
            if(!m_param_val_str.empty()){
                //m_arg_val = str_to_num<T>(m_param_val_str);
                m_arg_val = (T)strtol(m_param_val_str.c_str(), NULL, 0);
                //m_arg_val = (T)atof(m_param_val_str.c_str());
            }else{
                m_arg_val = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
}

my_bool parse_param_exist(std::string m_arg_param, std::string m_arg_key_word);
my_bool parse_param_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_data_str);
my_bool parse_param_yn(std::string m_arg_param, std::string m_arg_key_word, my_bool& m_arg_yn);
my_bool parse_param_hex_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_str);
void usage(char* m_arg_0);
my_bool parse_params_search(char* m_arg_cmdl_param, cl_my_params& m_arg_my_params);
void parse_params(int m_arg_c, char* m_arg_v[], cl_my_params& m_arg_my_params);

#endif
