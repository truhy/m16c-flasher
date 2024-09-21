#include "tc_string.h"

#if defined(WIN32) || defined(WIN64)

tc_string string_utils_ns::str_to_tc(std::string m_arg_str){
	return tc_string(m_arg_str.begin(), m_arg_str.end());
}

std::string string_utils_ns::tc_to_str(tc_string m_arg_tc_str){
	return std::string(m_arg_tc_str.begin(), m_arg_tc_str.end());
}

//Alternative slow conversions (due to copying twice).

/*
tc_string ns_tc_string_utils::str_to_tc(std::string m_arg_str){
	tc_string m_tc_str;
	wchar_t* m_wchar_str_ptr;

	m_wchar_str_ptr = new wchar_t[m_arg_str.size()];
	mbstowcs(m_wchar_str_ptr, m_arg_str.data(), m_arg_str.size());
	m_tc_str.assign(m_wchar_str_ptr, m_arg_str.size());
	delete [] m_wchar_str_ptr;

	return m_tc_str;
}

std::string ns_tc_string_utils::tc_to_str(tc_string m_arg_tc_str){
	std::string m_str;
	char* m_str_ptr;

	m_str_ptr = new char[2 * m_arg_tc_str.size()];
	wcstombs(m_str_ptr, (wchar_t*)m_arg_tc_str.data(), 2 * m_arg_tc_str.size());
	m_str.assign(m_str_ptr, m_arg_tc_str.size());
	delete [] m_str_ptr;

	return m_str;
}
*/

#endif
