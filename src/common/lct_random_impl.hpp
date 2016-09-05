#include <limits>

template<typename T>
lct_random_t<T>::lct_random_t():
	m_lct_min(std::numeric_limits<lct_int32_t>::min()),
	m_lct_max(std::numeric_limits<lct_int32_t>::max()),
	m_lct_random_device(),
	m_lct_engine(m_lct_random_device()),
	m_lct_uid(m_lct_min,m_lct_max){
}

template<typename T>
lct_random_t<T>::lct_random_t(const lct_int32_t min, const lct_int32_t max):
	m_lct_min(min),
	m_lct_max(max),
	m_lct_random_device(),
	m_lct_engine(m_lct_random_device()),
	m_lct_uid(m_lct_min,m_lct_max){
}

template<typename T>
lct_random_t<T>::~lct_random_t(){
}

template<typename T>
typename lct_random_t<T>::object_type lct_random_t<T>::random(){
	return m_lct_uid(m_lct_engine);
}
