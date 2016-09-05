#include "rps_room_record.h"

lct_bool_t				rps_room_record_t::m_instanced_flag		= false;
std::once_flag			rps_room_record_t::m_once_flag;
rps_room_record_t* 		rps_room_record_t::m_ptr_instance		= nullptr;

rps_room_record_t* rps_room_record_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

void rps_room_record_t::init_instance(){
	m_ptr_instance		= new rps_room_record_t();
	m_instanced_flag	= true;
}

rps_room_record_t::rps_room_record_t(){

}

rps_room_record_t::~rps_room_record_t(){
}

lct_error_code_t rps_room_record_t::get_rps_room(const rps_room_id_t& id, shp_rps_room_t& room) const{
	LCT_TRACE_LOG_DEBUG();
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	rps_room_map_t::const_iterator itRe = m_rps_room_map.find(id);
	if(itRe == m_rps_room_map.end()){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	room = itRe->second;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_room_record_t::add_rps_room(const shp_rps_room_t& room){
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	LCT_TRACE_LOG_DEBUG();
	m_rps_room_map[room->get_rps_room_id()] = room;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_room_record_t::del_rps_room(const rps_room_id_t& id){
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	LCT_TRACE_LOG_DEBUG();
	rps_room_map_t::iterator it = m_rps_room_map.find(id);
	if(it == m_rps_room_map.end()){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_rps_room_map.erase(it);
	return lct_error_code_t::lct_error_code_successful;
}


lct_error_code_t rps_room_record_t::get_available_rooms(rps_room_set_t& rps_room_set, lct_uint32_t max_size) const{
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	LCT_TRACE_LOG_DEBUG();
	for(rps_room_map_t::const_iterator itRe = m_rps_room_map.begin(); itRe != m_rps_room_map.end() && (max_size-- > 0); ++itRe){
		const shp_rps_room_t& shp_rps_room_ref = itRe->second;
		if(!shp_rps_room_ref->is_room_full()){
			rps_room_set.insert(shp_rps_room_ref);
			LCT_TRACE_LOG_DEBUG();
		} else {
			LCT_TRACE_LOG_DEBUG();
		}
	}
	std::cout << __PRETTY_FUNCTION__ << "\t rps_room_set.size() is " << rps_room_set.size() << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}
