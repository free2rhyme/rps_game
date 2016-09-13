

template <typename T>
lct_queue_t<T>::lct_queue_t(const lct_uint32_t waiting_time):m_waiting_time(waiting_time){
}

template <typename T>
lct_queue_t<T>::~lct_queue_t(){
}

template <typename T>
lct_error_code_t lct_queue_t<T>::enqueue(const object_type& obj){
	bool is_empty = false;
	{
		std::unique_lock<lct_mutex_t> guard(m_mutex);
		is_empty = m_lct_queue_data.empty();
		m_lct_queue_data.push(obj);
		m_cond_var.notify_one();
	}
	return lct_error_code_t::lct_error_code_successful;
}

template <typename T>
lct_error_code_t lct_queue_t<T>::enqueue(object_type&& obj){
	bool is_empty = false;
	{
		std::unique_lock<lct_mutex_t> guard(m_mutex);
		is_empty = m_lct_queue_data.empty();
		m_lct_queue_data.push(obj);
	}
	if(is_empty){
		m_cond_var.notify_one();
	}
	return lct_error_code_t::lct_error_code_successful;
}

template <typename T>
lct_error_code_t lct_queue_t<T>::dequeue(object_type& obj){
	std::unique_lock<lct_mutex_t> guard(m_mutex);
	while(empty()){
		if(m_waiting_time == 0){
			return lct_error_code_t::lct_error_code_general_failure;
		} else if(m_waiting_time == m_infinite){
			m_cond_var.wait(guard);
		} else {
			if(m_cond_var.wait_for(guard, std::chrono::milliseconds(m_waiting_time)) == std::cv_status::timeout){
				return lct_error_code_t::lct_error_code_general_failure;
			}
			if(empty()){
				return lct_error_code_t::lct_error_code_general_failure;
			}
		}
	}
	obj = m_lct_queue_data.front();
	m_lct_queue_data.pop();
	return lct_error_code_t::lct_error_code_successful;
}

template <typename T>
lct_bool_t lct_queue_t<T>::empty() const{
	return m_lct_queue_data.empty();
}

template <typename T>
lct_uint32_t lct_queue_t<T>::size() const{
	return m_lct_queue_data.size();
}

template <typename T>
void lct_queue_t<T>::notify_all(){
	m_cond_var.notify_all();
}
