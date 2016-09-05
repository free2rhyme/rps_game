#ifndef __LCT_HASH_MAP_H__
#define __LCT_HASH_MAP_H__

#include <unordered_map>
#include <mutex>
#define lct_hash_map_t std::unordered_map

/**
template <typename key, typename value, typename compare = std::less<key> >
class lct_hash_map_t{
private:
	typedef typename key		key_type;
	typedef typename key*		key_pointer_type;
	typedef typename value		value_type;
	typedef typename value*		value_pointer_type;
	typedef typename std::unordered_map<key, value, compare>	lct_hash_map_type;
public:
	lct_hash_map_t();
	~lct_hash_map_t();

private:
	lct_bool_t		add(const key&, const value&);
	lct_bool_t		del(const key&);
	lct_bool_t		update(const key&, const value&);
	value_type		find(const key&);
	lct_bool_t		is_inmap(const key&);
	lct_uint32_t	size() const;
	lct_bool_t		empty() const;

private:
	lct_hash_map_type	m_lct_hash_map;
};


**/


#endif
