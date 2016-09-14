/*
 * lct_thread_pool_impl.hpp
 *
 *  Created on: Sep 14, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

template<typename Callable, typename... Args>
void lct_thread_pool_impl_t::emplace_task(Callable&& func, Args&&... vargs){
    std::function<typename std::result_of<Callable(Args...)>::type()> task(std::bind(std::forward<Callable>(func), std::forward<Args>(vargs)...));
	m_tasks_queue.enqueue(task);
}
