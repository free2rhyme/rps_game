#ifndef __RPS_PLAYER_H__
#define __RPS_PLAYER_H__

#include <memory>
#include <functional>
#include "lct_common_define.h"
#include "lct_base_socket.h"
#include "rps_game_indentification.h"
#include "lct_object_replicability.h"

#define RPS_GAME_MINE_OPTION_TYPE_MASK			((lct_uint8_t)0x07)
#define RPS_GAME_MINE_OPTION_ROCK_MASK			((lct_uint8_t)0x01)
#define RPS_GAME_MINE_OPTION_PAPER_MASK			((lct_uint8_t)0x02)
#define RPS_GAME_MINE_OPTION_SCISSORS_MASK		((lct_uint8_t)0x03)
#define RPS_GAME_MINE_OPTION_RANDOM_MASK		((lct_uint8_t)0x04)

#define RPS_GAME_PEER_SHAPE_TYPE_MASK			((lct_uint8_t)0x38)
#define RPS_GAME_PEER_SHAPE_ROCK_MASK			((lct_uint8_t)0x08)
#define RPS_GAME_PEER_SHAPE_PAPER_MASK			((lct_uint8_t)0x10)
#define RPS_GAME_PEER_SHAPE_SCISSORS_MASK		((lct_uint8_t)0x18)

#define RPS_GAME_RESULT_TYPE_MASK				((lct_uint8_t)0xC0)
#define RPS_GAME_RESULT_WIN_MASK				((lct_uint8_t)0x40)
#define RPS_GAME_RESULT_TIE_MASK				((lct_uint8_t)0x80)
#define RPS_GAME_RESULT_LOSE_MASK				((lct_uint8_t)0xC0)

enum class rps_player_role_t: lct_uint8_t{
	rps_play_role_undefined,
	rps_play_role_host,
	rps_play_role_visitor
};

enum class rps_game_shape_t: lct_uint8_t{
	rps_game_shape_undefined	= 0,
	rps_game_shape_rock			= RPS_GAME_MINE_OPTION_ROCK_MASK,
	rps_game_shape_paper		= RPS_GAME_MINE_OPTION_PAPER_MASK,
	rps_game_shape_scissors		= RPS_GAME_MINE_OPTION_SCISSORS_MASK
};

class rps_player_t: lct_object_uncopyable{
public:
	rps_player_t(const rps_player_role_t, const shp_lct_base_socket_t);
	rps_player_t(const rps_player_role_t, const rps_player_id_t&, const shp_lct_base_socket_t);
	rps_player_t(const rps_player_role_t, const rps_player_id_t&, const lct_string_t&, const shp_lct_base_socket_t);
	rps_player_t(const rps_player_role_t, const lct_uint32_t&, const lct_string_t&, const shp_lct_base_socket_t);

	virtual ~rps_player_t();

public:
	rps_player_id_t 		get_rps_player_id() const;
	rps_player_role_t		get_rps_player_role() const;
	lct_string_t			get_rps_player_name() const;
	shp_lct_base_socket_t	get_lct_socket_shp() const;
	rps_room_id_t			get_rps_room_id() const;
	rps_game_shape_t		get_rps_game_shape() const;

	lct_uint32_t			get_wins() const;
	lct_uint32_t			get_ties() const;
	lct_uint32_t			get_losses() const;

	lct_error_code_t		set_rps_room_id(const rps_room_id_t&);
	lct_error_code_t		set_rps_player_role(const rps_player_role_t);
	lct_error_code_t		set_rps_game_shape(const rps_game_shape_t);
	lct_error_code_t		reset_game_shape();
	lct_error_code_t		set_as_host();
	lct_error_code_t		set_as_visitor();
	lct_error_code_t		win_again();
	lct_error_code_t		tie_again();
	lct_error_code_t		lose_again();

protected:
	rps_player_role_t			m_rps_player_role;
	rps_game_shape_t			m_rps_game_shape;
	rps_room_id_t				m_rps_room_id;
	const rps_player_id_t		m_rps_player_id;

	lct_uint32_t				m_wins;
	lct_uint32_t				m_ties;
	lct_uint32_t				m_losses;

	shp_lct_base_socket_t		m_lct_socket_shp;
	lct_string_t				m_rps_player_name;
};


typedef std::shared_ptr<rps_player_t> shp_rps_player_t;


#endif
