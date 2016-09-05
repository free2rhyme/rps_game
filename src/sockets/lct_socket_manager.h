#ifndef __LCT_SOCKET_MANAGER_H__
#define __LCT_SOCKET_MANAGER_H__

#include <mutex>
#include <sys/epoll.h>
#include "lct_common_define.h"
#include "lct_event_reactor.h"
#include "lct_base_socket.h"
#include "lct_hash_map.h"
#include "lct_input_stream.h"

class lct_socket_manager_t: public lct_event_reactor_t{
	typedef std::mutex											lct_sockets_map_mutex_t;
	typedef lct_hash_map_t<lct_int32_t, shp_lct_base_socket_t>	lct_sockets_map_t;

public:
	lct_error_code_t	get_lct_socket_shp(const lct_int32_t, shp_lct_base_socket_t&) const;

	virtual lct_error_code_t	process_write_event(const shp_lct_base_socket_t&, const shp_lct_output_stream_t&);
	virtual lct_error_code_t	process_read_event(const lct_int32_t, const shp_lct_input_stream_t&);
	virtual lct_error_code_t	process_accept_event(const shp_lct_base_socket_t&);
	virtual lct_error_code_t	process_close_event(const lct_uint32_t);

private:
	lct_error_code_t lct_execute() override;

protected:
	lct_error_code_t init();
	lct_error_code_t add_connect_fd(const lct_int32_t, const shp_lct_base_socket_t&);
	lct_error_code_t process_socket_event(const lct_int32_t, const epoll_event&);

protected:
	lct_socket_manager_t();
	virtual ~lct_socket_manager_t();

private:
	lct_int32_t							m_epoll_fd;
	lct_sockets_map_t					m_sockets_map;
	mutable lct_sockets_map_mutex_t		m_sockets_map_mutex;
};

#endif


