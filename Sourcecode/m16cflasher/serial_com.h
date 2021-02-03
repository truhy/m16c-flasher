#ifndef SERIAL_COM_H
#define SERIAL_COM_H

#include "my_types.h"
#include "tc_string.h"
#include <windows.h>
#include <string>

class cl_serial_com{
protected:
	HANDLE m_dev_handle;  //Handle to a device
	std::string m_last_err_msg;
	DCB m_dcb;  //Win32 serial com parameters
	COMMTIMEOUTS m_timeouts;
	BOOL m_is_rd_timed_out;

public:
	cl_serial_com();
	~cl_serial_com();
	std::string last_err_msg();
	size_t last_err_msg_len();
	void clear_last_err_msg();
	my_bool close_handle();
	my_bool open_handle(const TCHAR* m_arg_dev_path);
	my_bool open_by_str(std::string m_arg_dev_path);
	void clear_last_err();
	my_bool set_params(DWORD m_arg_baud_rate);
	my_bool set_timeout(DWORD m_arg_timeout_ms);
	DWORD read(void* m_arg_buf, my_uint32 m_arg_req_len);
	DWORD write(void* m_arg_buf, my_uint32 m_arg_wr_len);
};

#endif
