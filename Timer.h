#pragma once
#include <chrono>
#include "HourMinuteSecond.h"
#include <thread>

class Timer
{
public:
	HourMinuteSecond time;

	std::chrono::steady_clock::time_point start_time;

	std::chrono::steady_clock::time_point end_time;

	bool going = false;

	Timer();

	Timer(HourMinuteSecond time);

	void start();

	void end();

	void update();

	void reset();

	HourMinuteSecond getTime();

	void setTime(HourMinuteSecond newTime);

};

