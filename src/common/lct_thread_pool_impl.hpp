/*
 * lct_thread_pool_impl.hpp
 *
 *  Created on: Sep 14, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

template<typename Callable, typename... Args>
auto lct_thread_pool_impl_t::emplace_task(Callable&& func, Args&&... vargs)->std::future<typename std::result_of<Callable(Args...)>::type>{
    //std::function<typename std::result_of<Callable(Args...)>::type()> task(std::bind(std::forward<Callable>(func), std::forward<Args>(vargs)...));

	using result_type = typename std::result_of<Callable(Args...)>::type;

	std::packaged_task<result_type()> task(std::bind(std::forward<Callable>(func), std::forward<Args>(vargs)...));

	std::future<result_type> fur = task.get_future();
	m_tasks_queue.enqueue(std::move(task));
	return fur;
}
