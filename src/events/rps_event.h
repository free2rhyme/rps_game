#ifndef __RPS_EVENT_H__
#define __RPS_EVENT_H__

#include <memory>
#include "rps_player.h"

enum class rps_user_event_t: lct_uint8_t{
	rps_user_event_log_in,
	rps_user_event_get_room_list,
	rps_user_event_setup_room,
	rps_user_event_select_room,
	rps_user_event_be_ready,
	rps_user_event_show_shape,
	rps_user_event_log_off
};

class rps_event_t{
public:
	explicit rps_event_t(const rps_user_event_t);
	virtual ~rps_event_t();
	rps_user_event_t get_event_type() const;
	virtual lct_error_code_t get_shp_rps_player(shp_rps_player_t&) const;
	virtual lct_error_code_t do_process() = 0;

private:
	rps_user_event_t m_rps_user_event;
};

class rps_event_login_t:public rps_event_t{
public:
	rps_event_login_t();
};

class rps_base_event_t:public rps_event_t{
public:
	explicit rps_base_event_t(const rps_user_event_t, const shp_rps_player_t&);
	virtual lct_error_code_t get_shp_rps_player(shp_rps_player_t&) const override;
protected:
	lct_error_code_t get_peer_player_shp(shp_rps_player_t&) const;
protected:
	shp_rps_player_t	m_shp_rps_player;
};

class rps_event_get_room_list_t:public rps_base_event_t{
public:
	explicit rps_event_get_room_list_t(const shp_rps_player_t&);
	virtual lct_error_code_t do_process() override;
};

class rps_event_setup_room_t:public rps_base_event_t{
public:
	explicit rps_event_setup_room_t(const shp_rps_player_t&, const lct_string_t&);
	virtual lct_error_code_t do_process() override;
protected:
	lct_string_t	m_room_name;
};

class rps_event_select_room_t:public rps_base_event_t{
public:
	explicit rps_event_select_room_t(const shp_rps_player_t&, const lct_uint32_t);
	virtual lct_error_code_t do_process() override;
protected:
	lct_error_code_t notify_peer_client(const shp_rps_player_t& ) const;
	lct_error_code_t notifiy_client(const shp_rps_player_t& ) const;
protected:
	lct_uint32_t	m_room_id;
};

class rps_event_be_ready_t:public rps_base_event_t{
public:
	explicit rps_event_be_ready_t(const shp_rps_player_t&);
	virtual lct_error_code_t do_process() override;
protected:
	lct_error_code_t notifiy_client(const shp_rps_player_t& ) const;
};


class rps_event_show_shape_t:public rps_base_event_t{
public:
	explicit rps_event_show_shape_t(const shp_rps_player_t&, const lct_uint8_t );
	virtual lct_error_code_t do_process() override;
private:
	lct_error_code_t handle_shape_value() const;
	lct_error_code_t game_determine(const shp_rps_player_t&);
	lct_error_code_t notify_client(const shp_rps_player_t&, const lct_uint8_t) const;
protected:
	lct_uint8_t	m_shape_value;
};

class rps_event_logoff_t:public rps_base_event_t{
public:
	explicit rps_event_logoff_t(const shp_rps_player_t&);
	virtual lct_error_code_t do_process() override;
protected:
	lct_error_code_t notify_peer_client(const shp_rps_player_t& ) const;
};

typedef std::shared_ptr<rps_event_t>	shp_rps_event_t;

#endif

