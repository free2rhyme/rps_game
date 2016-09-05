#include "rps_event.h"
#include "rps_response_message.h"
#include "rps_player_record.h"
#include "lct_random.h"

rps_event_t::rps_event_t(const rps_user_event_t event_type):
	m_rps_user_event(event_type){
}
rps_event_t::~rps_event_t(){
}

rps_user_event_t rps_event_t::get_event_type() const{
	return m_rps_user_event;
}

lct_error_code_t rps_event_t::get_shp_rps_player(shp_rps_player_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}

rps_event_login_t::rps_event_login_t():rps_event_t(rps_user_event_t::rps_user_event_log_in){
}

rps_base_event_t::rps_base_event_t(const rps_user_event_t event_type, const shp_rps_player_t& rps_player_shp):
	rps_event_t(event_type),
	m_shp_rps_player(rps_player_shp){
}

lct_error_code_t rps_base_event_t::get_shp_rps_player(shp_rps_player_t& rps_player_shp) const{
	rps_player_shp = m_shp_rps_player;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_base_event_t::get_peer_player_shp(shp_rps_player_t& peer_player_shp) const{
	rps_room_id_t rps_room_id = m_shp_rps_player->get_rps_room_id();

	shp_rps_room_t rps_room_shp;
	lct_error_code_t result_code = rps_room_record_sgt->get_rps_room(rps_room_id, rps_room_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	rps_player_role_t cur_role = m_shp_rps_player->get_rps_player_role();
	if(cur_role == rps_player_role_t::rps_play_role_visitor){
		result_code = rps_player_record_sgt->get_rps_player(rps_room_shp->get_host_player_id(), peer_player_shp);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return result_code;
		}
	} else if(cur_role == rps_player_role_t::rps_play_role_host){
		result_code = rps_player_record_sgt->get_rps_player(rps_room_shp->get_visitor_player_id(), peer_player_shp);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			return result_code;
		}
	} else {
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	return lct_error_code_t::lct_error_code_successful;
}

rps_event_get_room_list_t::rps_event_get_room_list_t(const shp_rps_player_t& rps_player_shp):
		rps_base_event_t(rps_user_event_t::rps_user_event_get_room_list, rps_player_shp){
}

lct_error_code_t rps_event_get_room_list_t::do_process(){
	rps_room_set_t rps_room_set;
	LCT_TRACE_LOG_DEBUG();
	lct_error_code_t result_code = rps_room_record_sgt->get_available_rooms(rps_room_set);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_shp_rps_player->get_rps_player_id().value() is " << m_shp_rps_player->get_rps_player_id().value() << std::endl;
	shp_rps_response_message_t rps_response_message_shp(
			new rps_game_login_response_message_t(m_shp_rps_player->get_rps_player_id().value(),rps_room_set));

	result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = m_shp_rps_player->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}

rps_event_setup_room_t::rps_event_setup_room_t(const shp_rps_player_t& rps_player_shp, const lct_string_t& room_nm):
		rps_base_event_t(rps_user_event_t::rps_user_event_setup_room, rps_player_shp),
		m_room_name(room_nm){
}

lct_error_code_t rps_event_setup_room_t::do_process(){
	LCT_TRACE_LOG_DEBUG();
	rps_room_id_t next_room_id;
	shp_rps_room_t rps_room_shp(new rps_room_t(next_room_id, m_room_name, m_shp_rps_player->get_rps_player_id()));
	std::cout << __PRETTY_FUNCTION__ << "socketid(" << m_shp_rps_player->get_rps_player_id().value() << ")"\
			<< " room_id(" << rps_room_shp->get_rps_room_id().value() << " ) and name( " << \
			rps_room_shp->get_room_name() << ")" << std::endl;

	lct_error_code_t result_code = m_shp_rps_player->set_as_host();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = m_shp_rps_player->set_rps_room_id(next_room_id);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = rps_room_record_sgt->add_rps_room(rps_room_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}


rps_event_select_room_t::rps_event_select_room_t(const shp_rps_player_t& rps_player_shp, const lct_uint32_t room_id):
		rps_base_event_t(rps_user_event_t::rps_user_event_select_room, rps_player_shp),
		m_room_id(room_id){
}

lct_error_code_t rps_event_select_room_t::notify_peer_client(const shp_rps_player_t& peer_player_shp) const{

	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_shp_rps_player->get_rps_player_id().value() is " << m_shp_rps_player->get_rps_player_id().value() << std::endl;
	shp_rps_response_message_t rps_response_message_shp(
			new rps_game_enter_room_response_message_t(peer_player_shp->get_rps_player_id().value(), m_shp_rps_player->get_rps_player_name()));

	std::cout << __PRETTY_FUNCTION__ << "host player_id(" << peer_player_shp->get_rps_player_id().value() << std::endl;
	std::cout << __PRETTY_FUNCTION__ << "host m_shp_rps_player->get_rps_player_name() is " << m_shp_rps_player->get_rps_player_name() << std::endl;

	lct_error_code_t result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = peer_player_shp->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

lct_error_code_t rps_event_select_room_t::notifiy_client(const shp_rps_player_t& peer_player_shp) const{
	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_shp_rps_player->get_rps_player_id().value() is " << m_shp_rps_player->get_rps_player_id().value() << std::endl;

	std::cout << __PRETTY_FUNCTION__ << "peer_player_shp->get_rps_player_name() is " << peer_player_shp->get_rps_player_name() << std::endl;
	shp_rps_response_message_t rps_response_message_shp(
			new rps_game_enter_room_response_message_t(m_shp_rps_player->get_rps_player_id().value(), peer_player_shp->get_rps_player_name()));

	lct_error_code_t result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = m_shp_rps_player->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

lct_error_code_t rps_event_select_room_t::do_process(){
	LCT_TRACE_LOG_DEBUG();
	rps_room_id_t rps_room_id(m_room_id);

	lct_error_code_t result_code = m_shp_rps_player->set_as_visitor();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = m_shp_rps_player->set_rps_room_id(rps_room_id);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_rps_room_t rps_room_shp;
	result_code = rps_room_record_sgt->get_rps_room(rps_room_id, rps_room_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_room_shp->update_visitor_info(m_shp_rps_player->get_rps_player_id());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_rps_player_t peer_player_shp;
	result_code = rps_player_record_sgt->get_rps_player(rps_room_shp->get_host_player_id(), peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = notify_peer_client(peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = notifiy_client(peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}


rps_event_be_ready_t::rps_event_be_ready_t(const shp_rps_player_t& rps_player_shp):
		rps_base_event_t(rps_user_event_t::rps_user_event_be_ready, rps_player_shp){
}

lct_error_code_t rps_event_be_ready_t::do_process(){
	LCT_TRACE_LOG_DEBUG();
	shp_rps_player_t peer_player_shp;
	lct_error_code_t result_code = get_peer_player_shp(peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	result_code = notifiy_client(peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_event_be_ready_t::notifiy_client(const shp_rps_player_t& peer_player_shp) const{
	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_shp_rps_player->get_rps_player_id().value() is " << m_shp_rps_player->get_rps_player_id().value() << std::endl;
	shp_rps_response_message_t rps_response_message_shp(
			new rps_peer_ready_response_message_t(peer_player_shp->get_rps_player_id().value()));

	std::cout << __PRETTY_FUNCTION__ << "peer_player_shp(" << peer_player_shp->get_rps_player_id().value() << std::endl;

	lct_error_code_t result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = peer_player_shp->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

rps_event_show_shape_t::rps_event_show_shape_t(const shp_rps_player_t& rps_player_shp, const lct_uint8_t shape_value):
		rps_base_event_t(rps_user_event_t::rps_user_event_show_shape, rps_player_shp),
		m_shape_value(shape_value){
}


lct_error_code_t rps_event_show_shape_t::handle_shape_value() const{

	switch(m_shape_value & RPS_GAME_MINE_OPTION_TYPE_MASK){
		case RPS_GAME_MINE_OPTION_ROCK_MASK:
			m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_rock);
			break;
		case RPS_GAME_MINE_OPTION_PAPER_MASK:
			m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_paper);
			break;
		case RPS_GAME_MINE_OPTION_SCISSORS_MASK:
			m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_scissors);
			break;
		case RPS_GAME_MINE_OPTION_RANDOM_MASK:{
			lct_random_t<> lrt(1,3);
			lct_int32_t value = lrt.random();
			if(value == 1){
				m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_rock);
			} else if(value == 2){
				m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_paper);
			} else {
				m_shp_rps_player->set_rps_game_shape(rps_game_shape_t::rps_game_shape_scissors);
			}
			break;
		}
		default:
			break;
	}

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_event_show_shape_t::notify_client(const shp_rps_player_t& player_shp, const lct_uint8_t game_result) const{
	shp_rps_response_message_t rps_response_message_shp(
				new rps_game_result_response_message_t(player_shp->get_rps_player_id().value(), game_result,
						player_shp->get_wins(), player_shp->get_ties(), player_shp->get_losses()));

	std::cout << __PRETTY_FUNCTION__ << "player_shp(" << player_shp->get_rps_player_id().value() << std::endl;

	lct_error_code_t result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = player_shp->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

lct_error_code_t rps_event_show_shape_t::game_determine(const shp_rps_player_t& peer_rps_player){
	const rps_game_shape_t peer_shape = peer_rps_player->get_rps_game_shape();
	const rps_game_shape_t mine_shape = m_shp_rps_player->get_rps_game_shape();

	lct_uint8_t peer_shape_value = static_cast<lct_uint8_t>(peer_shape);
	lct_uint8_t mine_shape_value = static_cast<lct_uint8_t>(mine_shape);

	lct_uint8_t mine_game_result = mine_shape_value | (peer_shape_value << 3);
	lct_uint8_t peer_game_result = peer_shape_value | (mine_shape_value << 3);
	if(peer_shape == mine_shape){
		peer_rps_player->tie_again();
		m_shp_rps_player->tie_again();

		mine_game_result |= RPS_GAME_RESULT_TIE_MASK;
		peer_game_result |= RPS_GAME_RESULT_TIE_MASK;
	} else if(((peer_shape_value < peer_game_result) && ((peer_game_result - peer_shape_value) <= peer_shape_value))
			|| ((peer_shape_value > peer_game_result) && ((peer_shape_value - peer_game_result) > peer_game_result ))){
		peer_rps_player->win_again();
		m_shp_rps_player->lose_again();

		mine_game_result |= RPS_GAME_RESULT_LOSE_MASK;
		peer_game_result |= RPS_GAME_RESULT_WIN_MASK;
	} else {
		m_shp_rps_player->win_again();
		peer_rps_player->lose_again();

		mine_game_result |= RPS_GAME_RESULT_WIN_MASK;
		peer_game_result |= RPS_GAME_RESULT_LOSE_MASK;
	}
	std::cout << "mine_game_result is " << (lct_uint16_t)(mine_game_result) << std::endl;
	std::cout << "peer_game_result is " << (lct_uint16_t)(peer_game_result) << std::endl;

	lct_error_code_t result_code = notify_client(m_shp_rps_player, mine_game_result);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}else{
		m_shp_rps_player->reset_game_shape();
	}
	result_code = notify_client(peer_rps_player, peer_game_result);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	} else {
		peer_rps_player->reset_game_shape();
	}

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_event_show_shape_t::do_process(){
	LCT_TRACE_LOG_DEBUG();

	handle_shape_value();

	shp_rps_player_t peer_player_shp;

	lct_error_code_t result_code = get_peer_player_shp(peer_player_shp);
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	if(peer_player_shp->get_rps_game_shape() == rps_game_shape_t::rps_game_shape_undefined){
		LCT_TRACE_LOG_DEBUG();
		return lct_error_code_t::lct_error_code_successful;
	} else{
		return game_determine(peer_player_shp);
	}

	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}

rps_event_logoff_t::rps_event_logoff_t(const shp_rps_player_t& rps_player_shp):
		rps_base_event_t(rps_user_event_t::rps_user_event_log_off, rps_player_shp){
}

lct_error_code_t rps_event_logoff_t::notify_peer_client(const shp_rps_player_t& peer_player_shp) const{

	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_shp_rps_player->get_rps_player_id().value() is " << m_shp_rps_player->get_rps_player_id().value() << std::endl;
	shp_rps_response_message_t rps_response_message_shp(
			new rps_peer_log_off_response_message_t(peer_player_shp->get_rps_player_id().value()));

	std::cout << __PRETTY_FUNCTION__ << "peer_player_shp(" << peer_player_shp->get_rps_player_id().value() << std::endl;

	lct_error_code_t result_code = rps_response_message_shp->init();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	result_code = rps_response_message_shp->build_stream();
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}

	shp_lct_base_socket_t lct_socket_shp = peer_player_shp->get_lct_socket_shp();
	result_code = lct_socket_shp->process_write_event(rps_response_message_shp->get_output_stream_shp());
	if(result_code != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return result_code;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}

lct_error_code_t rps_event_logoff_t::do_process(){
	lct_error_code_t result_code = lct_error_code_t::lct_error_code_successful;
	do{
		rps_room_id_t room_id = m_shp_rps_player->get_rps_room_id();
		shp_rps_room_t rps_room_shp;
		result_code = rps_room_record_sgt->get_rps_room(room_id, rps_room_shp);
		if(result_code != lct_error_code_t::lct_error_code_successful){
			LCT_TRACE_LOG_ERROR();
			break;
		}

		shp_rps_player_t peer_player_shp;
		result_code = get_peer_player_shp(peer_player_shp);

		if(result_code == lct_error_code_t::lct_error_code_successful){
			if(m_shp_rps_player->get_rps_player_role() == rps_player_role_t::rps_play_role_host){
				result_code = rps_room_shp->update_host_info(peer_player_shp->get_rps_player_id());
				if(result_code != lct_error_code_t::lct_error_code_successful){
					LCT_TRACE_LOG_ERROR();
				}
			}
			result_code = rps_room_shp->clear_visitor_info();
			if(result_code != lct_error_code_t::lct_error_code_successful){
				LCT_TRACE_LOG_ERROR();
			}

			result_code = notify_peer_client(peer_player_shp);
			if(result_code != lct_error_code_t::lct_error_code_successful){
				LCT_TRACE_LOG_ERROR();
				break;
			}
		} else {
			result_code = rps_room_record_sgt->del_rps_room(m_shp_rps_player->get_rps_room_id());
			if(result_code != lct_error_code_t::lct_error_code_successful){
				LCT_TRACE_LOG_ERROR();
			}
		}

	}while(0);

	if(rps_player_record_sgt->del_rps_player(m_shp_rps_player->get_rps_player_id()) != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	return result_code;
}


