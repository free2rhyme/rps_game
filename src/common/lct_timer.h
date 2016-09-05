#ifndef __LCT_TIMER_H__
#define __LCT_TIMER_H__

#include <functional>
#include <chrono>
#include <future>

class lct_timer_t{
public:
    template <class callable,  class... arguments>
    lct_timer_t(int duration_milliseconds, lct_bool_t is_async, callable&& func, arguments&&... vargs){
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(func), std::forward<arguments>(vargs)...));

        const std::chrono::milliseconds durn(duration_milliseconds);
        if(is_async){
            std::thread([duration_milliseconds, task, durn]() {
            	std::this_thread::sleep_for(durn);
                task();
            }).detach();
        } else {
            std::this_thread::sleep_for(durn);
            task();
        }
    }
};

#endif

