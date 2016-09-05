#include "lct_utility.h"
#include "rps_event_handler.h"
#include "rps_message_handler.h"
#include "rps_socket_manager.h"
#include "rps_event_log.h"

int main(int argc, char* argv[]){

	lct_uint32_t local_server_ip_address = 0;
	lct_uint16_t local_server_port		 = 8088;
	if(argc <= 1){
		if(get_local_ip_address(local_server_ip_address) != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return -1;
		}
	} else if(argc == 2){
		if(get_local_ip_address(local_server_ip_address, lct_string_t(argv[2])) != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return -1;
		}
	} else {
		local_server_ip_address	= atol(argv[1]);
		local_server_port		= (lct_uint16_t)atoi(argv[2]);
	}
	std::cout << local_server_ip_address <<":"<<local_server_port<< std::endl;

	LCT_TRACE_LOG_DEBUG();

	lct_error_code_t result_code = rps_event_logger->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}
	LCT_TRACE_LOG_DEBUG();

	result_code = rps_event_handler_sgt->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}
	result_code = rps_event_handler_sgt->run();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}

	result_code = rps_message_handler_sgt->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}
	result_code = rps_message_handler_sgt->run();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}

	result_code = rps_socket_manager_sgt->init(local_server_port, local_server_ip_address);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}

	result_code = rps_socket_manager_sgt->run(false);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return -1;
	}


	return 0;
}






