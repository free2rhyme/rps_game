/*
 * lct_thread_pool.h
 *
 *  Created on: Sep 13, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef __LCT_THREAD_POOL_H_
#define __LCT_THREAD_POOL_H_

#include "lct_common_define.h"
#include "lct_object_replicability.h"
#include <future>
#include <mutex>

class lct_thread_pool_impl_t;

class lct_thread_pool_t:lct_object_uncopyable{
public:
	static lct_thread_pool_t* get_instance();
	void shutdown();

	template<typename Callable, typename... Args>
	auto emplace_task(Callable&& func, Args&&... vargs)->std::future<typename std::result_of<Callable(Args...)>::type>;
private:
	lct_thread_pool_impl_t* 		m_thread_pool_impl_ptr;
	static lct_thread_pool_t*		m_ptr_instance;
	static lct_bool_t				m_instanced_flag;
	static std::once_flag			m_once_flag;

private:
	explicit lct_thread_pool_t();
	~lct_thread_pool_t();
	static void init_instance();
};

#include "lct_thread_pool.hpp"

#endif /* SRC_COMMON_LCT_THREAD_POOL_H_ */
