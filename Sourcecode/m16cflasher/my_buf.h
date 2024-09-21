#ifndef MY_BUF_H
#define MY_BUF_H

#include "tru_exception.h"
#include <stdlib.h>
#include <cstdint>

class cl_my_buf{
protected:
	unsigned char* m_buf;

public:
	cl_my_buf() :
		m_buf(NULL){
	}
	~cl_my_buf(){
		if(m_buf != NULL){
			free(m_buf);
			m_buf = NULL;
		}
	}
	unsigned char* buf(){
		return m_buf;
	}
	void alloc_buf(uint32_t m_arg_len){
		if(m_buf != NULL){
			free(m_buf);
			m_buf = NULL;
		}

		m_buf = (unsigned char*)malloc(m_arg_len);
		if(m_buf == NULL){
			errno = ENOMEM;
			throw tru_exception::get_clib_last_error(__func__, "");
		}
	}
};

#endif
