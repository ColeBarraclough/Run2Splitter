#include "Timer.h"

Timer::Timer() {
	this->time.setSeconds(0);
	this->time.setMinutes(0);
	this->time.setHours(0);
}

Timer::Timer(HourMinuteSecond time) {
	this->setTime(time);
}

void Timer::start() {
	start_time = std::chrono::steady_clock::now();
}

void Timer::end() {
	end_time = std::chrono::steady_clock::now();
}

void Timer::update() {
	end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
	start_time = end_time;
	time.addSeconds(elapsed_seconds.count());
}

void Timer::reset() {
	this->time.setSeconds(0);
	this->time.setMinutes(0);
	this->time.setHours(0);
}

HourMinuteSecond Timer::getTime() {
	return time;
}

void Timer::setTime(HourMinuteSecond time) {
	this->time.setSeconds(time.getSeconds());
	this->time.setMinutes(time.getMinutes());
	this->time.setHours(time.getHours());
}

