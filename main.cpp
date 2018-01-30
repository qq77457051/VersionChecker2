#include "VersionChecker2.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VersionChecker2 w;
	w.show();
	w.setMouseTracking(true);
	return a.exec();
}
