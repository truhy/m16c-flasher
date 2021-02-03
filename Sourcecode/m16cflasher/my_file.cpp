#include "my_file.h"

cl_my_file::cl_my_file() :
	m_file(NULL){
	m_line = (char*)malloc(MAX_FILE_LINE_LEN);
}

cl_my_file::~cl_my_file(){
	close_file();
	if(m_line != NULL){
        free(m_line);
    }
}

std::string cl_my_file::last_err_msg(){
	return m_last_err;
}

size_t cl_my_file::last_err_msg_len(){
	return m_last_err.length();
}

my_bool cl_my_file::open_file(std::string m_arg_rd_file_path_name, std::string m_arg_open_mode){
    m_last_err.empty();
	
	close_file();

	//Open file
    m_file = fopen(m_arg_rd_file_path_name.c_str(), m_arg_open_mode.c_str());
    if(m_file == NULL){
        m_last_err = "Error: could not open file " + m_arg_rd_file_path_name;
        return FALSE;
    }

    return TRUE;
}

my_bool cl_my_file::read_file_line(std::string& m_arg_line){
	m_last_err.empty();

    m_line[0] = 0;
    fgets(m_line, 300, m_file);

    //File error?
    if(ferror(m_file)){
		m_last_err.assign(strerror(errno));
		m_last_err = "Error: " + m_last_err;
        return FALSE;
    }else{
        m_arg_line.assign(m_line);
        //Erase newline character at end of line
        if(m_arg_line.size() > 0){
            if(m_arg_line.substr(m_arg_line.size() - 1, 1).compare("\n") == 0){
                m_arg_line.erase(m_arg_line.size() - 1);
            }
        }
    }

    return TRUE;
}

my_bool cl_my_file::read_file(void* m_arg_data_buf, size_t m_arg_rd_size, size_t& m_arg_file_bytes_rd){
    m_last_err.empty();

	//Read block bytes
    m_arg_file_bytes_rd = fread(m_arg_data_buf, 1, m_arg_rd_size, m_file);

    //File error?
    if(ferror(m_file)){
        m_last_err.assign(strerror(errno));
		m_last_err = "Error: " + m_last_err;
        return FALSE;
    }else if(m_arg_file_bytes_rd != m_arg_rd_size){
        m_last_err = "Error: some bytes failed to read!";
        return FALSE;
    }

    return TRUE;
}

my_bool cl_my_file::write_file(const void* m_arg_data_buf, size_t m_arg_wr_size, size_t& m_arg_file_bytes_wr){
    m_last_err.empty();

	//Write block bytes
    m_arg_file_bytes_wr = fwrite(m_arg_data_buf, 1, m_arg_wr_size, m_file);

    //File error?
    if(ferror(m_file)){
        m_last_err.assign(strerror(errno));
		m_last_err = "Error: " + m_last_err;
        return FALSE;
    }else if(m_arg_file_bytes_wr != m_arg_wr_size){
        m_last_err = "Error: some bytes failed to write!";
        return FALSE;
    }

    return TRUE;
}

long cl_my_file::length(){
	long m_prev = ftell(m_file);
	long m_len;
	fseek(m_file, 0, SEEK_END);
	m_len = ftell(m_file);
	fseek(m_file, m_prev, SEEK_SET);

	return m_len;
}

void cl_my_file::close_file(){
	m_last_err.empty();

    if(m_file != NULL){
        fclose(m_file);
		m_file = NULL;
    }
}
