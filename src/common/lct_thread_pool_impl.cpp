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
	m_running_flag = false;

	m_tasks_queue.notify_all();

	for(std::thread &worker: m_workers){
		worker.join();
	}
}

void lct_thread_pool_impl_t::init(){
	m_running_flag = true;
	for(uint32_t i = 0; i < m_thread_count; ++i){
		m_workers.emplace_back(std::thread([this]{
			while(m_running_flag){
				lct_thread_pool_task_t	task;
				if (lct_error_code_t::lct_error_code_successful == this->m_tasks_queue.dequeue(task)){
					task();
				}
			}
		}));
	}
}

void lct_thread_pool_impl_t::shutdown(){
	m_running_flag = false;
}

