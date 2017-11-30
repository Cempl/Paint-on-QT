#include "MainWindow.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	a.addLibraryPath("platforms");

	Paint::MainWindow w;
	w.show();

	return a.exec();
}
