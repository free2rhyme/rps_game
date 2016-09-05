#ifndef __LCT_OUTPUT_STREAM_H__
#define __LCT_OUTPUT_STREAM_H__

#include <memory>
#include "lct_common_define.h"
#include "lct_object_replicability.h"

class lct_output_stream_t: lct_object_uncopyable{
public:
	lct_output_stream_t(const lct_uint32_t);
	virtual ~lct_output_stream_t();

public:
	lct_error_code_t write(const lct_uchar_t);
	lct_error_code_t write(const lct_char_t);
	lct_error_code_t write(const lct_uint16_t);
	lct_error_code_t write(const lct_int16_t);
	lct_error_code_t write(const lct_uint32_t);
	lct_error_code_t write(const lct_int32_t);
	lct_error_code_t write(const void*, const lct_uint32_t);

	lct_uint32_t		size() const;
	const lct_char_t*	get_stream_data() const;
	lct_char_t*			extract_stream();

protected:
	lct_uint32_t		m_stream_size;
	lct_uint32_t		m_offset;
	lct_char_t*			m_stream_ptr;
};

typedef std::shared_ptr<lct_output_stream_t>	shp_lct_output_stream_t;

#endif
