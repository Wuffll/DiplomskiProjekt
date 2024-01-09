#pragma once

#include "TimeControl.h"

class FpsManager
{
public:

	FpsManager();
	FpsManager(const unsigned int& fpsLimit);

	void SetFpsLimit(const unsigned int& fpsLimit);
	unsigned int GetFpsLimit() const;

	unsigned int GetCurrentFps() const;

	const TimeControl& GetTimer() const;

	void Begin();
	bool TimeToGo();

private:

	unsigned int mFpsLimit = 0;
	unsigned int mCurrentFps = 0;
	double mWaitTime;
	TimeControl mTimer{};

};