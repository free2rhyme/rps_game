#include "rps_room.h"

rps_room_t::rps_room_t():
		m_is_destroyed_flag(false),m_rps_room_id(0), m_room_name("undefined"),
		m_host_id(0), m_visitor_id(0){
}

rps_room_t::rps_room_t(const rps_room_id_t& room_id, const lct_string_t& str_name, const rps_player_id_t& host_id):
		m_is_destroyed_flag(false),m_room_name(str_name), m_rps_room_id(room_id),
		m_host_id(host_id), m_visitor_id(0){

	if(m_room_name.length() > 24){
		m_room_name.erase(m_room_name.begin() + 24, m_room_name.end());
	}
}

rps_room_t::~rps_room_t(){
}

rps_room_id_t rps_room_t::get_rps_room_id() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	return m_rps_room_id;
}

lct_string_t rps_room_t::get_room_name() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	return m_room_name;
}

lct_bool_t rps_room_t::is_destroyed() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	return m_is_destroyed_flag;
}
lct_bool_t rps_room_t::is_room_full() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << "m_visitor_id.value() is " << m_visitor_id.value() << std::endl;
	std::cout << __PRETTY_FUNCTION__ << "m_is_destroyed_flag is " << m_is_destroyed_flag << std::endl;
	return ((m_visitor_id.value() != 0) && (m_is_destroyed_flag == false));
}

rps_player_id_t rps_room_t::get_host_player_id() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	return m_host_id;
}

rps_player_id_t rps_room_t::get_visitor_player_id() const{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	return m_visitor_id;
}
lct_error_code_t rps_room_t::update_host_info(const rps_player_id_t& player_id){
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	m_host_id = player_id;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_room_t::update_visitor_info(const rps_player_id_t& player_id){
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	LCT_TRACE_LOG_DEBUG();
	m_visitor_id = player_id;
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t rps_room_t::clear_visitor_info(){
	LCT_TRACE_LOG_DEBUG();
	return update_visitor_info(rps_player_id_t(0));
}

lct_error_code_t rps_room_t::destroy_room(){
	std::lock_guard<std::recursive_mutex> guard(m_mutex);
	m_is_destroyed_flag = true;
	clear_visitor_info();
	m_host_id = rps_player_id_t(0);
	return lct_error_code_t::lct_error_code_successful;
}

