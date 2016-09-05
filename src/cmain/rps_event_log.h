#ifndef __RPS_EVENT_LOG_H__
#define __RPS_EVENT_LOG_H__

#include <sstream>
#include "lct_file_helper.h"
#include "lct_queue.h"
#include "lct_event_reactor.h"

class rps_event_log_t final: lct_event_reactor_t{
	typedef std::shared_ptr<std::string>		shp_rps_log_block_t;
	typedef lct_queue_t<shp_rps_log_block_t>	rps_log_queue_t;
public:
	static rps_event_log_t* get_instance();

	lct_error_code_t	init();

	lct_error_code_t	log_event(const lct_string_t&);
	lct_error_code_t	log_event(const lct_char_t*);

	template <typename T, typename... Args>
	lct_error_code_t	log_event(const T& t, Args... args);

private:
	template <typename T>
	lct_error_code_t	log_event(std::stringstream& ss, const T& t);

	template <typename T, typename... Args>
	lct_error_code_t	log_event(std::stringstream& ss, const T& t, Args... args);

	void flush_timer(const lct_uint32_t);

protected:
	virtual lct_error_code_t lct_execute() override;

private:
	static void init_instance();

private:
	rps_event_log_t();
	~rps_event_log_t();

private:
	lct_file_helper_t*	m_lct_file_ptr;
	rps_log_queue_t		m_rps_log_queue;

	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_event_log_t* 		m_ptr_instance;
};

#define rps_event_logger rps_event_log_t::get_instance()

template <typename T>
lct_error_code_t rps_event_log_t::log_event(std::stringstream& ss, const T& t){
	ss << t;
	return lct_error_code_t::lct_error_code_successful;
}

template <typename T, typename... Args>
lct_error_code_t rps_event_log_t::log_event(std::stringstream& ss, const T& t, Args... args){
	ss << t;
	return log_event(ss, args...);
}

template <typename T, typename... Args>
lct_error_code_t rps_event_log_t::log_event(const T& t, Args... args){
	std::stringstream ss;

	log_event(ss, t, args...);

	log_event(ss.str());

	return lct_error_code_t::lct_error_code_successful;
}


#endif




