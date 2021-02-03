#ifndef MY_BUF_H
#define MY_BUF_H

#include "my_types.h"

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
	my_bool alloc_buf(my_uint32 m_arg_len){
		if(m_buf != NULL){
			free(m_buf);
			m_buf = NULL;
		}

		m_buf = (unsigned char*)malloc(m_arg_len);
		if(m_buf == NULL){
			return FALSE;
		}
		return TRUE;
	}
};

#endif
