/*
 * stimer - Simple Elapsed Timer Library
 * Forked from the project https://github.com/ricardocrudo/stimer
 *
 * Copyright (c) 2016 Ricardo Crudo <ricardo.crudo@gmail.com>
 * Copyright (c) 2018 Ilhami KIVRAK <ilhami.kivrak@sinyalizasyon.com.tr>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "sElapsedTimer.h"
#include <stdlib.h>


struct sElapsedTimer_t {
    uint32_t counter;
    int enabled;
};

static sElapsedTimer_t g_timers[SELAPSEDTIMER_MAX_INSTANCES];

static inline sElapsedTimer_t* sElapsedTimer_take(void);
static inline void sElapsedTimer_give(sElapsedTimer_t *timer);


static inline sElapsedTimer_t* sElapsedTimer_take(void)
{
    static unsigned int isInitialized = 0;

    if (0 == isInitialized )
    {
        for(int i = 0; i < SELAPSEDTIMER_MAX_INSTANCES; ++i  )
        {
            g_timers[i].enabled = -1;
        }
        isInitialized = 1;
    }

    // iterate all array searching for a free spot
    // a timer is considered free when enabled is negative
    for (int i = 0; i < SELAPSEDTIMER_MAX_INSTANCES; i++)
    {
        sElapsedTimer_t *timer  = &g_timers[i];
        if (timer->enabled < 0)
            return timer;
    }

    return 0;
}

static inline void sElapsedTimer_give(sElapsedTimer_t *timer)
{
    if(NULL != timer) timer->enabled = -1;
}

sElapsedTimer_t* stimer_create(sElapsedTimer_t mode, void (*callback)(void *arg))
{
    sElapsedTimer_t *timer = sElapsedTimer_take();

    if (timer)
    {
        timer->enabled = 0;
    }

    return timer;
}

void stimer_destroy(sElapsedTimer_t *timer)
{
    sElapsedTimer_give(timer);
}

uint32_t stimer_get_time(sElapsedTimer_t *timer)
{
   return timer->counter;
}

void stimer_start(sElapsedTimer_t *timer)
{
    timer->enabled = 1;
}

uint32_t sElapsedTimer_stop(sElapsedTimer_t *timer)
{
	  uint32_t elapsedTime = timer->counter;
	
  	timer->enabled = 0;
	  timer->counter = 0;
	
		return elapsedTime;
	  
}

uint32_t sElapsedTimer_restart(sElapsedTimer_t *timer)
{
    uint32_t elapsedTime = timer->counter;
	
  	timer->enabled = 1;
	  timer->counter = 0;
	
		return elapsedTime;
}

void stimer_tick(void)
{
    for (int i = 0; i < SELAPSEDTIMER_MAX_INSTANCES; i++)
    {
        sElapsedTimer_t *timer = &g_timers[i];

        if (!timer->enabled)
            continue;

				timer->counter++;
    }
}

/*EOF*/
