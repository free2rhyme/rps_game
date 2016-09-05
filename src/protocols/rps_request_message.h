#ifndef __RPS_REQUEST_MESSAGE_H__
#define __RPS_REQUEST_MESSAGE_H__
#include "rps_message.h"
#include "lct_input_stream.h"

class rps_request_message_t:public rps_message_t{
public:

	explicit rps_request_message_t(const lct_uint16_t, const rps_message_type_t, const lct_int32_t);
	virtual lct_error_code_t parse(const lct_char_t*, const lct_uint32_t);

	virtual lct_uint32_t	get_message_body_size() const override;
	virtual lct_error_code_t get_user_name(lct_string_t&) const;
	virtual lct_error_code_t get_room_name(lct_string_t&) const;
	virtual lct_error_code_t get_room_id(lct_uint32_t&) const;
	virtual lct_error_code_t get_shape(lct_uint8_t&) const;
	lct_int32_t	get_socket_id() const;

protected:
	lct_error_code_t parse(shp_lct_input_stream_t&);

protected:
	lct_int32_t				m_socket_id;
};

typedef std::shared_ptr<rps_request_message_t> shp_rps_request_message_t;

class rps_message_keepalive_request_t:public rps_request_message_t{
public:
	explicit rps_message_keepalive_request_t(const lct_int32_t);
	static rps_request_message_t* new_instance(const lct_int32_t);
};

class rps_message_login_request_t:public rps_request_message_t{
public:
	static rps_request_message_t* new_instance(const lct_int32_t);
	rps_message_login_request_t(const lct_int32_t);
	virtual lct_error_code_t parse(const lct_char_t*, const lct_uint32_t) override;
	virtual lct_error_code_t get_user_name(lct_string_t&) const override;

protected:
	lct_string_t	m_user_name;
};

class rps_message_setup_new_room_request_t:public rps_request_message_t{
public:
	static rps_request_message_t* new_instance(const lct_int32_t);
	rps_message_setup_new_room_request_t(const lct_int32_t);
	virtual lct_error_code_t parse(const lct_char_t*, const lct_uint32_t) override;
	virtual lct_error_code_t get_room_name(lct_string_t&) const override;

protected:
	lct_string_t	m_room_name;
};

class rps_message_select_a_room_request_t:public rps_request_message_t{
public:
	static rps_request_message_t* new_instance(const lct_int32_t);
	rps_message_select_a_room_request_t(const lct_int32_t);
	virtual lct_error_code_t parse(const lct_char_t*, const lct_uint32_t) override;
	virtual lct_error_code_t get_room_id(lct_uint32_t&) const override;
protected:
	lct_uint32_t	m_room_id;
};

class rps_message_be_ready_request_t:public rps_request_message_t{
public:
	static rps_request_message_t* new_instance(const lct_int32_t);
	explicit rps_message_be_ready_request_t(const lct_int32_t);
};

class rps_message_show_shape_request_t:public rps_request_message_t{
public:
	static rps_request_message_t* new_instance(const lct_int32_t);
	explicit rps_message_show_shape_request_t(const lct_int32_t);
	virtual lct_error_code_t parse(const lct_char_t*, const lct_uint32_t) override;
	virtual lct_error_code_t get_shape(lct_uint8_t&) const override;
protected:
	lct_uint8_t		m_shape;
};


#endif
