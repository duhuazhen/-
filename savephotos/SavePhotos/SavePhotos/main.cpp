#include "SavePhotos.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SavePhotos w;
	w.show();
	return a.exec();
}
