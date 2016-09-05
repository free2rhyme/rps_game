#ifndef __LCT_TCP_LISTENING_SOCKET_H__
#define __LCT_TCP_LISTENING_SOCKET_H__

#include "lct_base_socket.h"

class lct_tcp_listening_socket_t:public lct_base_socket_t{
public:
	lct_tcp_listening_socket_t(const lct_uint16_t, const lct_uint32_t);
	virtual ~lct_tcp_listening_socket_t();

	virtual lct_error_code_t	init() override;
	lct_error_code_t			process_read_event(lct_socket_manager_t*);

protected:
	lct_error_code_t 	start_listen();
	lct_error_code_t	process_read_event();
};

#endif

