#include "TimeControl.h"

#include "Debug.h"

TimeControl::TimeControl()
{
}

void TimeControl::Start()
{
	mStarted = true;
	mStart = Clock::now();
}

double TimeControl::End()
{
	if (!mStarted)
		return 0.0;

	mEnd = Clock::now();

	return DurationInSeconds();
}

void TimeControl::Stop()
{
	mStarted = false;
}

const bool& TimeControl::IsStarted() const
{
	return mStarted;
}

double TimeControl::DurationInSeconds() const
{
	if (!mStarted)
		return 0.0;

	double duration = ((std::chrono::duration_cast<std::chrono::nanoseconds>)(mEnd - mStart)).count() / 1000000000.0;
	// Debug::Print("Duration = " + STRING(duration));

	return (duration >= 0.0) ? duration : 0.0;
}
