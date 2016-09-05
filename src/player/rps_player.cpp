#include "rps_player.h"

rps_player_t::rps_player_t(const rps_player_role_t player_role, const shp_lct_base_socket_t socket_shp):
	m_rps_player_role(player_role),
	m_rps_game_shape(rps_game_shape_t::rps_game_shape_undefined),
	m_rps_room_id(),
	m_rps_player_id(),
	m_wins(0),
	m_ties(0),
	m_losses(0),
	m_lct_socket_shp(socket_shp),
	m_rps_player_name(""){
}

rps_player_t::rps_player_t(const rps_player_role_t player_role, const rps_player_id_t& player_id, const shp_lct_base_socket_t socket_shp):
	m_rps_player_role(player_role),
	m_rps_game_shape(rps_game_shape_t::rps_game_shape_undefined),
	m_rps_room_id(),
	m_rps_player_id(player_id),
	m_wins(0),
	m_ties(0),
	m_losses(0),
	m_lct_socket_shp(socket_shp),
	m_rps_player_name(""){
}

rps_player_t::rps_player_t(const rps_player_role_t player_role,
		const rps_player_id_t& player_id,
		const lct_string_t& player_name,
		const shp_lct_base_socket_t socket_shp):
	m_rps_player_role(player_role),
	m_rps_game_shape(rps_game_shape_t::rps_game_shape_undefined),
	m_rps_room_id(),
	m_rps_player_id(player_id),
	m_wins(0),
	m_ties(0),
	m_losses(0),
	m_lct_socket_shp(socket_shp),
	m_rps_player_name(player_name){
}

rps_player_t::rps_player_t(const rps_player_role_t player_role,
		const lct_uint32_t& player_id,
		const lct_string_t& player_name,
		const shp_lct_base_socket_t socket_shp):
	m_rps_player_role(player_role),
	m_rps_game_shape(rps_game_shape_t::rps_game_shape_undefined),
	m_rps_player_id(rps_player_id_t(player_id)),
	m_wins(0),
	m_ties(0),
	m_losses(0),
	m_lct_socket_shp(socket_shp),
	m_rps_player_name(player_name){
}

rps_player_t::~rps_player_t(){
}

rps_player_id_t rps_player_t::get_rps_player_id() const{
	return m_rps_player_id;
}

lct_string_t rps_player_t::get_rps_player_name() const{
	return m_rps_player_name;
}

rps_player_role_t rps_player_t::get_rps_player_role() const{
	return m_rps_player_role;
}

shp_lct_base_socket_t rps_player_t::get_lct_socket_shp() const{
	return m_lct_socket_shp;
}

rps_room_id_t rps_player_t::get_rps_room_id() const{
	return m_rps_room_id;
}

lct_error_code_t rps_player_t::set_rps_room_id(const rps_room_id_t& room_id){
	m_rps_room_id = room_id;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_t::set_rps_player_role(const rps_player_role_t player_role){
	m_rps_player_role = player_role;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_t::set_as_host(){
	return set_rps_player_role(rps_player_role_t::rps_play_role_host);
}

lct_error_code_t rps_player_t::set_as_visitor(){
	return set_rps_player_role(rps_player_role_t::rps_play_role_visitor);
}

rps_game_shape_t rps_player_t::get_rps_game_shape() const{
	return m_rps_game_shape;
}

lct_error_code_t rps_player_t::set_rps_game_shape(const rps_game_shape_t game_shape){
	m_rps_game_shape = game_shape;
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t rps_player_t::reset_game_shape(){
	return set_rps_game_shape(rps_game_shape_t::rps_game_shape_undefined);
}
lct_uint32_t rps_player_t::get_wins() const{
	return m_wins;
}

lct_uint32_t rps_player_t::get_ties() const{
	return m_ties;
}

lct_uint32_t rps_player_t::get_losses() const{
	return m_losses;
}

lct_error_code_t rps_player_t::win_again(){
	++m_wins;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_t::tie_again(){
	++m_ties;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t rps_player_t::lose_again(){
	++m_losses;
	return lct_error_code_t::lct_error_code_successful;
}
