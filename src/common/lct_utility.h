#ifndef __LCT_UTILITY_H__
#define __LCT_UTILITY_H__

#include <arpa/inet.h>
#include <ifaddrs.h>
#include <cstring>
#include <mutex>
#include "lct_common_define.h"

#define ntohll(x) (((lct_uint64_t)(ntohl((lct_int32_t)((x<<32)>>32)))<<32)|(lct_uint32_t)ntohl(((lct_int32_t)(x>>32))))
#define htonll(x) ntohll(x)

lct_string_t lct_ntoa(const lct_uint32_t);

lct_error_code_t get_local_ip_address(lct_uint32_t& ip_address, const lct_string_t& eth_nm = "eth0");

template <typename T>
class lct_identify_t final{
public:
	static lct_uint32_t next();
	lct_identify_t()										= delete;
	~lct_identify_t()										= delete;
private:
	static std::mutex	m_mutex;
	static lct_uint32_t m_lct_auto_incre_id;
};

#include "lct_utility_impl.hpp"

#endif



