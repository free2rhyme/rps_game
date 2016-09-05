#include "rps_game_indentification.h"
#include "lct_utility.h"

rps_player_id_t::rps_player_id_t():m_player_id(lct_identify_t<rps_player_id_t>::next()){
	std::cout << __PRETTY_FUNCTION__ << "m_player_id is " << m_player_id << std::endl;
}

rps_player_id_t::rps_player_id_t(const lct_uint32_t id):m_player_id(id){
}

rps_player_id_t::rps_player_id_t(const rps_player_id_t& that): m_player_id(that.m_player_id){
}

rps_player_id_t& rps_player_id_t::operator = (const rps_player_id_t& that){
	m_player_id = that.m_player_id;
	return *this;
}

lct_bool_t rps_player_id_t::operator == (const rps_player_id_t& that) const{
	return m_player_id == that.m_player_id;
}

rps_player_id_t::~rps_player_id_t(){
}

rps_room_id_t::rps_room_id_t():m_room_id(lct_identify_t<rps_room_id_t>::next()){
}

rps_room_id_t::rps_room_id_t(const lct_uint32_t room_id): m_room_id(room_id){
}

rps_room_id_t::rps_room_id_t(const rps_room_id_t& that):m_room_id(that.m_room_id){
}

rps_room_id_t& rps_room_id_t::operator = (const rps_room_id_t& that){
	m_room_id = that.m_room_id;
	return *this;
}

rps_room_id_t& rps_room_id_t::operator = (rps_room_id_t&& that){
	m_room_id = that.m_room_id;
	return *this;
}

lct_bool_t rps_room_id_t::operator == (const rps_room_id_t& that) const{
	return m_room_id == that.m_room_id;
}

rps_room_id_t::~rps_room_id_t(){
}

lct_uint32_t rps_room_id_t::value() const{
	return m_room_id;
}

