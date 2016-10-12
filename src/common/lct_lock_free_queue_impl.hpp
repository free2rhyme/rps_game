/*
 * lct_lock_free_queue_impl.hpp
 *
 *  Created on: Sep 28, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef SRC_COMMON_LCT_LOCK_FREE_QUEUE_IMPL_HPP_
#define SRC_COMMON_LCT_LOCK_FREE_QUEUE_IMPL_HPP_

template <typename T>
lct_lock_free_queue_t<T>::counted_node_ptr::counted_node_ptr() noexcept(true):external_count(0), ptr(nullptr){
}

template <typename T>
lct_lock_free_queue_t<T>::counted_node_ptr::~counted_node_ptr(){
	if(ptr != nullptr){
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
lct_lock_free_queue_t<T>::counted_node_ptr::counted_node_ptr(const lct_lock_free_queue_t<T>::counted_node_ptr& that):
	external_count(that.external_count), ptr(new node(*(that.ptr))){
}

template <typename T>
lct_lock_free_queue_t<T>::counted_node_ptr::counted_node_ptr(lct_lock_free_queue_t<T>::counted_node_ptr&& that):external_count(that.external_count), ptr(that.ptr){
	that.ptr = nullptr;
}

template <typename T>
typename lct_lock_free_queue_t<T>::counted_node_ptr& lct_lock_free_queue_t<T>::counted_node_ptr::operator=(const lct_lock_free_queue_t<T>::counted_node_ptr& that){
	if(this == &that){
		return *this;
	}
	external_count = that.external_count;
	if(ptr != nullptr){
		delete ptr;
	}
	ptr = new node(*(that.ptr));
	return *this;
}

template <typename T>
typename lct_lock_free_queue_t<T>::counted_node_ptr& lct_lock_free_queue_t<T>::counted_node_ptr::operator=(lct_lock_free_queue_t<T>::counted_node_ptr&& that){
	if(this == &that){
		return *this;
	}
	external_count = that.external_count;
	ptr = that.ptr;
	that.ptr = nullptr;
	return *this;
}

template <typename T>
lct_lock_free_queue_t<T>::lct_lock_free_queue_t() noexcept(true){
	counted_node_ptr	new_next;
	new_next.ptr				= new node;
	new_next.external_count		= 1;
	head.store(new_next);
	tail.store(new_next);
}

template <typename T>
lct_lock_free_queue_t<T>::~lct_lock_free_queue_t(){
	counted_node_ptr old_head=head.load(std::memory_order_relaxed);
	while(true){
		node* const ptr = old_head.ptr;
		if(ptr == tail.load().ptr){
			ptr->release_ref();
			break;
		}
		if(head.compare_exchange_strong(old_head, ptr->next)){
			T* const res = ptr->data.exchange(nullptr);
			free_external_counter(old_head);
			continue;
		}
		ptr->release_ref();
	}
}

template <typename T>
lct_lock_free_queue_t<T>::node::node(){
	node_counter new_count;
	new_count.internal_count	= 0;
	new_count.external_counters	= 2;
	count.store(new_count);

	next.ptr			= nullptr;
	next.external_count	= 0;
}

template <typename T>
void lct_lock_free_queue_t<T>::node::release_ref(){
	node_counter old_counter = count.load(std::memory_order_relaxed);
	node_counter new_counter;
	do{
		new_counter = old_counter;
		--new_counter.internal_count;
	}while(!count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));

	if(!new_counter.internal_count && !new_counter.external_counters){
		delete this;
	}
}
template <typename T>
void lct_lock_free_queue_t<T>::increase_external_count(std::atomic<counted_node_ptr>& counter, counted_node_ptr& old_counter){
	counted_node_ptr new_counter;
	do{
		new_counter = old_counter;
		++new_counter.external_count;
	}while(!counter.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));

	old_counter.external_count = new_counter.external_count;
}

template <typename T>
void lct_lock_free_queue_t<T>::free_external_counter(counted_node_ptr& old_node_ptr){
	node* const ptr = old_node_ptr.ptr;
	int const count_increase=old_node_ptr.external_count - 2;
	node_counter old_counter = ptr->count.load(std::memory_order_relaxed);

	node_counter new_counter;
	do{
		new_counter = old_counter;
		--new_counter.external_counters;
		new_counter.internal_count += count_increase;
	}while(!ptr->count.compare_exchange_strong(old_counter, new_counter, std::memory_order_acquire, std::memory_order_relaxed));

	if(!new_counter.internal_count && !new_counter.external_counters){
		delete ptr;
	}
}

template <typename T>
void lct_lock_free_queue_t<T>::push(const T& new_value){
	std::unique_ptr<T>	new_data(new T(new_value));
	counted_node_ptr	new_next;
	new_next.ptr				= new node;
	new_next.external_count		= 1;
	counted_node_ptr old_tail	= tail.load();

	while(true){
		increase_external_count(tail, old_tail);
		T* old_data = nullptr;
		if(old_tail.ptr->data.compare_exchange_strong(old_data, new_data.get())){
			old_tail.ptr->next = new_next;
			old_tail = tail.exchange(new_next);
			free_external_counter(old_tail);
			new_data.release();
			break;
		}
		old_tail.ptr->release_ref();
	}
}

template <typename T>
void lct_lock_free_queue_t<T>::push(T&& new_value){
	std::unique_ptr<T>	new_data(new T(new_value));
	counted_node_ptr	new_next;
	new_next.ptr				= new node;
	new_next.external_count		= 1;
	counted_node_ptr old_tail	= tail.load();

	while(true){
		increase_external_count(tail, old_tail);
		T* old_data = nullptr;
		if(old_tail.ptr->data.compare_exchange_strong(old_data, new_data.get())){
			old_tail.ptr->next = new_next;
			old_tail = tail.exchange(new_next);
			free_external_counter(old_tail);
			new_data.release();
			break;
		}
		old_tail.ptr->release_ref();
	}
}

template <typename T>
void lct_lock_free_queue_t<T>::push(T* new_value){
	std::unique_ptr<T>	new_data(new_value);
	counted_node_ptr	new_next;
	new_next.ptr				= new node;
	new_next.external_count		= 1;
	counted_node_ptr old_tail	= tail.load();

	while(true){
		increase_external_count(tail, old_tail);
		T* old_data = nullptr;
		if(old_tail.ptr->data.compare_exchange_strong(old_data, new_data.get())){
			old_tail.ptr->next = new_next;
			old_tail = tail.exchange(new_next);
			free_external_counter(old_tail);
			new_data.release();
			break;
		}
		old_tail.ptr->release_ref();
	}
}

template <typename T>
std::unique_ptr<T> lct_lock_free_queue_t<T>::pop(){
	counted_node_ptr old_head=head.load(std::memory_order_relaxed);
	while(true){
		node* const ptr = old_head.ptr;
		if(ptr == tail.load().ptr){
			//ptr->release_ref();
			return std::unique_ptr<T>();
		}
		if(head.compare_exchange_strong(old_head, ptr->next)){
			T* const res = ptr->data.exchange(nullptr);
			free_external_counter(old_head);
			return std::unique_ptr<T>(res);
		}
		ptr->release_ref();
	}
}

#endif /* SRC_COMMON_LCT_LOCK_FREE_QUEUE_IMPL_HPP_ */
