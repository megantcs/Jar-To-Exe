#ifndef TIMER_H
#define TIMER_H

#include <time.h>

struct timer 
{
	clock_t __timer;
};

struct timer* ctor_timer();
void		  dtor_timer(struct timer* _this);

void		  start_timer(struct timer* _this);
double		  end_timer_ms(struct timer* _this);
double		  end_timer_second(struct timer* _this);

#endif