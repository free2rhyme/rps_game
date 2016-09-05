#ifndef __LCT_RANDOM_H__
#define __LCT_RANDOM_H__

#include <random>
#include "lct_common_define.h"

template<typename T = lct_int32_t >
class lct_random_t final: lct_object_uncopyable{
private:
	typedef	T		object_type;
public:
	lct_random_t();
	lct_random_t(const lct_int32_t, const lct_int32_t);
	~lct_random_t();

	object_type random();
private:
	const lct_int32_t								m_lct_min;
	const lct_int32_t								m_lct_max;
	std::random_device								m_lct_random_device;
	std::default_random_engine 						m_lct_engine;
	std::uniform_int_distribution<object_type>		m_lct_uid;
};

#include "lct_random_impl.hpp"

#endif

