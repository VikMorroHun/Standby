#ifndef TESTCPUTHREAD_H
#define TESTCPUTHREAD_H

#include <QObject>
#include <QThread>
//#include <QDebug>
#include <windows.h>

class TestCPUThread : public QThread
{
	Q_OBJECT

public:
	TestCPUThread();
	void run();
	float GetCPULoad();
	float CalculateCPULoad(unsigned long long, unsigned long long);
	unsigned long long FileTimeToInt64(const FILETIME &);

private:
	int counter;
	bool bTest;

signals:
	void startProgress();
	void sendMessage(QString s);
	void sendLCDValue(int);

private slots:
	void threadStart();
	void threadStop();

};

#endif // TESTCPUTHREAD_H
