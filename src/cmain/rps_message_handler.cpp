#include "rps_message_handler.h"
#include "rps_event_handler.h"
#include "rps_player_record.h"
#include "rps_socket_manager.h"

lct_bool_t					rps_message_handler_t::m_instanced_flag	= false;
std::once_flag				rps_message_handler_t::m_once_flag;
rps_message_handler_t* 		rps_message_handler_t::m_ptr_instance	= nullptr;

rps_message_handler_t::rps_message_handler_t(){}
rps_message_handler_t::~rps_message_handler_t(){
}

void rps_message_handler_t::init_instance(){
	m_ptr_instance	= new rps_message_handler_t();
	m_instanced_flag = true;
}

rps_message_handler_t* rps_message_handler_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

lct_error_code_t rps_message_handler_t::init(){

	m_rps_message_handler_map[rps_message_type_t::rps_client_log_in_request]			= std::bind(&rps_message_handler_t::handle_log_in_message, this, std::placeholders::_1);
	m_rps_message_handler_map[rps_message_type_t::rps_client_show_shape_request]		= std::bind(&rps_message_handler_t::handle_show_shape_message, this, std::placeholders::_1);
	m_rps_message_handler_map[rps_message_type_t::rps_client_log_off_request]			= std::bind(&rps_message_handler_t::handle_log_off_message, this, std::placeholders::_1);
	m_rps_message_handler_map[rps_message_type_t::rps_client_setup_new_room_request]	= std::bind(&rps_message_handler_t::handle_setup_new_room_message, this, std::placeholders::_1);
	m_rps_message_handler_map[rps_message_type_t::rps_client_select_a_room_request]		= std::bind(&rps_message_handler_t::handle_select_a_room_message, this, std::placeholders::_1);
	m_rps_message_handler_map[rps_message_type_t::rps_client_be_ready_request]			= std::bind(&rps_message_handler_t::handle_be_ready_message, this, std::placeholders::_1);

	return lct_event_reactor_t::init();
}

lct_error_code_t rps_message_handler_t::lct_execute(){
	LCT_TRACE_LOG_DEBUG();
	while(m_running_flag){
		LCT_TRACE_LOG_DEBUG();
		shp_rps_request_message_t rps_message_shp;
		lct_error_code_t result_code = m_lct_message_queue.dequeue(rps_message_shp);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			continue;
		}
		if(do_process_message(rps_message_shp)!= lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_message_handler_t::add_message(const shp_rps_request_message_t& rps_message_shp){
	return m_lct_message_queue.enqueue(rps_message_shp);
}

lct_error_code_t rps_message_handler_t::do_process_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();
	rps_message_handler_map_t::const_iterator itRe = m_rps_message_handler_map.find(rps_message_shp->get_message_type());
	if(itRe == m_rps_message_handler_map.end()){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	rps_message_handler_func_t func = itRe->second;
	return func(rps_message_shp);
}

lct_error_code_t rps_message_handler_t::handle_log_in_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();

	shp_lct_base_socket_t lct_socket_shp;
	if(rps_socket_manager_sgt->get_lct_socket_shp(rps_message_shp->get_socket_id(), lct_socket_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	lct_string_t user_name;
	if(rps_message_shp->get_user_name(user_name) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	shp_rps_player_t rps_player_shp(
			new rps_player_t(rps_player_role_t::rps_play_role_undefined,
					rps_message_shp->get_player_id(), user_name, lct_socket_shp));

	std::cout << __PRETTY_FUNCTION__ << "rps_message_shp->get_player_id() is " << rps_message_shp->get_player_id().value() << std::endl;

	if( rps_player_record_sgt->add_rps_player(rps_player_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	if(lct_socket_shp->set_player_id(rps_message_shp->get_player_id().value()) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	shp_rps_event_t rps_event_shp(new rps_event_get_room_list_t(rps_player_shp));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

lct_error_code_t rps_message_handler_t::handle_show_shape_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();
	shp_rps_player_t rps_player_shp;

	std::cout << __PRETTY_FUNCTION__ << "rps_message_shp->get_player_id() is " << rps_message_shp->get_player_id().value() << std::endl;

	if(rps_player_record_sgt->get_rps_player(rps_message_shp->get_player_id(), rps_player_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	lct_uint8_t shape_value = 0;
	if(rps_message_shp->get_shape(shape_value) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	shp_rps_event_t rps_event_shp(new rps_event_show_shape_t(rps_player_shp, shape_value));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

lct_error_code_t rps_message_handler_t::handle_log_off_message(const shp_rps_request_message_t& rps_message_shp){
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_message_handler_t::handle_setup_new_room_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();
	lct_string_t room_name;
	if(rps_message_shp->get_room_name(room_name) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	shp_rps_player_t rps_player_shp;

	std::cout << __PRETTY_FUNCTION__ << "rps_message_shp->get_player_id() is " << rps_message_shp->get_player_id().value() << std::endl;

	if(rps_player_record_sgt->get_rps_player(rps_message_shp->get_player_id(), rps_player_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	shp_rps_event_t rps_event_shp(new rps_event_setup_room_t(rps_player_shp, room_name));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

lct_error_code_t rps_message_handler_t::handle_select_a_room_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();
	lct_uint32_t room_id;
	if(rps_message_shp->get_room_id(room_id) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	shp_rps_player_t rps_player_shp;

	std::cout << __PRETTY_FUNCTION__ << "rps_message_shp->get_player_id() is " << rps_message_shp->get_player_id().value() << std::endl;

	if(rps_player_record_sgt->get_rps_player(rps_message_shp->get_player_id(), rps_player_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	shp_rps_event_t rps_event_shp(new rps_event_select_room_t(rps_player_shp, room_id));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

lct_error_code_t rps_message_handler_t::handle_be_ready_message(const shp_rps_request_message_t& rps_message_shp){
	LCT_TRACE_LOG_DEBUG();
	shp_rps_player_t rps_player_shp;

	std::cout << __PRETTY_FUNCTION__ << "rps_message_shp->get_player_id() is " << rps_message_shp->get_player_id().value() << std::endl;

	if(rps_player_record_sgt->get_rps_player(rps_message_shp->get_player_id(), rps_player_shp) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	shp_rps_event_t rps_event_shp(new rps_event_be_ready_t(rps_player_shp));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

