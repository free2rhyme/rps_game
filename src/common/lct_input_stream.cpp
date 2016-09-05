#include "lct_input_stream.h"
#include "lct_utility.h"
#include <arpa/inet.h>
#include <cstring>

lct_input_stream_t::lct_input_stream_t(const lct_char_t* stream_ptr, const lct_uint32_t stream_size, lct_bool_t is_network_order):
	m_network_order_flag(is_network_order),
	m_stream_size(stream_size),
	m_offset(0),
	m_stream_ptr(stream_ptr){
}

lct_input_stream_t::~lct_input_stream_t(){
	m_stream_ptr = nullptr;
}

lct_error_code_t lct_input_stream_t::read(lct_uchar_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *(static_cast<const lct_char_t*>(m_stream_ptr + m_offset));
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_char_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_char_t*)(m_stream_ptr + m_offset));
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_uint16_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_uint16_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohs(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_int16_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_int16_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohs(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_uint32_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_uint32_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohl(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_int32_t& value_ref){
	if (( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_int32_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohl(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_uint64_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_uint64_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohll(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(lct_int64_t& value_ref){
	if(( m_offset + sizeof(value_ref)) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	value_ref = *((lct_int64_t*)(m_stream_ptr + m_offset));
	if(m_network_order_flag){
		value_ref = ntohll(value_ref);
	}
	m_offset += sizeof(value_ref);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::read(void* buffer_ptr, const lct_uint64_t length){
	if((m_offset + length) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}

	if(buffer_ptr != nullptr){
		memcpy(buffer_ptr, m_stream_ptr+m_offset, length);
		m_offset += length;
	} else {
		return lct_error_code_t::lct_error_code_general_failure;
	}
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::skip(const lct_uint32_t length){
	if((m_offset + length ) > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_offset += length;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_input_stream_t::reset_stream(){
	m_offset = 0;
	return lct_error_code_t::lct_error_code_successful;
}

const lct_char_t* lct_input_stream_t::data() const{
	return (m_stream_ptr + m_offset);
}

lct_uint32_t lct_input_stream_t::length() const{
	return m_stream_size - m_offset;
}

lct_uint32_t lct_input_stream_t::size() const{
	return m_stream_size;
}

