/*
 * lct_thread_pool.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#include "lct_thread_pool.h"

lct_bool_t				lct_thread_pool_t::m_instanced_flag		= false;
std::once_flag			lct_thread_pool_t::m_once_flag;
lct_thread_pool_t* 		lct_thread_pool_t::m_ptr_instance		= nullptr;

lct_thread_pool_t::lct_thread_pool_t():m_thread_pool_impl_ptr(nullptr){
	uint32_t thread_size = std::thread::hardware_concurrency();
	thread_size = thread_size > 2 ? (thread_size - 1): 3;
	m_thread_pool_impl_ptr = new lct_thread_pool_impl_t(thread_size);
}

lct_thread_pool_t::~lct_thread_pool_t(){
	if(m_thread_pool_impl_ptr != nullptr){
		delete m_thread_pool_impl_ptr;
		m_thread_pool_impl_ptr = nullptr;
	}
}

void lct_thread_pool_t::init_instance(){
	m_ptr_instance = new lct_thread_pool_t();
	m_instanced_flag = true;
}

lct_thread_pool_t* lct_thread_pool_t::get_instance(){
	if(m_instanced_flag == false){
		std::call_once(m_once_flag, init_instance);
	}
	return m_ptr_instance;
}

void lct_thread_pool_t::shutdown(){
	if(m_instanced_flag){
		m_thread_pool_impl_ptr->shutdown();
	}
}
