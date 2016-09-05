
#include "lct_file_helper.h"

lct_file_helper_t::lct_file_helper_t():m_file_mode(), m_file_name_ptr(nullptr), m_file_stream_ptr(nullptr){

}
lct_file_helper_t::~lct_file_helper_t(){
	if(m_file_name_ptr != nullptr){
		delete m_file_name_ptr;
	}
	if(m_file_stream_ptr != nullptr){
		if(m_file_stream_ptr->is_open()){
			close();
		}
		delete m_file_stream_ptr;
	}
}
lct_error_code_t lct_file_helper_t::open(const lct_string_t& file_name, const std::ios_base::openmode file_mode){
	if(file_name.size() == 0){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_file_name_ptr 	= new lct_string_t(file_name);
	m_file_mode			= file_mode;
	m_file_stream_ptr 	= new std::fstream(m_file_name_ptr->data(), m_file_mode);
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_file_helper_t::read(){
	return lct_error_code_t::lct_error_code_general_failure;
}

lct_error_code_t lct_file_helper_t::write_line(const lct_string_t& str){
	write_line(str.data());
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_file_helper_t::write(const lct_char_t* data){
	if((m_file_stream_ptr == nullptr) || (!m_file_stream_ptr->is_open())){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	(*m_file_stream_ptr) << data;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_file_helper_t::write_line(const lct_char_t* data){
	if((m_file_stream_ptr == nullptr) || (!m_file_stream_ptr->is_open())){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	(*m_file_stream_ptr) << data << std::endl;
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_file_helper_t::save(){
	if((m_file_stream_ptr == nullptr) || (!m_file_stream_ptr->is_open())){
		return lct_error_code_t::lct_error_code_general_failure;
	}
	m_file_stream_ptr->flush();
	return lct_error_code_t::lct_error_code_successful;
}

lct_error_code_t lct_file_helper_t::close(){
	if(m_file_stream_ptr != nullptr){
		if(m_file_stream_ptr->is_open()){
			save();
			m_file_stream_ptr->close();
		}
	}
	return lct_error_code_t::lct_error_code_successful;
}
