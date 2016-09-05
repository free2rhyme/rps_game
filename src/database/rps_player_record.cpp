#include "rps_player_record.h"

lct_bool_t				rps_player_record_t::m_instanced_flag		= false;
std::once_flag			rps_player_record_t::m_once_flag;
rps_player_record_t* 	rps_player_record_t::m_ptr_instance			= nullptr;

rps_player_record_t* rps_player_record_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

void rps_player_record_t::init_instance(){
	m_ptr_instance		= new rps_player_record_t();
	m_instanced_flag	= true;
}

rps_player_record_t::rps_player_record_t(){

}

rps_player_record_t::~rps_player_record_t(){
}

lct_error_code_t rps_player_record_t::get_rps_player(const rps_player_id_t& id, shp_rps_player_t& player) const{
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	rps_player_map_t::const_iterator itRe = m_rps_player_map.find(id);
	if(itRe == m_rps_player_map.end()){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	player = itRe->second;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_record_t::add_rps_player(const shp_rps_player_t& player){
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	m_rps_player_map[player->get_rps_player_id()] = player;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_record_t::del_rps_player(const rps_player_id_t& id){
	std::unique_lock<std::mutex> guard(m_game_record_mutex);
	rps_player_map_t::iterator it = m_rps_player_map.find(id);
	if(it == m_rps_player_map.end()){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_rps_player_map.erase(it);
	return lct_error_code_t::lct_error_code_successful;
}
