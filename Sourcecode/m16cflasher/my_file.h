#ifndef MY_FILE_H
#define MY_FILE_H

#include "my_types.h"
#include <stdio.h>
#include <string>

#define MAX_FILE_LINE_LEN 300

class cl_my_file{
protected:
	FILE* m_file;
	char* m_line;
	std::string m_last_err;

public:
	cl_my_file();
	~cl_my_file();
	std::string last_err_msg();
	size_t last_err_msg_len();
	my_bool open_file(std::string m_arg_rd_file_path_name, std::string m_arg_open_mode);
    my_bool read_file_line(std::string& m_arg_line);
	my_bool read_file(void* m_arg_data_buf, size_t m_arg_rd_size, size_t& m_arg_file_bytes_rd);
    my_bool write_file(const void* m_arg_data_buf, size_t m_arg_wr_size, size_t& m_arg_file_bytes_wr);
	long length();
    void close_file();
};

#endif
