#ifndef __LCT_INPUT_STREAM_H__
#define __LCT_INPUT_STREAM_H__

#include "lct_common_define.h"
#include <memory>
#include "lct_object_replicability.h"

class lct_input_stream_t: lct_object_uncopyable{
public:
	lct_input_stream_t(const lct_char_t*, const lct_uint32_t, lct_bool_t is_network_order=true);
	virtual ~lct_input_stream_t();
public:
	lct_error_code_t read(lct_uchar_t&);
	lct_error_code_t read(lct_char_t&);
	lct_error_code_t read(lct_uint16_t&);
	lct_error_code_t read(lct_int16_t&);
	lct_error_code_t read(lct_uint32_t&);
	lct_error_code_t read(lct_int32_t&);
	lct_error_code_t read(lct_uint64_t&);
	lct_error_code_t read(lct_int64_t&);
	lct_error_code_t read(void*, const lct_uint64_t);
	lct_error_code_t skip(const lct_uint32_t);

	lct_error_code_t reset_stream();
	const lct_char_t* data() const;

	lct_uint32_t	length() const;
	lct_uint32_t	size() const;

protected:
	lct_bool_t					m_network_order_flag;
	lct_uint32_t				m_stream_size;
	lct_uint32_t				m_offset;
	const lct_char_t*			m_stream_ptr;
};

typedef std::shared_ptr<lct_input_stream_t>	shp_lct_input_stream_t;

#endif
