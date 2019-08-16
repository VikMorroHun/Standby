/***************************************************************************
 *   Copyright (C) 2019 by Viktor Dózsa                                    *
 *   luke2135@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 3 of the License.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses>.   *
 ***************************************************************************/

#include "GetCPUWidget.h"
#include "ui_GetCPUWidget.h"

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);ui->lineEdit->clear();this->setMouseTracking(true);
	pThreadCPU = new TestCPUThread();ui->lineEdit->setReadOnly(true);
	if (!pThreadCPU)
	{
		QMessageBox::information(this, tr("Error"), tr("Can't start new thread to measure CPU load on this system. Fatal error."));
		close();
	}
	sleepProcess = new QProcess(this);
	if (!sleepProcess)
	{
		QMessageBox::information(this, tr("Error"), tr("Can't start new process... Sleep mode disabled."));bSleepOff = true;
	}
	connect(this, SIGNAL(startTest()), pThreadCPU, SLOT(threadStart()), Qt::QueuedConnection);bSleepOff = false;
	connect(this, SIGNAL(stopTest()), pThreadCPU, SLOT(threadStop()), Qt::QueuedConnection);
	connect(pThreadCPU, SIGNAL(sendMessage(QString)), this, SLOT(thread_receiveMessage(QString)), Qt::QueuedConnection);
	connect(pThreadCPU, SIGNAL(sendLCDValue(int)), this, SLOT(thread_receiveLCDValue(int)), Qt::QueuedConnection);
	connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBox_valueChanged(int)), Qt::QueuedConnection);
	ui->lcdNumber->setStyleSheet("background: black");setWindowTitle(tr("Are we going to Standby?"));
	ui->horizontalSlider->setStyleSheet("background: black");ui->horizontalSlider_2->setStyleSheet("background: black");
	qi64MsecondsPassed = qi64Msec = key = 0;emit startTest();timer.start();iCountdown = ui->spinBox->value();ui->exitButton->setFocusPolicy(Qt::StrongFocus);
	ui->lineEdit->setFocus();
}

void Widget::sleep( int millisecondsToWait )
{
	QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
	while( QTime::currentTime() < dieTime )
	{
		QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
	}
}
Widget::~Widget()
{
	delete ui;
}

void Widget::on_exitButton_clicked()
{
	if (sleepProcess->state() == QProcess::Running)
		sleepProcess->kill();				//if it's running, we won't arrive here anyway
	delete sleepProcess;
	if (pThreadCPU->isRunning())
	{
		emit stopTest();
		while (pThreadCPU->isRunning())
			sleep(250);
		delete pThreadCPU;
		close();
	}
	else
	{
		delete pThreadCPU;
		close();
	}
}

void Widget::thread_receiveMessage(QString s)
{
	ui->lineEdit->setText(s);
}

void Widget::thread_receiveLCDValue(int iVal)
{
	if (iVal < 25)
		ui->lcdNumber->setStyleSheet("background: black; color: lightgray");
	else if (iVal < 50)
		ui->lcdNumber->setStyleSheet("background: black; color: lightblue");
	else if (iVal < 75)
		ui->lcdNumber->setStyleSheet("background: black; color: lightgreen");
	else
		ui->lcdNumber->setStyleSheet("background: black; color: red");
	ui->lcdNumber->display(iVal);
	if (iVal >= ui->spinBoxCPUThreshold->value())
	{
		iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
	}
	if (iVal < ui->spinBoxCPUThreshold->value())
	{
		qi64Msec = timer.elapsed();
		if ( qi64Msec > qi64MsecondsPassed + 1000)
		{
			qi64MsecondsPassed = qi64Msec;
			if (iCountdown > 0)
			{
				if (iCountdown == 1)
					ui->lineEdit->setText(tr("Entering standy in %1 second.").arg(iCountdown));
				else
					ui->lineEdit->setText(tr("Entering standy in %1 seconds.").arg(iCountdown));
				iCountdown--;
			}
			else if (!bSleepOff)
				{
					args.clear();iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
					//sleepProcess->start("sleep.bat", args);
					args << "powrprof.dll" << ",SetSuspendState" << "0,1,0";sleepProcess->start("rundll32.exe", args);
				}
		}
	}
}

void Widget::spinBox_valueChanged(int iVal)
{
	//qDebug() << "new Value: " << iVal;
	iCountdown = iVal;ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
	//ui->lineEdit->setText("mouse moved!");
	iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
	//ui->lineEdit->setText("mouse pressed!");
	iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != key)
	{
		key = event->key();
		iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
	}
	if (ui->exitButton->hasFocus() && ( key == Qt::Key_Enter || Qt::Key_Return ))
		emit ui->exitButton->clicked();
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
	key = 0;
	iCountdown = ui->spinBox->value();ui->lineEdit->clear();timer.restart();qi64MsecondsPassed = qi64Msec = 0;
}
