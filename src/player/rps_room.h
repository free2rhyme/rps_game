#ifndef __RPS_ROOM_H__
#define __RPS_ROOM_H__

#include <functional>
#include <mutex>
#include "lct_common_define.h"
#include "rps_game_indentification.h"
#include "lct_object_replicability.h"

class rps_room_t: lct_object_uncopyable{
public:
	rps_room_t();
	rps_room_t(const rps_room_id_t&, const lct_string_t&, const rps_player_id_t&);
	~rps_room_t();


public:
	rps_room_id_t 		get_rps_room_id() const;
	lct_bool_t	  		is_destroyed() const;
	lct_bool_t	  		is_room_full() const;
	lct_string_t		get_room_name() const;
	rps_player_id_t		get_host_player_id() const;
	rps_player_id_t		get_visitor_player_id() const;
	lct_error_code_t	update_visitor_info(const rps_player_id_t&);
	lct_error_code_t	update_host_info(const rps_player_id_t&);
	lct_error_code_t	clear_visitor_info();
	lct_error_code_t	destroy_room();

private:
	lct_bool_t							m_is_destroyed_flag;
	const rps_room_id_t					m_rps_room_id;
	rps_player_id_t						m_host_id;
	rps_player_id_t 					m_visitor_id;
	lct_string_t						m_room_name;
	mutable std::recursive_mutex		m_mutex;
};

typedef std::shared_ptr<rps_room_t> shp_rps_room_t;

#endif

