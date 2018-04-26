#include "MatlabCameratest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MatlabCameratest w;
	w.show();
	return a.exec();
}
