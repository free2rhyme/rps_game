#include <arpa/inet.h>
#include <cstring>
#include "lct_output_stream.h"

lct_output_stream_t::lct_output_stream_t(const lct_uint32_t stream_size):
	m_stream_size(stream_size), m_offset(0), m_stream_ptr(new lct_char_t[stream_size + 1]){
	LCT_TRACE_LOG_DEBUG();
	std::cout << __PRETTY_FUNCTION__ << " stream_size is " << stream_size << std::endl;
	*m_stream_ptr = '\0';
}

lct_output_stream_t::~lct_output_stream_t(){
	if(m_stream_ptr != nullptr){
		delete[] m_stream_ptr;
		m_stream_ptr = nullptr;
	}
}

lct_error_code_t lct_output_stream_t::write(const lct_uchar_t data){
	LCT_TRACE_LOG_ERROR();
	if(sizeof(lct_uchar_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_uchar_t*)(m_stream_ptr + m_offset)) = data;
	m_offset += sizeof(lct_uchar_t);

	*(m_stream_ptr + m_offset) = '\0';

	std::cout << __PRETTY_FUNCTION__ << "\t data is " << data << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const lct_char_t data){
	LCT_TRACE_LOG_DEBUG();
	if(sizeof(lct_char_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_char_t*)(m_stream_ptr + m_offset)) = data;
	m_offset += sizeof(lct_char_t);

	*(m_stream_ptr + m_offset) = '\0';
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const lct_uint16_t data){
	LCT_TRACE_LOG_DEBUG();
	if(sizeof(lct_uint16_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_uint16_t*)(m_stream_ptr + m_offset)) = htons(data);
	m_offset += sizeof(lct_uint16_t);

	*(m_stream_ptr + m_offset) = '\0';
	std::cout << __PRETTY_FUNCTION__ << "\t data is " << data << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const lct_int16_t data){
	LCT_TRACE_LOG_DEBUG();
	if(sizeof(lct_int16_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_int16_t*)(m_stream_ptr + m_offset)) = htons(data);
	m_offset += sizeof(lct_int16_t);

	*(m_stream_ptr + m_offset) = '\0';
	std::cout << __PRETTY_FUNCTION__ << "\t data is " << data << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const lct_uint32_t data){
	LCT_TRACE_LOG_DEBUG();
	if(sizeof(lct_uint32_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_uint32_t*)(m_stream_ptr + m_offset)) = htonl(data);
	m_offset += sizeof(lct_uint32_t);
	*(m_stream_ptr + m_offset) = '\0';
	std::cout << __PRETTY_FUNCTION__ << "\t data is " << data << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const lct_int32_t data){
	LCT_TRACE_LOG_DEBUG();
	if(sizeof(lct_int32_t) + m_offset > m_stream_size){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	*((lct_int32_t*)(m_stream_ptr + m_offset)) = htonl(data);
	m_offset += sizeof(lct_int32_t);

	*(m_stream_ptr + m_offset) = '\0';
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_output_stream_t::write(const void* data_ptr, const lct_uint32_t length){
	LCT_TRACE_LOG_DEBUG();
	if(length + m_offset > m_stream_size){
		std::cout << "length is " << length << std::endl;
		std::cout << "m_offset is " << m_offset << std::endl;
		std::cout << "m_stream_size is " << m_stream_size << std::endl;

		LCT_TRACE_LOG_DEBUG();
		return lct_error_code_t::lct_error_code_general_failure;
	}
	memcpy(m_stream_ptr + m_offset, data_ptr, length);
	m_offset += length;

	*(m_stream_ptr + m_offset) = '\0';
	return lct_error_code_t::lct_error_code_successful;
}

lct_uint32_t lct_output_stream_t::size() const{
	return m_stream_size;
}

lct_char_t* lct_output_stream_t::extract_stream(){
	lct_char_t* ptr = m_stream_ptr;
	m_stream_ptr = nullptr;
	return ptr;
}

const lct_char_t* lct_output_stream_t::get_stream_data() const{
	return m_stream_ptr;
}


