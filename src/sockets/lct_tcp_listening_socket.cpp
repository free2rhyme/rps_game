#include "lct_tcp_listening_socket.h"
#include "lct_socket_manager.h"
#include "lct_tcp_connect_socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

lct_tcp_listening_socket_t::lct_tcp_listening_socket_t(const lct_uint16_t local_port_number, const lct_uint32_t local_ip_address):
	lct_base_socket_t(lct_socket_type_t::lct_socket_type_tcp_listening, lct_socket_state_t::
			lct_socket_state_initiated, local_port_number, local_ip_address){
}

lct_tcp_listening_socket_t::~lct_tcp_listening_socket_t(){
}

lct_error_code_t lct_tcp_listening_socket_t::init(){
	m_socket_id = socket(PF_INET, SOCK_STREAM, 0);
	if(m_socket_id < LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	lct_int32_t sflag = 1;
	lct_int32_t result = setsockopt(m_socket_id, SOL_SOCKET, SO_REUSEADDR, (const char *)&sflag, sizeof(sflag));

	if(result != LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		m_last_system_call_error = errno;
		LCT_TRACE_LOG_ERROR();
		return close_socket();
	}

	if(bind_socket() != lct_error_code_t::lct_error_code_successful){
		close_socket();
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	return start_listen();
}

lct_error_code_t lct_tcp_listening_socket_t::start_listen(){
	lct_int32_t result = listen(m_socket_id, SOMAXCONN);
	if(result != LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		m_last_system_call_error = errno;
		LCT_TRACE_LOG_ERROR();
		return close_socket();
	}
	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_tcp_listening_socket_t::process_read_event(lct_socket_manager_t* socket_mgr_ptr){
	if(socket_mgr_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	LCT_TRACE_LOG_DEBUG();
	struct sockaddr_in remote_address;
	bzero(&remote_address, sizeof(sockaddr_in));

	socklen_t address_len = INET_ADDRSTRLEN;
	lct_int32_t conn_fd = accept(m_socket_id, (sockaddr*)(&remote_address), &address_len);
	if(conn_fd < LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	const lct_uint16_t peer_port_number	= ntohs( remote_address.sin_port);
	const lct_uint32_t peer_ip_address	= htonl( remote_address.sin_addr.s_addr);

	shp_lct_base_socket_t fresh_socket_shp(new lct_tcp_connect_socket_t(conn_fd, m_local_port_number,m_local_ip_address, peer_port_number, peer_ip_address));
	std::cout <<__PRETTY_FUNCTION__ << "conn_fd is " << conn_fd << std::endl;
	LCT_TRACE_LOG_DEBUG();
	return socket_mgr_ptr->process_accept_event(fresh_socket_shp);
}
