/*
 * lct_thread_pool_impl.h
 *
 *  Created on: Sep 13, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef SRC_COMMON_LCT_THREAD_POOL_IMPL_H_
#define SRC_COMMON_LCT_THREAD_POOL_IMPL_H_

#include "lct_common_define.h"
#include "lct_object_replicability.h"
#include <vector>
#include <future>
#include "lct_queue.h"

class lct_thread_pool_impl_t:lct_object_uncopyable{
public:
	explicit lct_thread_pool_impl_t(const lct_uint32_t);
	~lct_thread_pool_impl_t();

	template<typename Callable, typename... Args>
	auto emplace_task(Callable&& func, Args&&... vargs)->std::future<typename std::result_of<Callable(Args...)>::type>;

	void shutdown();
private:
	std::vector<std::thread>		m_workers;
	uint32_t						m_thread_count;
	lct_bool_t						m_running_flag;

	typedef std::function<void()>	lct_thread_pool_task_t;
	lct_queue_t<lct_thread_pool_task_t>		m_tasks_queue;
private:
	void init();
};

#include "lct_thread_pool_impl.hpp"

#endif /* SRC_COMMON_LCT_THREAD_POOL_IMPL_H_ */
