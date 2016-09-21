/*
 * lct_memory_pool_base.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef SRC_COMMON_LCT_MEMORY_POOL_BASE_H_
#define SRC_COMMON_LCT_MEMORY_POOL_BASE_H_

class lct_memory_pool_base{
private:
	union free_chunk_list{
		union free_chunk_list*	next_free_chunk;
		char					client_data[1];		// for memory save and re-usage
	};
	constexpr static size_t lct_free_block_size			= 16;
	constexpr static size_t lct_chunk_align_size		= 8;
	constexpr static size_t lct_chunk_allocation_size	= 20;

	static free_chunk_list* lct_free_block_list[lct_free_block_size];

protected:
	void* get_free_chunk(const size_t obj_bytes);
	constexpr size_t get_max_chunk_size() const;
	void deallocate_chunk(void* obj_ptr, const size_t obj_bytes);

private:
	free_chunk_list* allocate_chunk(const size_t obj_bytes);
	free_chunk_list*& get_free_list(const size_t obj_bytes);
	size_t get_roundup_chunk_size(const size_t obj_bytes);
};

#endif /* SRC_COMMON_LCT_MEMORY_POOL_BASE_H_ */
