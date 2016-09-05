
#include <cstring>
#include <sys/socket.h>
#include <errno.h>
#include "lct_tcp_connect_socket.h"
#include "lct_socket_manager.h"

lct_tcp_connect_socket_t::lct_tcp_connect_socket_t(
		const lct_uint32_t socket_id,
		const lct_uint16_t local_port_number,
		const lct_uint32_t local_ip_address,
		const lct_uint16_t peer_port_number,
		const lct_uint32_t peer_ip_address):
	lct_base_socket_t(socket_id, lct_socket_type_t::lct_socket_type_tcp_server,
			lct_socket_state_t::lct_socket_state_initiated, local_port_number, local_ip_address),
			m_peer_port_number(peer_port_number),
			m_peer_ip_address(peer_ip_address),
			m_recv_buffer_size(m_recv_buffer_default_size),
			m_bytes_in_recv_buffer(0),
			m_recv_buffer_ptr(nullptr){

}

lct_tcp_connect_socket_t::~lct_tcp_connect_socket_t(){
	if(m_recv_buffer_ptr != nullptr){
		delete[] m_recv_buffer_ptr;
	}
}

lct_error_code_t lct_tcp_connect_socket_t::init(){
	if(CreateRecvBuffer() != lct_error_code_t::lct_error_code_successful){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_tcp_connect_socket_t::reset_buffer(){
	bzero(m_recv_buffer_ptr, m_recv_buffer_size);
	m_bytes_in_recv_buffer = 0;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_tcp_connect_socket_t::get_peer_ip_address(lct_uint32_t& peer_ip_address) const{
	peer_ip_address = m_peer_ip_address;
	return lct_error_code_t::lct_error_code_successful;
}
lct_error_code_t lct_tcp_connect_socket_t::get_peer_port_number(lct_uint16_t& peer_port_number) const{
	peer_port_number = m_peer_port_number;
	return lct_error_code_t::lct_error_code_successful;
}


lct_error_code_t lct_tcp_connect_socket_t::process_read_event(lct_socket_manager_t* socket_mgr_ptr){
	LCT_TRACE_LOG_DEBUG();
	if(socket_mgr_ptr == nullptr){
		LCT_TRACE_LOG_ERROR();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	lct_char_t* recv_buffer_cur_ptr = m_recv_buffer_ptr;
	reset_buffer();
	while(true){
		lct_int32_t bytes_recv = recv(m_socket_id, recv_buffer_cur_ptr , m_recv_buffer_size - m_bytes_in_recv_buffer, 0);
		LCT_TRACE_LOG_DEBUG();
		std::cout << "bytes_recv is " << bytes_recv << " m_player_id is "<< m_player_id << std::endl;
		if(bytes_recv < 0){
			if(errno == EAGAIN || errno == EWOULDBLOCK){
				LCT_TRACE_LOG_DEBUG();
				break;
			} else {
				LCT_TRACE_LOG_ERROR();
				m_last_system_call_error = errno;
				socket_mgr_ptr->process_close_event(m_player_id);
				close_socket();
				return lct_error_code_t::lct_error_code_successful;
			}
		} else if(bytes_recv == 0){
			LCT_TRACE_LOG_DEBUG();
			socket_mgr_ptr->process_close_event(m_player_id);
			close_socket();
			return lct_error_code_t::lct_error_code_successful;
		} else {
			LCT_TRACE_LOG_DEBUG();

			m_bytes_in_recv_buffer += bytes_recv;
			std::cout << "m_bytes_in_recv_buffer is " << m_bytes_in_recv_buffer << std::endl;
			std::cout << "m_recv_buffer_ptr data is : " << m_recv_buffer_ptr << std::endl;
			std::string log_str(recv_buffer_cur_ptr, m_bytes_in_recv_buffer);
			std::cout << "received data is : " << log_str << std::endl;

			recv_buffer_cur_ptr += bytes_recv;
			LCT_TRACE_LOG_DEBUG();
		}
	}

	LCT_TRACE_LOG_DEBUG();

	shp_lct_input_stream_t lct_input_stream_shp(new lct_input_stream_t(m_recv_buffer_ptr, m_bytes_in_recv_buffer));
	return socket_mgr_ptr->process_read_event(m_socket_id, lct_input_stream_shp);
}

lct_error_code_t lct_tcp_connect_socket_t::CreateRecvBuffer(const lct_uint32_t max_message_size){
	m_recv_buffer_size = max_message_size;
	m_recv_buffer_ptr = new lct_char_t[max_message_size];

	LCT_TRACE_LOG_DEBUG();
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_tcp_connect_socket_t::process_write_event(const shp_lct_output_stream_t& output_stream_shp){

	const lct_char_t* sending_out_data = output_stream_shp->get_stream_data();
	std::cout << __PRETTY_FUNCTION__ << "m_socket_id is " << m_socket_id << std::endl;
	std::cout << __PRETTY_FUNCTION__ << "sending_out_data is " << sending_out_data << std::endl;
	std::cout << __PRETTY_FUNCTION__ << "output_stream_shp->size() is " << output_stream_shp->size() << std::endl;

	lct_int32_t result = send(m_socket_id, output_stream_shp->get_stream_data(), output_stream_shp->size(), 0);
	if(result < 0){
		m_last_system_call_error = errno;
		LCT_TRACE_LOG_ERROR();
		close_socket();
		return lct_error_code_t::lct_error_code_general_failure;
	}

	return lct_error_code_t::lct_error_code_successful;
}

