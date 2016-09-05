#ifndef __RPS_RESPONSE_MESSAGE_H__
#define __RPS_RESPONSE_MESSAGE_H__

#include "rps_message.h"
#include "rps_room_record.h"
#include "lct_output_stream.h"

class rps_response_message_t:public rps_message_t{
public:
	rps_response_message_t(const lct_uint16_t, const rps_message_type_t, const lct_uint32_t);

	virtual lct_error_code_t	init();
	virtual ~rps_response_message_t();
	virtual lct_uint32_t		get_message_body_size() const override;
	virtual lct_error_code_t	build_stream();
	shp_lct_output_stream_t		get_output_stream_shp() const;
protected:
	shp_lct_output_stream_t		m_lct_output_stream_shp;
};

typedef std::shared_ptr<rps_response_message_t> shp_rps_response_message_t;

class rps_game_login_response_message_t:public rps_response_message_t{
	struct rps_game_room_struct_t{
		lct_uint32_t	m_room_id;
		lct_char_t		m_room_name[RPS_ROOM_NAME_MAX_LENGTH];
	};
public:
	rps_game_login_response_message_t(const lct_uint32_t, const rps_room_set_t&);
	virtual ~rps_game_login_response_message_t();

public:
	virtual lct_error_code_t	build_stream();
	virtual lct_uint32_t		get_message_body_size() const override;
protected:
	const lct_uint32_t		m_rooms_size;
	rps_game_room_struct_t*	m_available_rooms_ptr;
};

class rps_game_enter_room_response_message_t:public rps_response_message_t{
public:
	rps_game_enter_room_response_message_t(const lct_uint32_t, const lct_string_t&);
	virtual ~rps_game_enter_room_response_message_t();

public:
	virtual lct_error_code_t	build_stream();
	virtual lct_uint32_t		get_message_body_size() const override;
protected:
	lct_string_t	m_peer_name;
};

class rps_peer_ready_response_message_t:public rps_response_message_t{
public:
	rps_peer_ready_response_message_t(const lct_uint32_t);
	virtual ~rps_peer_ready_response_message_t();
};


class rps_game_result_response_message_t:public rps_response_message_t{
public:
	rps_game_result_response_message_t(const lct_uint32_t, const lct_uint8_t,
			const lct_uint32_t, const lct_uint32_t, const lct_uint32_t);
	virtual ~rps_game_result_response_message_t();

public:
	virtual lct_error_code_t	build_stream();
	virtual lct_uint32_t		get_message_body_size() const override;
protected:
	lct_uint8_t		m_game_result;
    lct_uint32_t	m_wins;
    lct_uint32_t	m_ties;
    lct_uint32_t 	m_losses;
};

class rps_peer_log_off_response_message_t:public rps_response_message_t{
public:
	rps_peer_log_off_response_message_t(const lct_uint32_t);
	virtual ~rps_peer_log_off_response_message_t();
};

#endif
