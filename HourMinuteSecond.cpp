#include "HourMinuteSecond.h"

HourMinuteSecond::HourMinuteSecond() {
	this->hours = 0;
	this->minutes = 0;
	this->seconds = 0;
}
HourMinuteSecond::HourMinuteSecond(int hours, int minutes, double seconds) {
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
	correctTime();
}

void HourMinuteSecond::addSeconds(double seconds) {
	this->seconds += seconds;
	correctTime();
}

void HourMinuteSecond::addMinutes(int minutes) {
	this->minutes += minutes;
	correctTime();
}

void HourMinuteSecond::addhours(int hours) {
	this->hours += hours;
	correctTime();
}

void HourMinuteSecond::setSeconds(double seconds) {
	this->seconds = seconds;
	correctTime();
}

void HourMinuteSecond::setMinutes(int minutes) {
	this->minutes = minutes;
	correctTime();
}

void HourMinuteSecond::setHours(int hours) {
	this->hours += hours;
	correctTime();
}

double HourMinuteSecond::getSeconds() {
	return seconds;
}

int HourMinuteSecond::getMinutes() {
	return minutes;
}

int HourMinuteSecond::getHours() {
	return hours;
}

std::string HourMinuteSecond::toString() {
	if (*this == NULL_TIME) {
		return "-";
	}
	if (hours != 0) {
		return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
	}
	else if (minutes != 0) {
		if (seconds < 10 && seconds > -10) {
			return std::to_string(minutes) + ":0" + std::to_string(seconds);
		}
		return std::to_string(minutes) + ":" + std::to_string(seconds);
	}
	return std::to_string(seconds);
}

void HourMinuteSecond::correctTime() {
	if (seconds >= seconds_in_minute) {
		minutes += seconds / seconds_in_minute;
		seconds = seconds - (int) (seconds / seconds_in_minute)  * seconds_in_minute;
		if (minutes >= minutes_in_hour) {
			hours += minutes / minutes_in_hour;
			minutes = (int)(minutes / minutes_in_hour) * minutes_in_hour;
		}
	}
}

HourMinuteSecond& HourMinuteSecond::operator=(const HourMinuteSecond& a) {
	this->seconds = a.seconds;
	this->hours = a.hours;
	this->minutes = a.minutes;

	return *this;
}

bool HourMinuteSecond::operator<(const HourMinuteSecond& a) {
	if (this->hours < a.hours) {
		return true;
	}
	else if (this->hours == a.hours) {
		if (this->minutes < a.minutes) {
			return true;
		}
		else if (this->minutes == a.minutes) {
			if (this->seconds < a.seconds) {
				return true;
			}
		}
	}

	return false;
}

bool HourMinuteSecond::operator==(const HourMinuteSecond& a) {
	if (this->hours == a.hours && this->minutes == a.minutes && this->seconds == a.seconds) {
		return true;
	}
	return false;
}

HourMinuteSecond& HourMinuteSecond::operator-(const HourMinuteSecond& a) {
	double seconds1 = this->seconds + this->minutes * 60 + this->hours * 3600;
	double seconds2 = a.seconds + a.minutes * 60 + a.hours * 3600;
	this->setHours(0);
	this->setMinutes(0);
	this->setSeconds(seconds1 - seconds2);

	return *this;
}

std::ostream& operator<<(std::ostream& os, const HourMinuteSecond& hms)
{
	return os << hms.hours << '\n' << hms.minutes << '\n' << hms.seconds << '\n';
}