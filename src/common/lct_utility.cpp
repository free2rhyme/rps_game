#include "lct_utility.h"
#include <arpa/inet.h>

lct_string_t lct_ntoa(const lct_uint32_t ip_address){
	std::string StrAddress;
	struct in_addr in;
	in.s_addr = htonl(ip_address);
	char* cpHostName = inet_ntoa(in);
	if(cpHostName == nullptr){
		return std::string("unknown");
	}
	return std::string(cpHostName);
}


lct_error_code_t get_local_ip_address(lct_uint32_t& ip_address, const lct_string_t& eth_nm){

	bool 		is_found_flag	= false;
	ifaddrs*	if_addr_structs = nullptr;
	void*		addr_ptr		= nullptr;

	if(0 != getifaddrs(&if_addr_structs)){
		return lct_error_code_t::lct_error_code_general_failure;
	}

	for(ifaddrs* ifa = if_addr_structs; ifa != nullptr; ifa = ifa->ifa_next) {
		if(ifa->ifa_addr == nullptr){
			continue;
		}
		if(ifa->ifa_addr->sa_family == AF_INET){
			addr_ptr = &(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr);

			if(strcmp(ifa->ifa_name, eth_nm.data()) == 0 && addr_ptr != nullptr){
				ip_address = ((in_addr*)addr_ptr)->s_addr;
				is_found_flag = true;

				ip_address = ntohl(ip_address);
				break;
			}

		}else if(ifa->ifa_addr->sa_family == AF_INET6) {
			/**
			addr_ptr = &(((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr);

			if(strcmp(ifa->ifa_name, eth_nm.data()) == 0 && addr_ptr != nullptr){
				ip_address = ((in6_addr*)addr_ptr)->s6_addr32;
				is_found_flag = true;
				break;
			}**/
		}
	}

	if(if_addr_structs != nullptr){
		freeifaddrs(if_addr_structs);
	}

	if(is_found_flag){
		return lct_error_code_t::lct_error_code_successful;
	}else{
		return lct_error_code_t::lct_error_code_general_failure;
	}
}


