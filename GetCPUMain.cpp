#include "GetCPUWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator myappTranslator;
	myappTranslator.load("GetCPULoad_" + QLocale::system().name());
	a.installTranslator(&myappTranslator);
	Widget w;
	w.show();

	return a.exec();
}
