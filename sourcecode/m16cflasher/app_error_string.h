#ifndef APP_ERROR_STRING_H
#define APP_ERROR_STRING_H

#include "tru_macro.h"
#include <cstdint>

// Error message list
#define APP_ERROR_LIST(item) \
	item(APP_ERROR_NONE_ID,    "") \
	item(APP_ERROR_TX_FAIL_ID, "Transmit failed") \
	item(APP_ERROR_RX_FAIL_ID, "Received failed") \
	item(APP_ERROR_XFER_INFO_ID, "Requested {} byte(s) but {} transferred") \
	item(APP_ERROR_ECHO_ID, "Echo failed") \
	item(APP_ERROR_ECHO_INFO_ID, "Transmitted 0x{:02x} but received 0x{:02x}")

// Create enum from error message list
CREATE_ENUM(app_error_e, APP_ERROR_LIST)

class app_error_string{
public:
	INIT_INLINE_CLASS_ARRAY_ENUM(static constexpr char const *, messages, APP_ERROR_LIST)
};

#endif
