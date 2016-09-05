
#ifndef __LCT_ERROR_CODE_H__
#define __LCT_ERROR_CODE_H__

#include "lct_common_define.h"

enum class lct_error_code_t:lct_uint8_t{
	lct_error_code_successful,
	lct_error_code_general_failure
};

#define LINUX_SYSTEM_CALL_RETURN_SUCCESS 0

#endif





