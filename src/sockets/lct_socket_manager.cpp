
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>
#include "lct_socket_manager.h"
#include "lct_tcp_connect_socket.h"


#define EPOLL_ID_INITIAL -1

lct_socket_manager_t::lct_socket_manager_t():m_epoll_fd(EPOLL_ID_INITIAL){
}

lct_socket_manager_t::~lct_socket_manager_t(){
}

#define BACK_LOG	5

lct_error_code_t lct_socket_manager_t::init(){
	m_epoll_fd  = epoll_create(BACK_LOG);
	if(m_epoll_fd < LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	return lct_event_reactor_t::init();
}

lct_error_code_t lct_socket_manager_t::add_connect_fd(const lct_int32_t connect_fd, const shp_lct_base_socket_t& lct_socket_shp){
	epoll_event	event;
	event.data.fd		= connect_fd;
	event.events		= EPOLLIN | EPOLLET;
	if(lct_socket_shp->set_nonblocking_mode() != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	lct_int32_t result = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, connect_fd, &event);
	if(result < LINUX_SYSTEM_CALL_RETURN_SUCCESS){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	std::unique_lock<lct_sockets_map_mutex_t> guard(m_sockets_map_mutex);
	m_sockets_map[connect_fd] = lct_socket_shp;
	LCT_TRACE_LOG_DEBUG();

	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_socket_manager_t::get_lct_socket_shp(const lct_int32_t socket_id, shp_lct_base_socket_t& lct_socket_shp) const{
	std::unique_lock<lct_sockets_map_mutex_t> guard(m_sockets_map_mutex);
	lct_sockets_map_t::const_iterator itRe = m_sockets_map.find(socket_id);
	if(itRe == m_sockets_map.end()){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	lct_socket_shp = itRe->second;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_socket_manager_t::lct_execute(){
	epoll_event triggered_events[1024];
	lct_int32_t	triggered_events_max;

	while((m_running_flag) && (m_epoll_fd != EPOLL_ID_INITIAL)){
		lct_int32_t triggered_events_size = epoll_wait(m_epoll_fd, triggered_events, triggered_events_max, -1);
		LCT_TRACE_LOG_DEBUG();
		if(triggered_events_size < 0){
			LCT_TRACE_LOG_DEBUG();
			continue;
		} else if (triggered_events_size == 0){
			LCT_TRACE_LOG_DEBUG();
			continue;
		} else {
			LCT_TRACE_LOG_DEBUG();
			for(lct_uint32_t i = 0; i < triggered_events_size; i++){
				LCT_TRACE_LOG_DEBUG();
				process_socket_event(triggered_events[i].data.fd, triggered_events[i]);
			}
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_socket_manager_t::process_socket_event(const lct_int32_t socket_fd, const epoll_event& ev){
	shp_lct_base_socket_t lct_socket_shp;
	do{
		std::unique_lock<lct_sockets_map_mutex_t> guard(m_sockets_map_mutex);
		lct_sockets_map_t::const_iterator fs_socket_pair = m_sockets_map.find(socket_fd);
		if(fs_socket_pair == m_sockets_map.end()){
			LCT_TRACE_LOG_ERROR();
			return lct_error_code_t::lct_error_code_general_failure;
		}
		lct_socket_shp = fs_socket_pair->second;
	}while(0);

	if((ev.events & EPOLLIN) == true){
		return lct_socket_shp->process_read_event(this);
	}

	LCT_TRACE_LOG_ERROR();
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_error_code_t lct_socket_manager_t::process_write_event(const shp_lct_base_socket_t&, const shp_lct_output_stream_t&){
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t lct_socket_manager_t::process_read_event(const lct_int32_t, const shp_lct_input_stream_t&){
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t lct_socket_manager_t::process_accept_event(const shp_lct_base_socket_t&){
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t lct_socket_manager_t::process_close_event(const lct_uint32_t){
	return lct_error_code_t::lct_error_code_successful;
}
