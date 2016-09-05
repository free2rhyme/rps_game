
#ifndef __RPS_MESSAGE_HANDLER_H__
#define __RPS_MESSAGE_HANDLER_H__

#include <functional>
#include "lct_common_define.h"
#include "lct_queue.h"
#include "lct_hash_map.h"
#include "lct_event_reactor.h"
#include "rps_request_message.h"

class rps_message_handler_t: public lct_event_reactor_t{
private:
	typedef std::function<lct_error_code_t(const shp_rps_request_message_t&)> rps_message_handler_func_t;
	typedef lct_hash_map_t<rps_message_type_t, rps_message_handler_func_t, rps_message_type_hash> rps_message_handler_map_t;

	typedef lct_queue_t<shp_rps_request_message_t>		rps_message_queue_t;
public:
	static rps_message_handler_t* get_instance();

public:
	lct_error_code_t add_message(const shp_rps_request_message_t&);

	lct_error_code_t init();

protected:
	virtual lct_error_code_t lct_execute() override;
	lct_error_code_t handle_raw_message(const lct_char_t*, const lct_uint32_t, const lct_uint32_t);
	lct_error_code_t do_process_message(const shp_rps_request_message_t&);

private:
	static void init_instance();

	lct_error_code_t handle_log_in_message(const shp_rps_request_message_t&);
	lct_error_code_t handle_show_shape_message(const shp_rps_request_message_t&);
	lct_error_code_t handle_log_off_message(const shp_rps_request_message_t&);
	lct_error_code_t handle_setup_new_room_message(const shp_rps_request_message_t&);
	lct_error_code_t handle_select_a_room_message(const shp_rps_request_message_t&);
	lct_error_code_t handle_be_ready_message(const shp_rps_request_message_t&);

private:
	rps_message_handler_t();
	~rps_message_handler_t();

private:
	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_message_handler_t* 	m_ptr_instance;

	rps_message_queue_t				m_lct_message_queue;
	rps_message_handler_map_t		m_rps_message_handler_map;
};

#define rps_message_handler_sgt rps_message_handler_t::get_instance()

#endif




