/*
 * lct_thread_pool.hpp
 *
 *  Created on: Sep 14, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#include "lct_thread_pool_impl.h"

template<typename Callable, typename... Args>
void lct_thread_pool_t::emplace_task(Callable&& func, Args&&... vargs){
	m_thread_pool_impl_ptr->emplace_task(func, vargs...);
}

