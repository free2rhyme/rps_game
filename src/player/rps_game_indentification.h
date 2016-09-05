#ifndef __RPS_GAME_INDENTIFICATION_H__
#define __RPS_GAME_INDENTIFICATION_H__

#include "lct_common_define.h"


class rps_player_id_t{
public:
	rps_player_id_t();
	explicit rps_player_id_t(const lct_uint32_t);
	rps_player_id_t(const rps_player_id_t&);
	rps_player_id_t& operator = (const rps_player_id_t&);
	lct_bool_t operator == (const rps_player_id_t&) const;
	~rps_player_id_t();

	lct_uint32_t value() const{
		return m_player_id;
	}
	lct_bool_t is_dummy() const{
		return m_player_id == 0;
	}
private:
	lct_uint32_t	m_player_id;
};

struct rps_player_id_hash{
	lct_uint32_t operator()(const rps_player_id_t& __val) const{
		return std::hash<lct_uint32_t>()(__val.value());
	}
};


class rps_room_id_t{
public:
	rps_room_id_t();
	explicit rps_room_id_t(const lct_uint32_t);
	rps_room_id_t(const rps_room_id_t&);
	rps_room_id_t& operator = (const rps_room_id_t&);
	rps_room_id_t& operator = (rps_room_id_t&&);
	lct_bool_t operator == (const rps_room_id_t&) const;
	~rps_room_id_t();
	lct_uint32_t value() const;
private:
	lct_uint32_t	m_room_id;
};

struct rps_room_id_hash{
	lct_uint32_t operator()(const rps_room_id_t& __val) const{
		return std::hash<lct_uint32_t>()(__val.value());
	}
};


#endif

