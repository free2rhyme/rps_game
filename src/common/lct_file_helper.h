#ifndef __LCT_FILE_HELPER_H__
#define __LCT_FILE_HELPER_H__

#include <fstream>
#include "lct_common_define.h"
#include "lct_object_replicability.h"

class lct_file_helper_t:lct_object_uncopyable{
public:
	lct_file_helper_t();
	virtual ~lct_file_helper_t();

	lct_error_code_t open(const lct_string_t&, const std::ios_base::openmode mode = std::ios::in|std::ios::out|std::ios::app);
	lct_error_code_t read();
	lct_error_code_t write_line(const lct_string_t&);
	lct_error_code_t write(const lct_char_t*);

	lct_error_code_t write_line(const lct_char_t*);
	lct_error_code_t save();
	lct_error_code_t close();

protected:
	std::ios_base::openmode	m_file_mode;
	lct_string_t*			m_file_name_ptr;
	std::fstream*			m_file_stream_ptr;
};

#endif



