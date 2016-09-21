/*
 * lct_memory_pool.h
 *
 *  Created on: Sep 21, 2016
 *      Author: Ralph.Liu
 *       Email: wlc2rhyme@gmail.com
 */

#ifndef SRC_COMMON_LCT_MEMORY_POOL_H_
#define SRC_COMMON_LCT_MEMORY_POOL_H_

class lct_memory_pool_base;

template <typename T>
class lct_memory_pool_rep final: protected lct_memory_pool_base{
private:
	typedef T			object_type;
	typedef T*			pointer_type;
public:
	pointer_type allocate(const size_t obj_bytes){
		if(obj_bytes == 0){
			return nullptr;
		}
		if(obj_bytes != sizeof(object_type) || obj_bytes > get_max_chunk_size()){
			return static_cast<pointer_type>(::operator new(obj_bytes));
		}

		return static_cast<pointer_type>(get_free_chunk(obj_bytes));
	}

	void deallocate(pointer_type obj_ptr, const size_t obj_bytes){
		if(obj_bytes == 0){
			return;
		}
		if(obj_bytes != sizeof(object_type) || obj_bytes > get_max_chunk_size()){
			::operator delete(obj_ptr);
			return;
		}
		deallocate_chunk(static_cast<void*>(obj_ptr), obj_bytes);
	}
};

template <typename T>
class lct_memory_pool{
private:
	typedef T			object_type;
	typedef T*			pointer_type;

public:
	static pointer_type allocate(const size_t obj_bytes){
		return m_lct_memory_pool_rep.allocate(obj_bytes);
	}

	static void deallocate(pointer_type obj_ptr, const size_t obj_bytes){
		m_lct_memory_pool_rep.deallocate(obj_ptr, obj_bytes);
	}

private:
	static lct_memory_pool_rep<object_type>			m_lct_memory_pool_rep;
};

template<typename T>
lct_memory_pool_rep<T> lct_memory_pool<T>::m_lct_memory_pool_rep;

#endif /* SRC_COMMON_LCT_MEMORY_POOL_H_ */
