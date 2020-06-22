#pragma once
#include "Timer.h"

class split
{
	friend std::ostream& operator<<(std::ostream& os, const split& split);
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int /* file_version */) {
		ar & best_time;
		ar & best_run_time;
	}

	HourMinuteSecond split_time;

	HourMinuteSecond best_time;

	HourMinuteSecond best_run_time;

	std::string title;
public:


	split();

	split(HourMinuteSecond split_time_time, std::string title);

	split(HourMinuteSecond split_time_time, std::string title, HourMinuteSecond best_time_time);

	split(HourMinuteSecond split_time_time, std::string title, HourMinuteSecond best_time_time, HourMinuteSecond best_run_time_time);

	std::string getTitle();

	void setSplitTime(HourMinuteSecond newSplitTime);

	void setBestTime(HourMinuteSecond newBestTime);

	void setBestRunTime(HourMinuteSecond newBestRunTime);

	HourMinuteSecond getBestTime();

	HourMinuteSecond getBestRunTime();

	void setTitle(std::string newTitle);

	split& operator=(const split& a);

	HourMinuteSecond getTime();

	void update(double seconds);

	void setTime(HourMinuteSecond time);

	
};