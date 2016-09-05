#include <sys/stat.h>
#include <unistd.h>
#include "rps_event_log.h"
#include "lct_timer.h"

static const lct_string_t RPS_LOGGER_FOLDER("../log/");
static const lct_string_t RPS_LOGGER_FILE_NAME("rps_game_event.log");
static const lct_string_t RPS_LOGGER_FILE_PATH(RPS_LOGGER_FOLDER+RPS_LOGGER_FILE_NAME);
static const lct_uint32_t interval_to_flush = 1000;

lct_bool_t				rps_event_log_t::m_instanced_flag	= false;
std::once_flag			rps_event_log_t::m_once_flag;
rps_event_log_t* 		rps_event_log_t::m_ptr_instance		= nullptr;

rps_event_log_t::rps_event_log_t():m_lct_file_ptr(nullptr){}
rps_event_log_t::~rps_event_log_t(){
	if(m_lct_file_ptr){
		m_lct_file_ptr->close();
		delete m_lct_file_ptr;
		m_lct_file_ptr = nullptr;
	}
}

void rps_event_log_t::init_instance(){
	m_ptr_instance	= new rps_event_log_t();
	if(m_ptr_instance == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	m_instanced_flag = true;
}

rps_event_log_t* rps_event_log_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

void rps_event_log_t::flush_timer(const lct_uint32_t milliseonds){
	if(m_lct_file_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return;
	}
	m_lct_file_ptr->save();
	lct_timer_t timer(interval_to_flush, true , &rps_event_log_t::flush_timer, this, interval_to_flush);
	return;
}

lct_error_code_t rps_event_log_t::init(){
	LCT_TRACE_LOG_DEBUG();
	m_lct_file_ptr = new lct_file_helper_t();
	if(m_lct_file_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	if (access(RPS_LOGGER_FOLDER.data(), F_OK) < 0){
		if (mkdir(RPS_LOGGER_FOLDER.data(), 0755) < 0){
			LCT_TRACE_LOG_ERROR();
			return lct_error_code_t::lct_error_code_general_failure;
		}
	}
	lct_error_code_t result_code = m_lct_file_ptr->open(RPS_LOGGER_FILE_PATH);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();

	lct_timer_t timer(interval_to_flush, true , &rps_event_log_t::flush_timer, this, interval_to_flush);

	return lct_event_reactor_t::init(true);
}

lct_error_code_t rps_event_log_t::lct_execute(){
	LCT_TRACE_LOG_DEBUG();
	while(m_running_flag){
		shp_rps_log_block_t rps_log_block_shp;
		LCT_TRACE_LOG_DEBUG();
		lct_error_code_t result_code = m_rps_log_queue.dequeue(rps_log_block_shp);
		LCT_TRACE_LOG_DEBUG();
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			continue;
		}
		LCT_TRACE_LOG_DEBUG();
		result_code = m_lct_file_ptr->write_line(rps_log_block_shp->data());
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_event_log_t::log_event(const lct_string_t& log_info){
	LCT_TRACE_LOG_DEBUG();
	return log_event(log_info.data());
}

lct_error_code_t rps_event_log_t::log_event(const lct_char_t* log_info){
	shp_rps_log_block_t log_block_shp(new lct_string_t(log_info));
	LCT_TRACE_LOG_DEBUG();
	return m_rps_log_queue.enqueue(log_block_shp);
}
