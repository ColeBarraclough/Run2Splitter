#include "split.h"

split::split() {
	HourMinuteSecond h(0,0,0);
	this->setSplitTime(h);
	this->title = "";
	this->setBestTime(NULL_TIME);
	this->setBestRunTime(NULL_TIME);
}

split::split(HourMinuteSecond split_time_time, std::string title) {
	this->setSplitTime(split_time_time);
	this->title = title;
	this->setBestTime(NULL_TIME);
}

split::split(HourMinuteSecond split_time_time, std::string title, HourMinuteSecond best_time_time) {
	this->setSplitTime(split_time_time);
	this->title = title;
	this->setBestTime(best_time_time);
}

split::split(HourMinuteSecond split_time_time, std::string title, HourMinuteSecond best_time_time, HourMinuteSecond best_run_time_time) {
	this->setSplitTime(split_time_time);
	this->title = title;
	this->setBestTime(best_time_time);
	this->setBestRunTime(best_run_time_time);
}

std::string split::getTitle() {
	return title;
}

void split::setSplitTime(HourMinuteSecond newSplitTime) {
	split_time.setSeconds(newSplitTime.getSeconds());
	split_time.setMinutes(newSplitTime.getMinutes());
	split_time.setHours(newSplitTime.getHours());
}

void split::setTitle(std::string newTitle) {
	title = newTitle;
}

split& split::operator=(const split& a) {
	this->setSplitTime(a.split_time);
	this->title = a.title;
	return *this;
}

HourMinuteSecond split::getTime() {
	return split_time;
}

void split::setTime(HourMinuteSecond time) {
	this->split_time.setSeconds(time.getSeconds());
	this->split_time.setMinutes(time.getMinutes());
	this->split_time.setHours(time.getHours());
}

void split::update(double seconds) {
	split_time.addSeconds(seconds);
}

void split::setBestTime(HourMinuteSecond newBestTime) {
	best_time.setSeconds(newBestTime.getSeconds());
	best_time.setMinutes(newBestTime.getMinutes());
	best_time.setHours(newBestTime.getHours());
}

void split::setBestRunTime(HourMinuteSecond newBestRunTime) {
	best_run_time.setSeconds(newBestRunTime.getSeconds());
	best_run_time.setMinutes(newBestRunTime.getMinutes());
	best_run_time.setHours(newBestRunTime.getHours());
}

HourMinuteSecond split::getBestTime() {
	return best_time;
}

HourMinuteSecond split::getBestRunTime() {
	return best_run_time;
}

std::ostream& operator<<(std::ostream& os, const split& split) {
	return  os << split.title << '\n' << split.best_time << split.best_run_time;
}
