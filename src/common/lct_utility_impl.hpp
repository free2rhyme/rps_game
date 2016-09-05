
template<typename T>
lct_uint32_t lct_identify_t<T>::next(){
	std::unique_lock<std::mutex> guard(m_mutex);
	return ++m_lct_auto_incre_id;
}

template<typename T>
lct_uint32_t lct_identify_t<T>::m_lct_auto_incre_id = 0;

template<typename T>
std::mutex lct_identify_t<T>::m_mutex;
