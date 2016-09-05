
#include "lct_event_reactor.h"
#include <thread>
#include <unistd.h>

lct_event_reactor_t::lct_event_reactor_t():m_running_flag(false){
}

lct_event_reactor_t::~lct_event_reactor_t(){
}

lct_error_code_t lct_event_reactor_t::init(const lct_bool_t kickoff_run_flag){
	if(kickoff_run_flag){
		return run();
	} else {
		return lct_error_code_t::lct_error_code_successful;
	}
}

lct_error_code_t lct_event_reactor_t::run(const lct_bool_t is_async){
	m_running_flag = true;
	if(is_async){
		return lct_event_reactor();
	} else {
		return this->lct_execute();
	}
}

lct_error_code_t lct_event_reactor_t::lct_event_reactor(){
	std::thread async_td(&lct_event_reactor_t::lct_execute, std::ref(*this));
	async_td.detach();
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_event_reactor_t::lct_execute(){
	return lct_error_code_t::lct_error_code_successful;
}


lct_error_code_t lct_event_reactor_t::shutdown(){
	m_running_flag = false;
	return lct_error_code_t::lct_error_code_successful;
}
