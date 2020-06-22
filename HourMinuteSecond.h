#pragma once
#include <string> 
#include <iostream>
#include <boost/archive/tmpdir.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>
#define NULL_TIME HourMinuteSecond(0,0,-1)
#define DEF_TIME HourMinuteSecond(0,0,0)

class HourMinuteSecond
{
	friend std::ostream& operator<<(std::ostream& os, const HourMinuteSecond& hms);
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int /* file_version */) {
		ar & hours & minutes & seconds;
	}
	const unsigned int seconds_in_minute = 60;
	const unsigned int minutes_in_hour = 60;
	const unsigned int seconds_in_hour = seconds_in_minute * minutes_in_hour;
	int hours = 0;
	int minutes = 0;
	double seconds = 0;

public:
	
	HourMinuteSecond();

	HourMinuteSecond(int hours, int minutes, double seconds);
	
	void addSeconds(double seconds);

	void addMinutes(int minutes);

	void addhours(int hours);

	void setSeconds(double seconds);

	void setMinutes(int minutes);

	void setHours(int hours);

	double getSeconds();

	int getMinutes();

	int getHours();

	std::string toString();

	HourMinuteSecond& operator=(const HourMinuteSecond& a);

	bool operator<(const HourMinuteSecond& a);

	bool operator==(const HourMinuteSecond& a);

	HourMinuteSecond& operator-(const HourMinuteSecond& a);

private:
	void correctTime();
};

