/*
 * lct_lock_free_stack.h
 *
 *  Created on: Sep 12, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef __LCT_LOCK_FREE_STACK_H__
#define __LCT_LOCK_FREE_STACK_H__

#include <atomic>
#include <memory>
#include <future>
#include <lct_common_define.h>

template <typename T>
class lct_lock_free_stack{
private:
	struct node;
	struct counted_node_ptr{
		int32_t		external_count;
		node*		ptr;
	};

	struct node{
		std::shared_ptr<T>		data;
		std::atomic<int32_t>	internal_count;
		counted_node_ptr		next;
		node(T const& data_):data(std::make_shared<T>(data)), internal_count(0){}
	};

	std::atomic<counted_node_ptr> head;

	void increase_head_count(counted_node_ptr& old_counter);

public:
	lct_lock_free_stack();
	~lct_lock_free_stack();

	void push(T const& data);
	std::shared_ptr<T> pop();
};

#include "lct_lock_free_stack_impl.hpp"

#endif
