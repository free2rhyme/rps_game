/*
 * lct_lock_free_queue.h
 *
 *  Created on: Sep 28, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef SRC_COMMON_LCT_LOCK_FREE_QUEUE_H_
#define SRC_COMMON_LCT_LOCK_FREE_QUEUE_H_

#include <atomic>
#include <mutex>
#include <memory>

template <typename T>
class lct_lock_free_queue_t final{
private:
	struct node;
	struct counted_node_ptr{
		int		external_count;
		node*	ptr;
		counted_node_ptr() noexcept(true);
		~counted_node_ptr();
		counted_node_ptr(const counted_node_ptr&);
		counted_node_ptr(counted_node_ptr&&);
		counted_node_ptr& operator=(const counted_node_ptr&);
		counted_node_ptr& operator=(counted_node_ptr&&);
	};

	std::atomic<counted_node_ptr>	head;
	std::atomic<counted_node_ptr>	tail;

	struct node_counter{
		unsigned internal_count:	30;
		unsigned external_counters:	2;
	};

	struct node{
		std::atomic<T*>				data;
		std::atomic<node_counter>	count;
		counted_node_ptr			next;

		node();
		node(const node&)				= delete;
		node(node&&)					= delete;
		node& operator=(const node&)	= delete;
		node& operator=(node&&)			= delete;
		void release_ref();
	};

	static void increase_external_count(std::atomic<counted_node_ptr>&, counted_node_ptr&);
	static void free_external_counter(counted_node_ptr&);

	lct_lock_free_queue_t(const lct_lock_free_queue_t&)		= delete;
	lct_lock_free_queue_t(lct_lock_free_queue_t&&)			= delete;
	lct_lock_free_queue_t& operator=(const lct_lock_free_queue_t&)	= delete;
	lct_lock_free_queue_t& operator=(lct_lock_free_queue_t&&)		= delete;

public:
	lct_lock_free_queue_t() noexcept(true);
	~lct_lock_free_queue_t();

	void push(const T& new_value);
	void push(T&& new_value);
	void push(T* new_value);
	std::unique_ptr<T> pop();
};

#include "lct_lock_free_queue_impl.hpp"

#endif /* SRC_COMMON_LCT_LOCK_FREE_QUEUE_H_ */
