#pragma once
#include <chrono>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	void Initalize();
	void Mark(); // Calculate timeSpan and mark last = now
	float GetTimeSpan();

private:
	std::chrono::steady_clock::time_point last; // Hold the time from the last time call GetTime()
	float timeSpan; // time span from 2 times call Mark()
};