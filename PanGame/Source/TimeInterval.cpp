#include "TimeInterval.h"

bool TimeInterval::hasITBeen(int seconds) {
	if (newTimer) {
		newTimer = false;
		start = std::chrono::steady_clock::now();
		return false;
	}
	else {
		end = std::chrono::steady_clock::now();
		if (get_duration() >= seconds) {
			newTimer = true;
			return true;
		}
		else {
			return false;
		}
	}
}


double TimeInterval::get_duration() {
	std::chrono::duration<double> duration{ end - start };
	return duration.count();

}