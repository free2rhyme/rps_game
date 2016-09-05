#ifndef __RPS_SOCKET_MANAGER_H___
#define __RPS_SOCKET_MANAGER_H___

#include "lct_common_define.h"
#include "lct_socket_manager.h"
#include "rps_request_message.h"

class rps_socket_manager_t: public lct_socket_manager_t{
private:
	typedef std::function<rps_request_message_t*(const lct_uint32_t)> rps_message_func_t;
	typedef lct_hash_map_t<lct_uint16_t, rps_message_func_t> rps_message_hash_map_t;

public:
	static rps_socket_manager_t* get_instance();

private:
	static void init_instance();
	rps_socket_manager_t();
	~rps_socket_manager_t();

public:
	lct_error_code_t			init(const lct_uint16_t, const lct_uint32_t);

	virtual lct_error_code_t	process_write_event(const shp_lct_base_socket_t&, const shp_lct_output_stream_t&) override;
	virtual lct_error_code_t	process_read_event(const lct_int32_t, const shp_lct_input_stream_t&) override;
	virtual lct_error_code_t	process_accept_event(const shp_lct_base_socket_t&) override;
	virtual lct_error_code_t	process_close_event(const lct_uint32_t) override;

protected:
	lct_error_code_t 	init_message_register();
	lct_error_code_t	process_recv_buff_data(const lct_char_t*, const lct_uint32_t, const lct_int32_t);

private:
	shp_lct_base_socket_t	m_lct_tcp_listening_socket_shp;

	rps_message_hash_map_t			m_rps_message_hash_map;

	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_socket_manager_t* 	m_ptr_instance;
};

#define rps_socket_manager_sgt rps_socket_manager_t::get_instance()


#endif
