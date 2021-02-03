#include "cmd_line.h"

my_bool parse_param_exist(std::string m_arg_param, std::string m_arg_key_word){
    //Len of param is correct or longer?
    if(m_arg_param.size() == m_arg_key_word.size()){
        //Compares param to key word.
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            return TRUE;
        }
    }

    return FALSE;
}

my_bool parse_param_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_data_str){
    //Len of param is correct or longer?
    if(m_arg_param.size() >= (m_arg_key_word.size() + 1)){
        //Compares param to key word.
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            m_arg_data_str = m_arg_param.substr(m_arg_key_word.size(), m_arg_param.size());

            return TRUE;
        }
    }

    return FALSE;
}

my_bool parse_param_yn(std::string m_arg_param, std::string m_arg_key_word, my_bool& m_arg_yn){
    //Len of param is correct or longer?
    if(m_arg_param.size() >= (m_arg_key_word.size() + 1)){
        //Compares param to key word.
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            if(m_arg_param.substr(m_arg_key_word.size(), 1) == "y"){
                m_arg_yn = TRUE;
            }else{
                m_arg_yn = FALSE;
            }

            return TRUE;
        }
    }

    return FALSE;
}

my_bool parse_param_hex_str(std::string m_arg_param, std::string m_arg_key_word, std::string& m_arg_str){
    char m_char;
    std::string::size_type m_i;

    m_arg_str.clear();
    //Len of param is correct or longer?
    if(m_arg_param.size() >= (m_arg_key_word.size() + 1)){
        //Compares param to key word
        if(m_arg_param.compare(0, m_arg_key_word.size(), m_arg_key_word) == 0){
            for(m_i = 0; m_i < (m_arg_param.size() - m_arg_key_word.size()); m_i += 2){
                m_char = (char)strtoul(m_arg_param.substr(m_arg_key_word.size() + m_i, 2).c_str(), NULL, 16);
                m_arg_str += m_char;
            }

            return TRUE;
        }
    }

    return FALSE;
}

void usage(char* m_arg_0){
    printf("%s v1.0 - Feb 2020\n", m_arg_0);
    printf("Usage:\n");
	printf(" %s <cmd> [params] <dev>\n\n", m_arg_0);
	printf("cmd:\n");
	printf("/ver              : read bootloader version\n");
	printf("/status           : read status\n");
	printf("/id_chk           : Check ID\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("/read             : read memory\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : from address\n");
	printf("  /to_addr=<n>     : to address\n");
	printf("  /file=<x>        : file\n");
	printf("/blank_chk        : blank check\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : from address\n");
	printf("  /to_addr=<n>     : to address\n");
	printf("/erase            : erase blocks\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : from address\n");
	printf("  /to_addr=<n>     : to address\n");
	printf("/erase_unlocked   : erase all unlocked blocks\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("/read_lock_bit    : read lock bit of a block\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : highest address of block\n");
	printf("/prog_lock_bit    : program lock bit of a block\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : highest address of block\n");
	printf("/lock_on          : lock bit enable\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("/lock_off         : lock bit disable\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("/program          : program flash memory\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : from address\n");
	printf("  /to_addr=<n>     : to address\n");
	printf("  /blank_chk=<y|n> : enable blank check\n");
	printf("  /erase=<y|n>     : enable erase\n");
	printf("/download         : download to ram and run\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /file=<x>        : file\n");
	printf("/ba_output        : boot area output\n");
	printf("  /id_addr=<n>     : addr of 1st byte of id\n");
	printf("  /id=<hex>        : id hex string\n");
	printf("  /from_addr=<n>   : from address\n");
	printf("  /to_addr=<n>     : to address\n");
	printf("  /file=<x>        : file\n");
	printf("dev:\n");
	printf("/com=<n> : com port number\n");
}

my_bool parse_params_search(char* m_arg_cmdl_param, cl_my_params& m_arg_my_params){
	if(parse_param_exist(m_arg_cmdl_param, "/ver")){
        m_arg_my_params.m_cmd = CMD_VER;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/status")){
        m_arg_my_params.m_cmd = CMD_STATUS;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/id_chk")){
        m_arg_my_params.m_cmd = CMD_ID_CHK;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/read")){
        m_arg_my_params.m_cmd = CMD_READ;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/blank_chk")){
        m_arg_my_params.m_cmd = CMD_BLANK_CHK;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/erase")){
        m_arg_my_params.m_cmd = CMD_ERASE;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/erase_unlocked")){
        m_arg_my_params.m_cmd = CMD_ERASE_UNLOCKED;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/read_lock_bit")){
        m_arg_my_params.m_cmd = CMD_READ_LOCK_BIT;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/prog_lock_bit")){
        m_arg_my_params.m_cmd = CMD_PROGRAM_LOCK_BIT;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/lock_on")){
        m_arg_my_params.m_cmd = CMD_LOCK_BIT_ENABLE;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/lock_off")){
        m_arg_my_params.m_cmd = CMD_LOCK_BIT_DISABLE;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/program")){
        m_arg_my_params.m_cmd = CMD_PROGRAM;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/download")){
        m_arg_my_params.m_cmd = CMD_DOWNLOAD;
        return TRUE;
    }
	if(parse_param_exist(m_arg_cmdl_param, "/ba_output")){
        m_arg_my_params.m_cmd = CMD_BOOT_AREA_OUTPUT;
        return TRUE;
    }
	if(parse_param_val_int(m_arg_cmdl_param, "/com=", m_arg_my_params.m_com_port_nr)){
        return TRUE;
    }
	if(parse_param_val_int(m_arg_cmdl_param, "/id_addr=", m_arg_my_params.m_id_addr)){
        return TRUE;
    }
	if(parse_param_str(m_arg_cmdl_param, "/id=", m_arg_my_params.m_id_hex_str)){
        return TRUE;
    }
	if(parse_param_val_int(m_arg_cmdl_param, "/from_addr=", m_arg_my_params.m_from_addr)){
        return TRUE;
    }
	if(parse_param_val_int(m_arg_cmdl_param, "/to_addr=", m_arg_my_params.m_to_addr)){
        return TRUE;
    }
	if(parse_param_str(m_arg_cmdl_param, "/file=", m_arg_my_params.m_full_file_name)){
        return TRUE;
    }
	if(parse_param_yn(m_arg_cmdl_param, "/blank_chk=", m_arg_my_params.m_is_blank_chk_before_program)){
		return TRUE;
	}
	if(parse_param_yn(m_arg_cmdl_param, "/erase=", m_arg_my_params.m_is_erase_before_program)){
		return TRUE;
	}

	return FALSE;
}

void parse_params(int m_arg_c, char* m_arg_v[], cl_my_params& m_arg_my_params){
	int m_i;

    //Iterate to search for parameters.
    for(m_i = 1; m_i < m_arg_c; m_i++){
        parse_params_search(m_arg_v[m_i], m_arg_my_params);
    }
}
