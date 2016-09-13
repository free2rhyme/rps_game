/*
 * lct_lock_free_stack_impl.hpp
 *
 *  Created on: Sep 13, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

template <typename T>
void lct_lock_free_stack<T>::increase_head_count(counted_node_ptr& old_counter){
	counted_node_ptr new_counter;
	do{
		new_counter	= old_counter;
		++new_counter.external_count;
	}while(!head.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));
	old_counter.external_count = new_counter.external_count;
}

template <typename T>
lct_lock_free_stack<T>::lct_lock_free_stack():head(){
}

template <typename T>
lct_lock_free_stack<T>::~lct_lock_free_stack(){
	while(pop().get() != nullptr);
}

template <typename T>
void lct_lock_free_stack<T>::push(T const& data){
	counted_node_ptr new_node;
	new_node.ptr = new node(data);
	new_node.external_count = 1;
	new_node.ptr->next = head.load(std::memory_order_relaxed);
	while(!head.compare_exchange_weak(new_node.ptr->next, new_node, std::memory_order_release, std::memory_order_relaxed));
}

template <typename T>
std::shared_ptr<T> lct_lock_free_stack<T>::pop(){
	counted_node_ptr old_head = head.load(std::memory_order_relaxed);
	while(true){
		increase_head_count(old_head);
		node* const ptr = old_head.ptr;
		if(!ptr){
			return std::shared_ptr<T>();
		}

		if(head.compare_exchange_strong(old_head, ptr->next, std::memory_order_relaxed)){
			std::shared_ptr<T> res;
			res.swap(ptr->data);

			int32_t const count_increase = old_head.external_count - 2;
			if(ptr->internal_count.fetch_add(count_increase, std::memory_order_release) == -count_increase){
				delete ptr;
			}
			return res;
		} else if(ptr->internal_count.fetch_add(-1, std::memory_order_relaxed) == 1){
			ptr->internal_count.load(std::memory_order_acquire);
			delete ptr;
		}
	}
}
