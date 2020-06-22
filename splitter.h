#pragma once
#include "Timer.h"
#include "split.h"
#include <vector>
#include "iterator.h"
#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


class splitter
{
	friend std::ostream& operator<<(std::ostream& os, const splitter& splitter);
	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		// note, version is always the latest when saving
		ar & sub_splits;
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		if (version > 0)
			ar & sub_splits;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	split main_split;
	std::vector<split> sub_splits;
	int index;
	std::chrono::steady_clock::time_point start_time;

	std::chrono::steady_clock::time_point end_time;

	bool going = false;
	HourMinuteSecond split_time;
public:
	splitter();

	splitter(split mSplit, std::vector<split> sSplits);

	~splitter();

	void addSubSplit(split subSplit, int index);

	void appendSubSplit(split subSplit);

	void setMainSplit(split mSplit);

	void setSubSplits(std::vector<split> sSplits);

	void start();

	void end();

	void update();

	void reset();

	split& getMainSplit();

	split& getCurrentSplit();

	split& getSplit(int index);

	bool getGoing();

	int getIndex();

	HourMinuteSecond getCurrentStanding();

	std::vector<split> getSubSplits();

	HourMinuteSecond doSplit();
};

