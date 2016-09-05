#include "rps_request_message.h"
#include "rps_player.h"


rps_request_message_t::rps_request_message_t(const lct_uint16_t msg_id, const rps_message_type_t msg_type, lct_int32_t socket_id):
	rps_message_t(msg_id, msg_type, 0),
	m_socket_id(socket_id){
}
lct_int32_t rps_request_message_t::get_socket_id() const{
	return m_socket_id;
}

lct_error_code_t rps_request_message_t::get_user_name(lct_string_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_error_code_t rps_request_message_t::get_room_name(lct_string_t&)const{
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_error_code_t rps_request_message_t::get_room_id(lct_uint32_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}
lct_error_code_t rps_request_message_t::get_shape(lct_uint8_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_uint32_t rps_request_message_t::get_message_body_size() const{
	return m_message_body_size;
}

lct_error_code_t rps_request_message_t::parse(const lct_char_t* data_ptr, const lct_uint32_t length){
	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(data_ptr, length));
	LCT_TRACE_LOG_DEBUG();
	return parse(lct_input_stream_shp);
}

lct_error_code_t rps_request_message_t::parse(shp_lct_input_stream_t& input_stream_shp){
	lct_uint16_t msg_id = 0;
	lct_error_code_t result_code = input_stream_shp->read(msg_id);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_rps_message_header.set_message_id(msg_id);


	result_code = input_stream_shp->read(m_message_body_size);
	std::cout << __PRETTY_FUNCTION__ << "\t input_stream_shp->size() is " << input_stream_shp->size() << std::endl;
	std::cout << __PRETTY_FUNCTION__ << "\t m_message_body_size is " << m_message_body_size << std::endl;
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	if(m_message_body_size != (input_stream_shp->size() - sizeof(msg_id))){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	result_code = input_stream_shp->read(m_player_id);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	std::cout << "msg_id is " << msg_id << std::endl;
	std::cout << "m_message_body_size is " << m_message_body_size << std::endl;
	std::cout << "m_player_id is " << m_player_id << std::endl;

	return lct_error_code_t::lct_error_code_successful;
}

rps_message_keepalive_request_t::rps_message_keepalive_request_t(const lct_int32_t socket_id):
		rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_KEEPALIVE, rps_message_type_t::rps_server_keepalive_request, socket_id){
}

rps_request_message_t*  rps_message_keepalive_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_keepalive_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}

rps_message_login_request_t::rps_message_login_request_t(const lct_int32_t socket_id):
		rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_LOG_IN,rps_message_type_t::rps_client_log_in_request, socket_id){
}

lct_error_code_t rps_message_login_request_t::get_user_name(lct_string_t& user_name) const{
	user_name = m_user_name;
	return lct_error_code_t::lct_error_code_successful;
}

rps_request_message_t*  rps_message_login_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_login_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}

lct_error_code_t rps_message_login_request_t::parse(const lct_char_t* data_ptr, const lct_uint32_t length){
	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(data_ptr, length));
	LCT_TRACE_LOG_DEBUG();
	lct_error_code_t result_code = rps_request_message_t::parse(lct_input_stream_shp);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	m_player_id = rps_player_id_t().value();
	if(m_player_id == 0){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	lct_uint32_t remaing_data_bytes = lct_input_stream_shp->length();
	char* str_buff = new char[remaing_data_bytes];
	do{
		result_code = lct_input_stream_shp->read(str_buff, remaing_data_bytes);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			break;
		}
		m_user_name = std::string(str_buff, remaing_data_bytes);
	}while(0);
	delete[] str_buff;
	return result_code;
}

rps_request_message_t*  rps_message_setup_new_room_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_setup_new_room_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}


rps_message_setup_new_room_request_t::rps_message_setup_new_room_request_t(const lct_int32_t socket_id):
		rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_SET_ROOM, rps_message_type_t::rps_client_setup_new_room_request, socket_id){
}

lct_error_code_t rps_message_setup_new_room_request_t::get_room_name(lct_string_t& room_name) const{
	room_name = m_room_name;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_message_setup_new_room_request_t::parse(const lct_char_t* data_ptr, const lct_uint32_t length){
	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(data_ptr, length));

	LCT_TRACE_LOG_DEBUG();
	lct_error_code_t result_code = rps_request_message_t::parse(lct_input_stream_shp);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	lct_uint32_t remaing_data_bytes = lct_input_stream_shp->length();
	char* str_buff = new char[remaing_data_bytes];
	do{
		result_code = lct_input_stream_shp->read(str_buff, remaing_data_bytes);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			break;
		}
		m_room_name = std::string(str_buff, remaing_data_bytes);
	}while(0);
	delete[] str_buff;
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

rps_request_message_t*  rps_message_select_a_room_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_select_a_room_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}

rps_message_select_a_room_request_t::rps_message_select_a_room_request_t(const lct_int32_t socket_id):
	rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_SELECT_ROOM, rps_message_type_t::rps_client_select_a_room_request, socket_id),
	m_room_id(0){
}

lct_error_code_t rps_message_select_a_room_request_t::parse(const lct_char_t* data_ptr, const lct_uint32_t length){
	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(data_ptr, length));

	LCT_TRACE_LOG_DEBUG();
	lct_error_code_t result_code = rps_request_message_t::parse(lct_input_stream_shp);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	result_code = lct_input_stream_shp->read(m_room_id);;
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}
lct_error_code_t rps_message_select_a_room_request_t::get_room_id(lct_uint32_t& room_id) const{
	room_id = m_room_id;
	return lct_error_code_t::lct_error_code_successful;
}

rps_request_message_t*  rps_message_be_ready_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_be_ready_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}


rps_message_be_ready_request_t::rps_message_be_ready_request_t(const lct_int32_t socket_id):
		rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_BE_READY, rps_message_type_t::rps_client_be_ready_request, socket_id){
}

rps_request_message_t*  rps_message_show_shape_request_t::new_instance(const lct_int32_t socket_id){
	rps_request_message_t* obj_ptr = new rps_message_show_shape_request_t(socket_id);
	if(obj_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
	}
	return obj_ptr;
}

rps_message_show_shape_request_t::rps_message_show_shape_request_t(const lct_int32_t socket_id):
		rps_request_message_t(RPS_MESSAGE_MASK_REQUEST|RPS_MESSAGE_MASK_SHOW_SHAPE, rps_message_type_t::rps_client_show_shape_request, socket_id),
		m_shape(0){
};

lct_error_code_t rps_message_show_shape_request_t::parse(const lct_char_t* data_ptr, const lct_uint32_t length){
	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(data_ptr, length));

	LCT_TRACE_LOG_DEBUG();
	lct_error_code_t result_code = rps_request_message_t::parse(lct_input_stream_shp);
	if( result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	result_code = lct_input_stream_shp->read(m_shape);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	return result_code;
}

lct_error_code_t rps_message_show_shape_request_t::get_shape(lct_uint8_t& shape_id) const{
	shape_id = m_shape;
	return lct_error_code_t::lct_error_code_successful;
}

