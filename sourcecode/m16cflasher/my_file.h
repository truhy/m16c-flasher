#ifndef MY_FILE_H
#define MY_FILE_H

#include <stdio.h>
#include <string>

#define MAX_FILE_LINE_LEN 300

class cl_my_file{
protected:
	FILE *_file;
	char *_line;

public:
	cl_my_file();
	~cl_my_file();
	void open_file(std::string filefullpath, std::string open_mode);
	void read_file_line(std::string &line);
	void read_file(void *buf, size_t rlen, size_t &bytes_rd);
	void write_file(const void *buf, size_t wlen, size_t &bytes_wr);
	long length();
	void close_file();
	int eof();
};

#endif
