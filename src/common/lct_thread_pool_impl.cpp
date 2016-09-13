/*
 * lct_thread_pool_impl.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#include "lct_thread_pool_impl.h"

lct_thread_pool_impl_t::lct_thread_pool_impl_t(const uint32_t thread_count):
	m_thread_count(thread_count),
	m_running_flag(false){
	init();
}

lct_thread_pool_impl_t::~lct_thread_pool_impl_t(){

}

void lct_thread_pool_impl_t::init(){
	m_running_flag = true;
	for(uint32_t i = 0; i < m_thread_count; ++i){
		m_workers.emplace_back(std::thread([this]{
			while(m_running_flag){
				lct_thread_pool_task_t	task;
				if (lct_error_code_t::lct_error_code_successful == m_tasks_queue.dequeue(task)){
					task();
				}
			}
		}));
	}
}

template<typename Callable, typename... Args>
void lct_thread_pool_impl_t::emplace_task(Callable&& func, Args&&... vargs){
    std::function<typename std::result_of<Callable(Args...)>::type()> task(std::bind(std::forward<Callable>(func), std::forward<Args>(vargs)...));
	m_tasks_queue.enqueue(task);
}



