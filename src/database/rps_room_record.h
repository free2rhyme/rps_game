#ifndef __RPS_ROOM_RECORD_H__
#define __RPS_ROOM_RECORD_H__

#include <mutex>
#include <set>
#include <unordered_map>
#include "lct_common_define.h"
#include "lct_hash_map.h"
#include "rps_room.h"

typedef std::set<shp_rps_room_t> rps_room_set_t;

class rps_room_record_t final: lct_object_uncopyable{
private:
	typedef lct_hash_map_t<rps_room_id_t, shp_rps_room_t, rps_room_id_hash > rps_room_map_t;
public:
	static rps_room_record_t* get_instance();

public:
	lct_error_code_t get_rps_room(const rps_room_id_t&, shp_rps_room_t&) const;
	lct_error_code_t add_rps_room(const shp_rps_room_t&);
	lct_error_code_t del_rps_room(const rps_room_id_t&);

	lct_error_code_t get_available_rooms(rps_room_set_t&, lct_uint32_t max_size = rps_room_set_default_size) const;

private:
	static void init_instance();

private:
	rps_room_record_t();
	~rps_room_record_t();

public:
	static constexpr lct_uint32_t	rps_room_set_default_size = 10;

private:
	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_room_record_t* 		m_ptr_instance;

	rps_room_map_t					m_rps_room_map;
	mutable std::mutex				m_game_record_mutex;
};

#define rps_room_record_sgt rps_room_record_t::get_instance()


#endif
