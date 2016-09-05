#ifndef __RPS_PLAYER_RECORD_H__
#define __RPS_PLAYER_RECORD_H__

#include <mutex>
#include <unordered_map>
#include "lct_common_define.h"
#include "lct_hash_map.h"
#include "rps_player.h"

class rps_player_record_t final: lct_object_uncopyable{
private:
	typedef lct_hash_map_t<rps_player_id_t, shp_rps_player_t, rps_player_id_hash > rps_player_map_t;
public:
	static rps_player_record_t* get_instance();

public:
	lct_error_code_t get_rps_player(const rps_player_id_t&, shp_rps_player_t&) const;
	lct_error_code_t add_rps_player(const shp_rps_player_t&);
	lct_error_code_t del_rps_player(const rps_player_id_t&);

private:
	static void init_instance();

private:
	rps_player_record_t();
	~rps_player_record_t();


private:
	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;
	static rps_player_record_t* 	m_ptr_instance;

	rps_player_map_t				m_rps_player_map;
	mutable std::mutex				m_game_record_mutex;
};

#define rps_player_record_sgt rps_player_record_t::get_instance()


#endif
