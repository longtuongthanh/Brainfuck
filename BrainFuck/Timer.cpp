#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	
}

Timer::Timer(const Timer& timer)
{
	last = timer.last;
	timeSpan = timer.timeSpan;
}

Timer::~Timer()
{
}

void Timer::Initalize()
{
	last = steady_clock::now();
	timeSpan = 0.0f;
}

void Timer::Mark()
{
	steady_clock::time_point now = steady_clock::now();
	timeSpan = std::chrono::duration<float>(now - last).count(); // calculate timeSpan
	last = now;
}

float Timer::GetTimeSpan()
{
	return timeSpan;
}
