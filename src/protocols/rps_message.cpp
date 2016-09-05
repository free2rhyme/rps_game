#include "rps_message.h"

rps_message_t::rps_message_t(const lct_uint16_t msg_id, const rps_message_type_t msg_type, const lct_uint32_t player_id):
	m_rps_message_header(msg_id),
	m_message_type(msg_type),
	m_player_id(player_id),
	m_message_body_size(0){
}

rps_message_t::~rps_message_t(){
}

lct_uint32_t rps_message_t::get_message_header_size() const{
	return sizeof(m_rps_message_header);
}

lct_uint32_t rps_message_t::get_message_body_size() const{
	return  sizeof(m_message_body_size) + sizeof(m_player_id);
}

lct_uint32_t rps_message_t::get_message_size() const{
	return get_message_header_size() + get_message_body_size();
}

rps_message_type_t rps_message_t::get_message_type() const{
	return m_message_type;
}

rps_player_id_t rps_message_t::get_player_id() const{
	return rps_player_id_t(m_player_id);
}

rps_message_header_t::rps_message_header_t(const lct_uint16_t msg_id):m_rps_message_id(msg_id){
}

rps_message_header_t::~rps_message_header_t(){
}

lct_uint16_t rps_message_header_t::get_message_id() const{
	return m_rps_message_id;
}
lct_error_code_t rps_message_header_t::set_message_id(const lct_uint16_t msg_id){
	m_rps_message_id = msg_id;
	return lct_error_code_t::lct_error_code_successful;
}
