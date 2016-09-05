#include "rps_event_handler.h"

lct_bool_t				rps_event_handler_t::m_instanced_flag	= false;
std::once_flag			rps_event_handler_t::m_once_flag;
rps_event_handler_t* 	rps_event_handler_t::m_ptr_instance		= nullptr;

rps_event_handler_t::rps_event_handler_t(){}
rps_event_handler_t::~rps_event_handler_t(){}

void rps_event_handler_t::init_instance(){
	m_ptr_instance	= new rps_event_handler_t();
	m_instanced_flag = true;
}

rps_event_handler_t* rps_event_handler_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

lct_error_code_t rps_event_handler_t::init(){
	return lct_event_reactor_t::init();
}

lct_error_code_t rps_event_handler_t::lct_execute(){
	LCT_TRACE_LOG_DEBUG();
	while(m_running_flag){
		shp_rps_event_t rps_event_shp;
		LCT_TRACE_LOG_DEBUG();
		lct_error_code_t result_code = m_lct_event_queue.dequeue(rps_event_shp);
		LCT_TRACE_LOG_DEBUG();
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			continue;
		}
		if(do_process_event(rps_event_shp)!= lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_event_handler_t::add_event(const shp_rps_event_t& rps_event_shp){
	LCT_TRACE_LOG_DEBUG();
	return m_lct_event_queue.enqueue(rps_event_shp);
}

lct_error_code_t rps_event_handler_t::do_process_event(const shp_rps_event_t& rps_event_shp){
	LCT_TRACE_LOG_DEBUG();
	return rps_event_shp->do_process();
}

lct_error_code_t rps_event_handler_t::process_show_shape_event(const shp_rps_event_t& event_shp){
	LCT_TRACE_LOG_DEBUG();
	return event_shp->do_process();
}

lct_error_code_t rps_event_handler_t::process_log_off_event(const shp_rps_event_t& event_shp){
	return lct_error_code_t::lct_error_code_successful;
}
