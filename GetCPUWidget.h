#ifndef GETCPUWIDGET_H
#define GETCPUWIDGET_H

#include <QWidget>
#include <QTime>
#include <QString>
#include <QElapsedTimer>
#include <QProcess>
#include <QStringList>
#include <QMessageBox>
#include <QShortcut>
#include <QTranslator>
#include <QMouseEvent>
#include <QKeyEvent>
//#include <QDebug>
#include <testcputhread.h>
//#include <pdh.h>	//Performance Data Helper interface

//#define MB 1048576

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

signals:
	void startTest();
	void stopTest();

public:
	explicit Widget(QWidget *parent = 0);
	void sleep( int );
	TestCPUThread * pThreadCPU;
	QProcess * sleepProcess;
	~Widget();

private slots:
	void on_exitButton_clicked();

	void thread_receiveMessage(QString s);

	void thread_receiveLCDValue(int iVal);

	void spinBox_valueChanged(int iVal);

	void mouseMoveEvent(QMouseEvent *);

	void mousePressEvent(QMouseEvent *);

	void keyPressEvent(QKeyEvent *);

	void keyReleaseEvent(QKeyEvent *);

private:
	Ui::Widget *ui;
	int iCountdown, key;
	qint64 qi64MsecondsPassed, qi64Msec;
	QElapsedTimer timer;
	QStringList args;
	bool bSleepOff;
};

#endif // GETCPUWIDGET_H
