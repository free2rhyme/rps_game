#ifndef __RPS_EVENT_HANDLER_H__
#define __RPS_EVENT_HANDLER_H__

#include <memory>
#include <mutex>
#include "lct_queue.h"
#include "rps_event.h"
#include "lct_event_reactor.h"

class rps_event_handler_t: public lct_event_reactor_t{
private:
	typedef lct_queue_t<shp_rps_event_t>		rps_event_queue_t;

public:
	static rps_event_handler_t* get_instance();
	lct_error_code_t init();

protected:
	virtual lct_error_code_t lct_execute() override;

public:
	lct_error_code_t add_event(const shp_rps_event_t&);

private:
	static void init_instance();

private:
	lct_error_code_t do_process_event(const shp_rps_event_t&);
	lct_error_code_t process_log_in_event(const shp_rps_event_t&);
	lct_error_code_t process_log_off_event(const shp_rps_event_t&);
	lct_error_code_t process_setup_room_event(const shp_rps_event_t&);
	lct_error_code_t process_select_room_event(const shp_rps_event_t&);
	lct_error_code_t process_show_shape_event(const shp_rps_event_t&);
	lct_error_code_t process_beready_event(const shp_rps_event_t&);

private:
	rps_event_handler_t();
	~rps_event_handler_t();

private:
	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_event_handler_t* 	m_ptr_instance;

	rps_event_queue_t				m_lct_event_queue;
};

#define rps_event_handler_sgt rps_event_handler_t::get_instance()

#endif


