#include "rps_response_message.h"
#include <cstring>

rps_response_message_t::rps_response_message_t(const lct_uint16_t msg_id, const rps_message_type_t msg_type, const lct_uint32_t player_id):
	rps_message_t(msg_id, msg_type, player_id){
}

lct_error_code_t rps_response_message_t::init(){
	m_message_body_size		= get_message_body_size();
	m_lct_output_stream_shp	= shp_lct_output_stream_t(new lct_output_stream_t(get_message_size()));
}
rps_response_message_t::~rps_response_message_t(){
}

lct_uint32_t rps_response_message_t::get_message_body_size() const{
	return rps_message_t::get_message_body_size();
}

shp_lct_output_stream_t rps_response_message_t::get_output_stream_shp() const{
	return m_lct_output_stream_shp;
}

lct_error_code_t rps_response_message_t::build_stream(){
	lct_error_code_t result_code = m_lct_output_stream_shp->write(m_rps_message_header.get_message_id());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = m_lct_output_stream_shp->write(m_message_body_size);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = m_lct_output_stream_shp->write(m_player_id);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	return result_code;
}

rps_game_login_response_message_t::rps_game_login_response_message_t(
		const lct_uint32_t player_id, const rps_room_set_t& available_rooms):
	rps_response_message_t(RPS_MESSAGE_MASK_LOG_IN|RPS_MESSAGE_MASK_RESPONSE, rps_message_type_t::rps_server_log_in_response, player_id),
	m_rooms_size(available_rooms.size()),
	m_available_rooms_ptr(new rps_game_room_struct_t[available_rooms.size()]){
	if(player_id == 0){
		LCT_TRACE_LOG_ERROR();
		exit(0);
	}

	std::cout <<__PRETTY_FUNCTION__ << " m_rooms_size is " << m_rooms_size << std::endl;
	lct_uint32_t index = 0;
	for(rps_room_set_t::const_iterator itRe = available_rooms.begin(); itRe != available_rooms.end(); ++itRe){
		const shp_rps_room_t& read_ref = *itRe;
		rps_game_room_struct_t& write_ref = m_available_rooms_ptr[index++];
		write_ref.m_room_id				= read_ref->get_rps_room_id().value();
		const lct_string_t&  name_ref	= read_ref->get_room_name();

		lct_uint32_t effective_size = name_ref.size() < RPS_ROOM_NAME_MAX_LENGTH?name_ref.size():RPS_ROOM_NAME_MAX_LENGTH;
		lct_uint32_t gap = RPS_ROOM_NAME_MAX_LENGTH - effective_size;
		memcpy(write_ref.m_room_name, name_ref.data(), effective_size);
		memset(write_ref.m_room_name + effective_size, '\0', gap);

	}
}

rps_game_login_response_message_t::~rps_game_login_response_message_t(){
}

lct_error_code_t rps_game_login_response_message_t::build_stream(){
	lct_error_code_t result_code = rps_response_message_t::build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		return result_code;
	}

	for(lct_uint32_t i = 0; i < m_rooms_size; ++i){
		const rps_game_room_struct_t& ref = m_available_rooms_ptr[i];
		result_code = m_lct_output_stream_shp->write(ref.m_room_id);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return result_code;
		}

		result_code = m_lct_output_stream_shp->write(ref.m_room_name, RPS_ROOM_NAME_MAX_LENGTH);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return result_code;
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_uint32_t rps_game_login_response_message_t::get_message_body_size() const{
	std::cout << __PRETTY_FUNCTION__ << " m_rooms_size * RPS_ROOM_NAME_MAX_LENGTH is " << m_rooms_size * RPS_ROOM_NAME_MAX_LENGTH << std::endl;
	std::cout << __PRETTY_FUNCTION__ << " rps_response_message_t::get_message_body_size() is " << rps_response_message_t::get_message_body_size() << std::endl;
	return rps_response_message_t::get_message_body_size() + m_rooms_size * (RPS_ROOM_NAME_MAX_LENGTH + sizeof(rps_game_room_struct_t::m_room_id));
}

rps_game_enter_room_response_message_t::rps_game_enter_room_response_message_t(const lct_uint32_t player_id, const lct_string_t& str):
		rps_response_message_t(RPS_MESSAGE_MASK_SELECT_ROOM|RPS_MESSAGE_MASK_RESPONSE, rps_message_type_t::rps_server_select_a_room_response, player_id),
		m_peer_name(str){
}

rps_game_enter_room_response_message_t::~rps_game_enter_room_response_message_t(){

}

lct_error_code_t rps_game_enter_room_response_message_t::build_stream(){
	lct_error_code_t result_code = rps_response_message_t::build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = m_lct_output_stream_shp->write(m_peer_name.data(), (lct_uint32_t)m_peer_name.size());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_uint32_t rps_game_enter_room_response_message_t::get_message_body_size() const{
	return rps_response_message_t::get_message_body_size() + m_peer_name.size();
}


rps_peer_ready_response_message_t::rps_peer_ready_response_message_t(const lct_uint32_t player_id):
		rps_response_message_t(RPS_MESSAGE_MASK_BE_READY|RPS_MESSAGE_MASK_RESPONSE, rps_message_type_t::rps_server_peer_be_ready_response, player_id){
}

rps_peer_ready_response_message_t::~rps_peer_ready_response_message_t(){
}


rps_game_result_response_message_t::rps_game_result_response_message_t(const lct_uint32_t player_id, const lct_uint8_t game_result,
	const lct_uint32_t wins, const lct_uint32_t ties, const lct_uint32_t losses):
			rps_response_message_t(RPS_MESSAGE_MASK_SHOW_SHAPE|RPS_MESSAGE_MASK_RESPONSE, rps_message_type_t::rps_server_show_shape_response, player_id),
			m_game_result(game_result), m_wins(wins), m_ties(ties), m_losses(losses){
}
rps_game_result_response_message_t::~rps_game_result_response_message_t(){
}

lct_error_code_t rps_game_result_response_message_t::build_stream(){
	lct_error_code_t result_code = rps_response_message_t::build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = m_lct_output_stream_shp->write(m_game_result);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = m_lct_output_stream_shp->write(m_wins);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = m_lct_output_stream_shp->write(m_ties);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = m_lct_output_stream_shp->write(m_losses);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	return lct_error_code_t::lct_error_code_successful;
}
lct_uint32_t rps_game_result_response_message_t::get_message_body_size() const{
	return (rps_response_message_t::get_message_body_size() + sizeof(lct_uint8_t) + sizeof(lct_uint32_t) * 3);
}


rps_peer_log_off_response_message_t::rps_peer_log_off_response_message_t(const lct_uint32_t player_id):
	rps_response_message_t(RPS_MESSAGE_MASK_LOG_OFF|RPS_MESSAGE_MASK_RESPONSE, rps_message_type_t::rps_server_log_off_response, player_id){
}

rps_peer_log_off_response_message_t::~rps_peer_log_off_response_message_t(){

}
