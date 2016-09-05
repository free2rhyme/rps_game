#ifndef __LCT_TCP_CONNECT_SOCKET_H__
#define __LCT_TCP_CONNECT_SOCKET_H__
#include "lct_base_socket.h"
#include "rps_request_message.h"

class lct_tcp_connect_socket_t: public lct_base_socket_t{
public:
	lct_tcp_connect_socket_t(const lct_uint32_t, const lct_uint16_t, const lct_uint32_t, const lct_uint16_t, const lct_uint32_t);
	~lct_tcp_connect_socket_t();

public:
	virtual lct_error_code_t	init() override;
	lct_error_code_t	process_read_event(lct_socket_manager_t*);
	virtual lct_error_code_t	get_peer_ip_address(lct_uint32_t&) const override;
	virtual lct_error_code_t	get_peer_port_number(lct_uint16_t&) const override;

protected:
	lct_error_code_t	CreateRecvBuffer(const lct_uint32_t max_message_size = m_recv_buffer_default_size);
	lct_error_code_t	process_read_event();
	lct_error_code_t	reset_buffer();
	lct_error_code_t	process_write_event(const shp_lct_output_stream_t&);

protected:
	lct_uint16_t				m_peer_port_number;
	lct_uint32_t				m_peer_ip_address;

	lct_uint32_t				m_recv_buffer_size;
	lct_uint32_t				m_bytes_in_recv_buffer;
	lct_char_t*					m_recv_buffer_ptr;


	static const lct_uint32_t	m_recv_buffer_default_size = 1024;
};








#endif


