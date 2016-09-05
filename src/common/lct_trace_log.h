#ifndef __LCT_TRACE_LOG__
#define __LCT_TRACE_LOG__

#include <iostream>
#include <thread>

#define LCT_TRACE_LOG_ERROR()		std::cout << std::this_thread::get_id() << "\t" << __FILE__ << "\t" << __LINE__ << "\t ERROR \t" << __PRETTY_FUNCTION__ << std::endl
#define LCT_TRACE_LOG_DEBUG()		std::cout << std::this_thread::get_id() << "\t" << __FILE__ << "\t" << __LINE__ << "\t DEBUG \t" << __PRETTY_FUNCTION__ << std::endl
#define LCT_TRACE_LOG_ENTER()		std::cout << std::this_thread::get_id() << "\t" << __FILE__ << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << " is entering" << std::endl
#define LCT_TRACE_LOG_EXIT()		std::cout << std::this_thread::get_id() << "\t" << __FILE__ << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << " is ended" << std::endl

#endif


