/*
 * lct_memory_pool_base.cpp
 *
 *  Created on: Sep 21, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#include <cstddef>
#include <cstring>
#include <exception>
#include <new>
#include "lct_memory_pool_base.h"

void* lct_memory_pool_base::get_free_chunk(const size_t obj_bytes){
	free_chunk_list*& free_list = get_free_list(obj_bytes);
	if(free_list == nullptr){
		free_list = allocate_chunk(obj_bytes);
		//double check free_list == nullptr?
	}
	free_chunk_list* free_chunk = free_list;
	free_list = free_list->next_free_chunk;
	return static_cast<void*>(free_chunk);
}

constexpr size_t lct_memory_pool_base::get_max_chunk_size() const{
	return lct_chunk_align_size*lct_free_block_size;
}

void lct_memory_pool_base::deallocate_chunk(void* obj_ptr, const size_t obj_bytes){
	free_chunk_list* free_chunk = static_cast<free_chunk_list*>(obj_ptr);
	memset(obj_ptr, '\0', obj_bytes);

	free_chunk_list*& free_list = get_free_list(obj_bytes);
	if(free_list == nullptr){
		free_list = free_chunk;
		return;
	}
	free_chunk->next_free_chunk = free_list;
	free_list = free_chunk;
}

lct_memory_pool_base::free_chunk_list* lct_memory_pool_base::allocate_chunk(const size_t obj_bytes){
	const size_t fit_bytes = get_roundup_chunk_size(obj_bytes);
	const size_t total_bytes = fit_bytes * lct_chunk_allocation_size;
	lct_memory_pool_base::free_chunk_list* data_chunk_list = nullptr;
	try{
		data_chunk_list = static_cast<free_chunk_list*>(::operator new(total_bytes));
		throw std::exception();
	}catch(...){
		std::new_handler global_handler = std::set_new_handler(nullptr);
		std::set_new_handler(global_handler);
		if(global_handler != nullptr){
			(*global_handler)();
		}
	}
	memset(data_chunk_list, '\0', total_bytes);

	free_chunk_list* next_chunk = data_chunk_list;
	for(size_t index = 1; index < lct_chunk_allocation_size; ++index){
		free_chunk_list* new_chunk = next_chunk + fit_bytes;
		next_chunk->next_free_chunk = new_chunk;
	}
	return data_chunk_list;
}

lct_memory_pool_base::free_chunk_list*& lct_memory_pool_base::get_free_list(const size_t obj_bytes){
	size_t index = (obj_bytes + lct_chunk_align_size - 1) / lct_chunk_align_size - 1;
	return *(lct_free_block_list + index);
}

size_t lct_memory_pool_base::get_roundup_chunk_size(const size_t obj_bytes){
	size_t fit_bytes = (obj_bytes + lct_chunk_align_size - 1) & ~(lct_chunk_align_size - 1);
	return fit_bytes;
}

lct_memory_pool_base::free_chunk_list*	lct_memory_pool_base::lct_free_block_list[lct_free_block_size] = {
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};


