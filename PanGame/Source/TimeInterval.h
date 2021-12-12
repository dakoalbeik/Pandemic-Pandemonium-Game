#pragma once
#include <chrono>

class TimeInterval {
public:
	bool hasITBeen(int seconds);

private:
	double get_duration();

	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
	bool newTimer{ true };
};


