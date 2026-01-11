#ifndef MY_BUF_H
#define MY_BUF_H

#include "tru_exception.h"
#include <stdlib.h>
#include <cstdint>

class cl_my_buf{
protected:
	unsigned char *buf;

public:
	cl_my_buf() :
		buf(NULL){
	}
	~cl_my_buf(){
		if(buf != NULL){
			free(buf);
			buf = NULL;
		}
	}
	unsigned char* buf_ptr(){
		return buf;
	}
	void alloc_buf(uint32_t m_arg_len){
		if(buf != NULL){
			free(buf);
			buf = NULL;
		}

		buf = (unsigned char*)malloc(m_arg_len);
		if(buf == NULL){
			errno = ENOMEM;
			throw tru_exception::get_clib_last_error(__func__, "");
		}
	}
};

#endif
