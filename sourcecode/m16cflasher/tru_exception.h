/*
	Author        : Truong Hy
	Date started  : 24 May 2009
	Prog. language: C++

	A class for exception information.
*/

#ifndef TRU_EXCEPTION_H
#define TRU_EXCEPTION_H

#include "to_string.h"
#include <string>
#include <cstring>

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#include <errno.h>
#endif

// Error source
// RTL = Run-time library
// OS = Operating System
// VEN = Vendor
enum tru_exception_src_e{
	TRU_EXCEPT_SRC_RTL,
	TRU_EXCEPT_SRC_OS,
	TRU_EXCEPT_SRC_VEN
};

class tru_exception{
protected:
	std::string _action;          // Error string action, e.g. do_something()
	tru_exception_src_e _source;  // Error where it is defined
	size_t _code;                 // Error numeric code
	std::string _message;         // Error string message
	std::string _info;            // Error extra info

public:
	tru_exception() : _source(TRU_EXCEPT_SRC_RTL), _code(0){}
	tru_exception(std::string action, tru_exception_src_e source, size_t code, std::string message, std::string info) : _action(action), _source(source), _code(code), _message(message), _info(info){}

	virtual void set_action(std::string action){ _action = action; }
	virtual void set_number(size_t code){ _code = code; }
	virtual void set_message(std::string message){ _message = message; }
	virtual void set_info(std::string info){ _info = info; }
	virtual void set_error(std::string action, tru_exception_src_e source, size_t code, std::string message, std::string info){ _action = action; _source = source; _code = code; _message = message; _info = info; }
	virtual std::string get_action(){ return _action; }
	virtual tru_exception_src_e get_source(){ return _source; }
	virtual std::string get_source_str(tru_exception_src_e source){
		switch(source){
			case TRU_EXCEPT_SRC_RTL: return "RTL";
			case TRU_EXCEPT_SRC_OS: return "OS";
			default: return "VEN";
		}
	}
	virtual size_t get_code(){ return _code; }
	virtual std::string get_message(){ return _message; }
	virtual std::string get_error(){ return ("\"" + _action + "\", \"" + get_source_str(_source) + "\", \"0x" + string_utils_ns::to_string_right_hex(_code, 8, '0') + "\", \"" + _message + "\", \"" + _info + "\""); }
	virtual void clear(){ _action.clear(); _source = TRU_EXCEPT_SRC_RTL; _code = 0; _message.clear(); _info.clear(); }
	virtual void throw_me(){ throw *this; }
#if defined(WIN32) || defined(WIN64)
	static tru_exception get_clib_last_error(const char *caller, std::string info){
		char buf[256];
		strerror_s(buf, 256, errno);
		return tru_exception(caller, TRU_EXCEPT_SRC_RTL, errno, buf, info);
	}
	static tru_exception get_os_last_error(const char *caller, std::string info){
		DWORD err_code = GetLastError();
		LPCTSTR msg_buf = NULL;
		size_t size = FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			NULL,
			err_code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language,
			(LPTSTR)&msg_buf,
			0,
			NULL
		);
		std::string err_msg((char *)msg_buf, size);
		LocalFree((LPVOID)msg_buf);

		return tru_exception(caller, TRU_EXCEPT_SRC_OS, err_code, err_msg, info);
	}
#else
	static tru_exception get_clib_last_error(const char *caller, std::string info){
		return tru_exception(caller, TRU_EXCEPT_SRC_RTL, errno, std::strerror(errno), info);
	}
	static tru_exception get_os_last_error(const char *caller, std::string info){
		return get_clib_last_error(caller, info);
	}
#endif
};

#endif
