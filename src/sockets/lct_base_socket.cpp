#include "lct_base_socket.h"
#include <cstring>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

lct_base_socket_t::lct_base_socket_t(
		const lct_socket_type_t socket_type,
		const lct_socket_state_t socket_state,
		const lct_uint16_t local_port_number,
		const lct_uint32_t local_ip_address):
	m_socket_type(socket_type),
	m_socket_state(socket_state),
	m_local_port_number(local_port_number),
	m_local_ip_address(local_ip_address),
	m_socket_id(lct_base_socket_invalid_address),
	m_player_id(0),
	m_last_system_call_error(0){
}
lct_base_socket_t::lct_base_socket_t(
		const lct_uint32_t socket_id,
		const lct_socket_type_t socket_type,
		const lct_socket_state_t socket_state,
		const lct_uint16_t local_port_number,
		const lct_uint32_t local_ip_address):
	m_socket_type(socket_type),
	m_socket_state(socket_state),
	m_local_port_number(local_port_number),
	m_local_ip_address(local_ip_address),
	m_socket_id(socket_id),
	m_player_id(0),
	m_last_system_call_error(0){
}

lct_base_socket_t::~lct_base_socket_t(){
}

lct_error_code_t lct_base_socket_t::init(){
	return lct_error_code_t::lct_error_code_successful;
}

lct_socket_type_t lct_base_socket_t::get_socket_type() const{
	return m_socket_type;
}

lct_socket_state_t lct_base_socket_t::get_socket_state() const{
	return m_socket_state;
}

lct_int32_t lct_base_socket_t::get_socket_id() const{
	return m_socket_id;
}

lct_error_code_t lct_base_socket_t::set_player_id(const lct_uint32_t player_id){
	m_player_id = player_id;
	return lct_error_code_t::lct_error_code_successful;
}

lct_uint32_t lct_base_socket_t::get_player_id() const{
	return m_player_id;
}

lct_uint16_t lct_base_socket_t::get_local_port_number() const{
	return m_local_port_number;
}

lct_uint32_t lct_base_socket_t::get_local_ip_address() const{
	return m_local_ip_address;
}

lct_error_code_t lct_base_socket_t::get_peer_ip_address(lct_uint32_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}
lct_error_code_t lct_base_socket_t::get_peer_port_number(lct_uint16_t&) const{
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_error_code_t lct_base_socket_t::bind_socket(){

	struct sockaddr_in local_address;
	if(m_socket_id == lct_base_socket_invalid_address){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	memset(&local_address, 0, sizeof(local_address));

	local_address.sin_family		= AF_INET;
	local_address.sin_port			= htons(m_local_port_number);
	local_address.sin_addr.s_addr	= htonl(m_local_ip_address);

	lct_int32_t result = bind(m_socket_id, (struct sockaddr *)&local_address, sizeof(local_address));

	if(result != LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		m_last_system_call_error = errno;
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}else{
		return lct_error_code_t::lct_error_code_successful;
	}
}

lct_error_code_t lct_base_socket_t::set_nonblocking_mode(){
	if(m_socket_id == lct_base_socket_invalid_address){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	lct_int32_t fd_old_options = fcntl(m_socket_id, F_GETFL);
	lct_int32_t fd_new_options = fd_old_options | O_NONBLOCK;

	lct_int32_t result = fcntl(m_socket_id, F_SETFL, fd_new_options);

	if(result < LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_base_socket_t::close_socket(){
	if( 0 != close(m_socket_id)){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t lct_base_socket_t::process_read_event(lct_socket_manager_t*){
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_base_socket_t::process_write_event(const shp_lct_output_stream_t&){
	return lct_error_code_t::lct_error_code_successful;
}
