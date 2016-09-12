#ifndef __LCT_OBJECT_REPLICABILITY_H__
#define __LCT_OBJECT_REPLICABILITY_H__

class lct_object_cloneable{
public:
	~lct_object_cloneable() noexcept(true) {}
};

class lct_object_uncopyable{
public:
	lct_object_uncopyable() noexcept(true) {}

public:
	lct_object_uncopyable(const lct_object_uncopyable&)					= delete;
	lct_object_uncopyable(lct_object_uncopyable&&)						= delete;
	lct_object_uncopyable& operator = (const lct_object_uncopyable&)	= delete;
	lct_object_uncopyable& operator = (lct_object_uncopyable&&)			= delete;
	lct_object_uncopyable* operator & ()								= delete;
	const lct_object_uncopyable* operator & () const					= delete;
	virtual ~lct_object_uncopyable(){}
};

#endif


