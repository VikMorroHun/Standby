/*
Resource Links:
CPU Load Percent (Credit to Jeremy Friesner):           https://stackoverflow.com/questions/23143693/retrieving-cpu-load-percent-total-in-windows-with-c
Everything else (too many to list):                     https://stackoverflow.com/, https://docs.microsoft.com, https://doc.qt.io
*/

#include "TestCPUThread.h"

TestCPUThread::TestCPUThread()
{
	counter = 0;bTest = false;
}

void TestCPUThread::threadStart()
{
	counter = 0;bTest = true;
	this->start();
}

void TestCPUThread::threadStop()
{
	if (this->isRunning())
		bTest = false;
		//counter = 5000;		//exit() nem jó
}

void TestCPUThread::run()
{
	float f;
	while (bTest)
	{
		f = GetCPULoad() * 100;
		if (f < 0.0)
		{
			emit sendMessage(tr("Internal error - You should not see this."));
		}
		else
		{
			emit sendLCDValue( (int) f);this->msleep(200);
		}
	}
}

float TestCPUThread::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
	static unsigned long long _previousTotalTicks = 0;
	static unsigned long long _previousIdleTicks = 0;

	unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
	unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;


	float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);

	_previousTotalTicks = totalTicks;
	_previousIdleTicks = idleTicks;
	return ret;
}

unsigned long long TestCPUThread::FileTimeToInt64(const FILETIME & ft)
{
	return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
float TestCPUThread::GetCPULoad()
{
	FILETIME idleTime, kernelTime, userTime;
	return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}
