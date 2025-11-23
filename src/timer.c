#include "header/timer.h"
#include <stdlib.h>

struct timer* ctor_timer()
{
    struct timer* _ = (struct timer*)malloc(sizeof(struct timer));
    _->__timer = 0;

    return _;
}

void dtor_timer(struct timer* _this)
{
	if (_this) {
		free(_this);
	}
}

void start_timer(struct timer* _this)
{
	_this->__timer = clock();
}

double end_timer_ms(struct timer* _this)
{
	clock_t end = clock();
	return ((double)(end - _this->__timer) * 1000.0) / CLOCKS_PER_SEC;
}

double end_timer_second(struct timer* _this)
{
	clock_t end = clock();
	return (double)(end - _this->__timer) / CLOCKS_PER_SEC;
}
