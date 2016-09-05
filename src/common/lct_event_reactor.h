#ifndef __LCT_EVENT_REACTOR__H__
#define __LCT_EVENT_REACTOR__H__

#include "lct_common_define.h"
#include "lct_object_replicability.h"

class lct_event_reactor_t: lct_object_uncopyable{
public:
	lct_event_reactor_t();
	virtual ~lct_event_reactor_t();
	lct_error_code_t run(const lct_bool_t is_async = true);

protected:
	lct_error_code_t lct_event_reactor();
	lct_error_code_t init(const lct_bool_t kickoff_run_flag = false);

	virtual lct_error_code_t lct_execute() = 0;
	virtual lct_error_code_t shutdown();

protected:
	lct_bool_t		m_running_flag;
};

#endif
