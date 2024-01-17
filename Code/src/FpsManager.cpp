#include "FpsManager.h"

FpsManager::FpsManager()
	:
	mFpsLimit(61),
	mWaitTime(1.0 / mFpsLimit)
{
}

FpsManager::FpsManager(const unsigned int& fpsLimit)
	:
	mFpsLimit(fpsLimit + 1),
	mWaitTime(1.0 / (fpsLimit + 1))
{
}

void FpsManager::SetFpsLimit(const unsigned int& fpsLimit)
{
	mFpsLimit = fpsLimit + 1;
	mWaitTime = 1.0 / mFpsLimit;
}

unsigned int FpsManager::GetFpsLimit() const
{
	return mFpsLimit;
}

unsigned int FpsManager::GetCurrentFps() const
{
	return mCurrentFps;
}

const TimeControl& FpsManager::GetTimer() const
{
	return mTimer;
}

void FpsManager::Begin()
{
	mTimer.Start();
}

bool FpsManager::TimeToGo()
{
	if(mTimer.End() < mWaitTime)
		return false;
	
	mCurrentFps = 1.0 / mTimer.End();

	return true;
}
