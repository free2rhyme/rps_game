#ifndef __RPS_MESSAGE_H__
#define __RPS_MESSAGE_H__

#include <memory>
#include "lct_common_define.h"
#include "rps_player.h"
enum class rps_message_type_t:lct_uint8_t{
	rps_message_type_undefined,
	rps_server_keepalive_request,
	rps_client_keepalive_response,
	rps_client_log_in_request,
	rps_server_log_in_response,
	rps_client_show_shape_request,
	rps_server_show_shape_response,
	rps_client_log_off_request,
	rps_server_log_off_response,
	rps_server_update_score_request,
	rps_client_update_score_response,

	rps_client_setup_new_room_request,
	rps_server_setup_new_room_response,
	rps_client_select_a_room_request,
	rps_server_select_a_room_response,
	rps_client_be_ready_request,
	rps_server_peer_be_ready_response
};

struct rps_message_type_hash{
	lct_uint32_t operator()(const rps_message_type_t& __val) const{
		return std::hash<lct_int8_t>()((lct_int8_t)__val);
	}
};


#define RPS_MESSAGE_TYPE_MASK			((lct_uint16_t)0x0003)
#define RPS_MESSAGE_MASK_REQUEST		((lct_uint16_t)0x0001)
#define RPS_MESSAGE_MASK_RESPONSE		((lct_uint16_t)0x0002)

#define RPS_MESSAGE_SCENARIO_MASK		((lct_uint16_t)0x1FF0)
#define RPS_MESSAGE_MASK_LOG_IN			((lct_uint16_t)0x0010)
#define RPS_MESSAGE_MASK_SHOW_SHAPE		((lct_uint16_t)0x0020)
#define RPS_MESSAGE_MASK_LOG_OFF		((lct_uint16_t)0x0040)
#define RPS_MESSAGE_MASK_UPDATE_SCORE	((lct_uint16_t)0x0080)
#define RPS_MESSAGE_MASK_SET_ROOM		((lct_uint16_t)0x0100)
#define RPS_MESSAGE_MASK_SELECT_ROOM	((lct_uint16_t)0x0200)
#define RPS_MESSAGE_MASK_BE_READY		((lct_uint16_t)0x0300)
#define RPS_MESSAGE_MASK_KEEPALIVE		((lct_uint16_t)0x1000)

struct rps_message_header_t{
public:
	explicit rps_message_header_t(const lct_uint16_t);
	~rps_message_header_t();

	lct_error_code_t	set_message_id(const lct_uint16_t);
	lct_uint16_t		get_message_id() const;
private:
	lct_uint16_t		m_rps_message_id;
};

typedef std::shared_ptr<rps_message_header_t>	shp_rps_message_header_t;

class rps_message_t{
public:
	rps_message_type_t 		get_message_type() const;
	rps_player_id_t 		get_player_id() const;
	lct_uint32_t			get_message_header_size() const;
	virtual lct_uint32_t	get_message_body_size() const = 0;
	virtual lct_uint32_t	get_message_size() const;

public:
	rps_message_t(const lct_uint16_t, const rps_message_type_t, const lct_uint32_t);
	virtual ~rps_message_t();

protected:
	rps_message_header_t		m_rps_message_header;
	rps_message_type_t			m_message_type;
	lct_uint32_t				m_message_body_size;
	lct_uint32_t				m_player_id;
};

typedef std::shared_ptr<rps_message_t>			shp_rps_message_t;

#endif



