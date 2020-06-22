#include "splitter.h"


splitter::splitter() {
	HourMinuteSecond h(0, 0, 0);
	main_split.setSplitTime(h);
	main_split.setTitle("");
}

splitter::splitter(split mSplit, std::vector<split> sSplits) {
	this->setMainSplit(mSplit);
	this->setSubSplits(sSplits);
	index = 0;
}

splitter::~splitter()
{
	sub_splits.clear();

}

void splitter::addSubSplit(split subSplit, int index) {
	sub_splits.push_back(subSplit);
	split temp = subSplit;
	split temp2;
	sub_splits[index] = subSplit;
	for (int i = index; i < sub_splits.size(); ++i) {
		temp2 = sub_splits[index];
		sub_splits[index] = temp;
		temp = temp2;
	}
}

void splitter::appendSubSplit(split subSplit) {
	sub_splits.push_back(subSplit);
}

void splitter::setMainSplit(split mSplit) {
	main_split.setTitle(mSplit.getTitle());
	main_split.setTime(mSplit.getTime());
}

void splitter::setSubSplits(std::vector<split> sSplits) {
	sub_splits = sSplits;
}

split& splitter::getMainSplit() {
	return main_split;
}

std::vector<split> splitter::getSubSplits() {
	return sub_splits;
}


void splitter::start() {
	going = true;
	start_time = std::chrono::steady_clock::now();
	update();
}

void splitter::end() {
	going = false;
}

void splitter::update() {
	if (!going) {
		return;
	}
	end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
	start_time = end_time;
	main_split.update(elapsed_seconds.count());
	sub_splits[index].update(elapsed_seconds.count());
}

void splitter::reset() {
	HourMinuteSecond h(0, 0, 0);
	main_split.setSplitTime(h);
	for (int i = 0; i < sub_splits.size(); i++) {
		sub_splits[i].setSplitTime(h);
	}
	index = 0;
}

HourMinuteSecond splitter::doSplit() {
	if (!going) {
		return sub_splits[index].getTime();
	}
	HourMinuteSecond h(0, 0, 0);
	h = main_split.getTime();
	if (sub_splits[index].getTime() < sub_splits[index].getBestTime() || sub_splits[index].getBestTime() == NULL_TIME) {
		sub_splits[index].setBestTime(sub_splits[index].getTime());
	}
	sub_splits[index].setBestRunTime(main_split.getTime());
	if (index == (sub_splits.size() - 1)) {
		end();
	}
	else {
		index++;
	}
	return h;

}

split& splitter::getCurrentSplit() {
	return sub_splits[index];
}

split& splitter::getSplit(int index) {
	return sub_splits[index];
}

bool splitter::getGoing() {
	return going;
}

int splitter::getIndex() {
	return index;
}

HourMinuteSecond splitter::getCurrentStanding() {
	if (sub_splits[index].getBestRunTime() == NULL_TIME) {
		return main_split.getTime();
	}
	return main_split.getTime() - sub_splits[index].getBestRunTime();
}

std::ostream& operator<<(std::ostream& os, const splitter& splitter) {
	for (int i = 0; i < static_cast<int>(splitter.sub_splits.size()); i++) {
		os << splitter.sub_splits[i];
	}
	return os;
}