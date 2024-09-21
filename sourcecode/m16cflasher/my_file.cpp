#include "my_file.h"
#include "tru_exception.h"

cl_my_file::cl_my_file() :
	_file(NULL){
	_line = (char*)malloc(MAX_FILE_LINE_LEN);
}

cl_my_file::~cl_my_file(){
	close_file();
	if(_line != NULL){
		free(_line);
	}
}

void cl_my_file::open_file(std::string filefullpath, std::string open_mode){
	close_file();

	// Open file
#if defined(WIN32) || defined(WIN64)
	if(fopen_s(&_file, filefullpath.c_str(), open_mode.c_str()) != 0) throw tru_exception::get_clib_last_error(__func__, filefullpath);
#else
	_file = fopen(filefullpath.c_str(), open_mode.c_str());
	if(_file == NULL) throw tru_exception::get_clib_last_error(__func__, filefullpath);
#endif
}

void cl_my_file::read_file_line(std::string &line){
	_line[0] = 0;
	if(fgets(_line, 300, _file) == NULL){
		// File error?
		if(ferror(_file)){
			throw tru_exception::get_clib_last_error(__func__, "");
		}
	}else{
		line.assign(_line);
		// Erase newline character at end of line
		if(line.size() > 0){
			if(line.substr(line.size() - 1, 1).compare("\n") == 0){
				line.erase(line.size() - 1);
			}
		}
	}
}

void cl_my_file::read_file(void *buf, size_t rlen, size_t &bytes_rd){
	// Read block bytes
	bytes_rd = fread(buf, 1, rlen, _file);

	// File error?
	if(ferror(_file)){
		throw tru_exception::get_clib_last_error(__func__, "");
	}
}

void cl_my_file::write_file(const void *buf, size_t wlen, size_t &bytes_wr){
	// Write block bytes
	bytes_wr = fwrite(buf, 1, wlen, _file);

	// File error?
	if(ferror(_file)){
		throw tru_exception::get_clib_last_error(__func__, "");
	}
}

long cl_my_file::length(){
	long prev = ftell(_file);
	long len;
	fseek(_file, 0, SEEK_END);
	len = ftell(_file);
	fseek(_file, prev, SEEK_SET);

	return len;
}

void cl_my_file::close_file(){
	if(_file != NULL){
		fclose(_file);
		_file = NULL;
	}
}

int cl_my_file::eof(){
	return feof(_file);
}
