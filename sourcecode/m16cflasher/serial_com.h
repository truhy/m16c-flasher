#ifndef SERIAL_COM_H
#define SERIAL_COM_H

#if defined(WIN32) || defined(WIN64)

#include "tru_macro.h"
#include "tc_string.h"
#include <windows.h>
#include <cstdint>
#include <string>

//#define USE_ASYNC_READ_WRITE
#define SERIAL_COM_LEN_T DWORD

class serial_com{
protected:
	HANDLE fd;  // Handle to a device
	DCB dcb;  // Win32 serial com parameters
	COMMTIMEOUTS timeouts;
	bool is_rd_timed_out;

public:
	serial_com();
	~serial_com();
	void close_handle();
	void open_handle(const TCHAR *path);
	void open_handle(std::string path);
	void clear_comm_error();
	void set_params(uint32_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits, bool rtscts_en);
	void set_timeout(uint32_t timeout_ms);
	DWORD read_port(void *buf, uint32_t len);
	DWORD write_port(void *buf, uint32_t len);
	void purge();
};

#else

#include "tru_macro.h"
#include <cstdint>
#include <string>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <unistd.h> // write(), read(), close()
#include <termios.h>  // Contains POSIX terminal control definitions

#define SERIAL_LEN_T ssize_t

#define NOPARITY    0
#define ODDPARITY   1
#define EVENPARITY  2
#define MARKPARITY  3
#define SPACEPARITY 4

#define ONESTOPBIT   0  // 1 stop bit
#define ONE5STOPBITS 1  // 1.5 stop bits
#define TWOSTOPBITS  2  // 2 stop bits

class serial_com{
protected:
	int fd;  // Handle to a device
	uint8_t buf[255];

public:
	serial_com();
	~serial_com();
	void close_handle();
	void open_handle(std::string path);
	void clear_comm_error();
	speed_t baud_rate_to_code(uint32_t baud_rate);
	uint8_t byte_size_to_code(uint8_t byte_size);
	void set_params(uint32_t baud_rate, uint8_t byte_size, uint8_t parity, uint8_t stop_bits, bool rtscts_en);
	void set_timeout(uint32_t timeout_ms);
	void set_wait(uint32_t min_chars);
	void purge();
	ssize_t read_port(void *buf, uint32_t len);
	ssize_t write_port(void *buf, uint32_t len);
};

#endif

// Serial comm custom error message list
#define SERIALCOMM_ERROR_LIST(item) \
	item(SERIALCOMM_ERROR_WAITABANDONED_ID, "Wait abandoned") \
	item(SERIALCOMM_ERROR_TIMEDOUT_ID, "Timed out")

// Create enum from error message list
CREATE_ENUM(serialcomm_error_e, SERIALCOMM_ERROR_LIST)

class serialcomm_error_string{
public:
	INIT_INLINE_CLASS_ARRAY_ENUM(static constexpr char const *, messages, SERIALCOMM_ERROR_LIST)
};

#endif
