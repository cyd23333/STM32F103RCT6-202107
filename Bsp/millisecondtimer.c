#ifdef __cplusplus
extern "C" {
#endif

#include "millisecondtimer.h"
#define TickClock   72     // MHz
#define TickPeriod  50000   // us
volatile uint64_t _counter;

void initialise(void) 
{
	_counter = 0;
	SysTick_Config(TickClock * TickPeriod);
	NVIC_SetPriority (SysTick_IRQn,0);
}

// return system boot time in us
uint64_t time()
{
    uint64_t tick = _counter;
    uint64_t val = SysTick->VAL;

    if (tick != _counter) {
        tick = _counter;
        val = SysTick->VAL;
    }

    return (tick + 1) * TickPeriod - val / TickClock;
}
void delay(uint32_t millis) 
{
	uint32_t target;

	target = _counter + millis;
	while(_counter < target);
} 

void SysTick_Handler(void) 
{
	_counter++;
}

uint32_t millis(void)
{
	return _counter;
}

void usleep(uint32_t us)
{
    uint64_t timestamp = time();

    while (time() < timestamp + us);
}

void reset(void) 
{
	_counter = 0;
}

#ifdef __cplusplus
}
#endif

