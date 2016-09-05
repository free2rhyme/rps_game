
#include "rps_socket_manager.h"
#include "lct_tcp_listening_socket.h"
#include <arpa/inet.h>
#include "rps_message_handler.h"
#include "rps_room_record.h"
#include "rps_player_record.h"
#include "rps_event_handler.h"
#include "rps_event_log.h"
#include "lct_utility.h"

lct_bool_t				rps_socket_manager_t::m_instanced_flag	= false;
std::once_flag			rps_socket_manager_t::m_once_flag;
rps_socket_manager_t* 	rps_socket_manager_t::m_ptr_instance	= nullptr;

rps_socket_manager_t::rps_socket_manager_t(){
}

rps_socket_manager_t::~rps_socket_manager_t(){
}

rps_socket_manager_t* rps_socket_manager_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
		m_instanced_flag = true;
	}
	return m_ptr_instance;
}

void rps_socket_manager_t::init_instance(){
	m_ptr_instance	= new rps_socket_manager_t();
}

lct_error_code_t rps_socket_manager_t::init(const lct_uint16_t local_port_number, const lct_uint32_t local_ip_address){

	init_message_register();

	lct_error_code_t result_code = lct_socket_manager_t::init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_lct_tcp_listening_socket_shp.reset(new lct_tcp_listening_socket_t(local_port_number, local_ip_address));

	result_code = m_lct_tcp_listening_socket_shp->init();

	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	result_code = add_connect_fd(m_lct_tcp_listening_socket_shp->get_socket_id(), m_lct_tcp_listening_socket_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	rps_event_logger->log_event(lct_ntoa(local_ip_address), " : ", local_port_number, " is listening ");

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_socket_manager_t::init_message_register(){
	m_rps_message_hash_map[RPS_MESSAGE_MASK_REQUEST | RPS_MESSAGE_MASK_LOG_IN]			= rps_message_login_request_t::new_instance;
	m_rps_message_hash_map[RPS_MESSAGE_MASK_REQUEST | RPS_MESSAGE_MASK_SHOW_SHAPE]		= rps_message_show_shape_request_t::new_instance;
	m_rps_message_hash_map[RPS_MESSAGE_MASK_REQUEST | RPS_MESSAGE_MASK_SET_ROOM]		= rps_message_setup_new_room_request_t::new_instance;
	m_rps_message_hash_map[RPS_MESSAGE_MASK_REQUEST | RPS_MESSAGE_MASK_SELECT_ROOM]		= rps_message_select_a_room_request_t::new_instance;
	m_rps_message_hash_map[RPS_MESSAGE_MASK_REQUEST | RPS_MESSAGE_MASK_BE_READY]		= rps_message_be_ready_request_t::new_instance;

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_socket_manager_t::process_write_event(const shp_lct_base_socket_t& lct_base_socket_shp, const shp_lct_output_stream_t&){
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_socket_manager_t::process_read_event(const lct_int32_t socket_id, const shp_lct_input_stream_t& lct_input_stream_shp){
	return process_recv_buff_data(lct_input_stream_shp->data(), lct_input_stream_shp->length(), socket_id);
}

lct_error_code_t rps_socket_manager_t::process_accept_event(const shp_lct_base_socket_t& lct_base_socket_shp){
	lct_error_code_t result_code = lct_base_socket_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	result_code = add_connect_fd(lct_base_socket_shp->get_socket_id(), lct_base_socket_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	lct_uint32_t peer_ip_address;
	result_code = lct_base_socket_shp->get_peer_ip_address(peer_ip_address);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	lct_uint16_t peer_port_number;
	result_code = lct_base_socket_shp->get_peer_port_number(peer_port_number);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	rps_event_logger->log_event(lct_ntoa(peer_ip_address), " : ", peer_port_number, " is accepted ");

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_socket_manager_t::process_close_event(const lct_uint32_t player_id){

	shp_rps_player_t rps_player_shp;
	std::cout << __PRETTY_FUNCTION__ << " player_id is " << player_id << std::endl;
	lct_error_code_t result_code = rps_player_record_sgt->get_rps_player(rps_player_id_t(player_id), rps_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	shp_lct_base_socket_t lct_base_socket_shp = rps_player_shp->get_lct_socket_shp();
	rps_event_logger->log_event(lct_ntoa(lct_base_socket_shp->get_local_ip_address()), " : ", lct_base_socket_shp->get_local_port_number(), " is listening ");

	shp_rps_event_t rps_event_shp(new rps_event_logoff_t(rps_player_shp));
	return rps_event_handler_sgt->add_event(rps_event_shp);
}

lct_error_code_t rps_socket_manager_t::process_recv_buff_data(const lct_char_t* data_ptr, const lct_uint32_t length, const lct_int32_t socket_id){
	LCT_TRACE_LOG_DEBUG();
	if(data_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	const void* void_ptr = static_cast<const void*>(data_ptr);
	LCT_TRACE_LOG_DEBUG();
	if(void_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	const rps_message_header_t* msg_header_ptr = static_cast<const rps_message_header_t*>(void_ptr);
	if(msg_header_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	const lct_uint16_t msg_id 		= ntohs(msg_header_ptr->get_message_id());
	std::cout << __FILE__<< "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << "msg_id is  " << msg_id << std::endl;
	std::cout << __FILE__<< "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << "length is  " << length << std::endl;
	const lct_uint16_t type_id		= msg_id & RPS_MESSAGE_TYPE_MASK;
	const lct_uint16_t scenario_id	= msg_id & RPS_MESSAGE_SCENARIO_MASK;

	const lct_uint16_t message_key = type_id | scenario_id;
	rps_message_hash_map_t::const_iterator itRe = m_rps_message_hash_map.find(message_key);
	if(itRe == m_rps_message_hash_map.end()){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	rps_message_func_t func = itRe->second;
	rps_request_message_t* msg_ptr = func(socket_id);

	if(msg_ptr == nullptr || (msg_ptr->parse(data_ptr, length) != lct_error_code_t::lct_error_code_successful)){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	shp_rps_request_message_t rps_request_message_rps(msg_ptr);

	return rps_message_handler_sgt->add_message(rps_request_message_rps);
}
