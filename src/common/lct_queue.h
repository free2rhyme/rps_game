
#ifndef __LCT_QUEUE_H__
#define __LCT_QUEUE_H__

#include <queue>
#include <limits>
#include <mutex>
#include <condition_variable>
#include "lct_common_define.h"
#include "lct_object_replicability.h"

template <typename T>
class lct_queue_t: lct_object_uncopyable{
private:
typedef std::mutex	lct_mutex_t;

private:
	typedef T		object_type;
	typedef T*		pointer_type;
public:
	explicit lct_queue_t(const lct_uint32_t waiting_time = m_infinite);
	~lct_queue_t();

public:
	lct_error_code_t enqueue(const object_type&);
	lct_error_code_t enqueue(object_type&&);
	lct_error_code_t dequeue(object_type&);

	lct_bool_t		empty() const;
	lct_uint32_t	size() const;

	void notify_all();
private:
	lct_uint32_t				m_waiting_time; // milliseconds
	std::condition_variable		m_cond_var;
	lct_mutex_t					m_mutex;

	constexpr static lct_uint32_t	m_infinite = std::numeric_limits<lct_uint32_t>::max();

	typename std::queue<T>		m_lct_queue_data;
};

#include "lct_queue_impl.hpp"

#endif



