#ifndef __LCT_BASE_SOCKET_H__
#define __LCT_BASE_SOCKET_H__

#include <memory>
#include "lct_common_define.h"
#include "lct_output_stream.h"

enum class lct_socket_type_t: lct_uint8_t{
	lct_socket_type_invalid,
	lct_socket_type_udp_listening,
	lct_socket_type_tcp_listening,
	lct_socket_type_tcp_client,
	lct_socket_type_tcp_server
};

enum class lct_socket_state_t: lct_uint8_t {
	lct_socket_state_initiated,
	lct_socket_state_closed,
	lct_socket_state_listening,
	lct_socket_state_connect_pending,
	lct_socket_state_connecting,
	lct_socket_state_connect,
	lct_socket_state_udp_ready
};

class lct_socket_manager_t;

class lct_base_socket_t: lct_object_uncopyable{

public:
	lct_base_socket_t(const lct_socket_type_t,
			const lct_socket_state_t,
			const lct_uint16_t,
			const lct_uint32_t);

	lct_base_socket_t(const lct_uint32_t,
			const lct_socket_type_t,
			const lct_socket_state_t,
			const lct_uint16_t,
			const lct_uint32_t);
	virtual ~lct_base_socket_t();

public:
	virtual lct_error_code_t	init() = 0;

	virtual lct_error_code_t	process_read_event(lct_socket_manager_t*);
	virtual lct_error_code_t	process_write_event(const shp_lct_output_stream_t&);
	virtual lct_error_code_t	get_peer_ip_address(lct_uint32_t&) const;
	virtual lct_error_code_t	get_peer_port_number(lct_uint16_t&) const;

	lct_socket_type_t			get_socket_type() const;
	lct_socket_state_t			get_socket_state() const;
	lct_int32_t					get_socket_id() const;
	lct_uint16_t				get_local_port_number() const;
	lct_uint32_t				get_local_ip_address() const;
	lct_uint32_t				get_player_id() const;


	lct_error_code_t			set_player_id(const lct_uint32_t);
	lct_error_code_t			set_nonblocking_mode();
	lct_error_code_t			close_socket();
protected:
	lct_error_code_t			bind_socket();

protected:
	lct_socket_type_t		m_socket_type;
	lct_socket_state_t		m_socket_state;
	lct_uint16_t			m_local_port_number;
	lct_uint32_t			m_local_ip_address;
	lct_int32_t				m_socket_id;
	lct_uint32_t			m_player_id;
	lct_uint32_t			m_last_system_call_error;

	const static lct_uint32_t	lct_base_socket_invalid_address = 0;
};

typedef std::shared_ptr<lct_base_socket_t> shp_lct_base_socket_t;

#endif

