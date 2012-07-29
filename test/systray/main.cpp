#include <QApplication>
#include <QDebug>

#include <string>
#include <iostream>

// #include <libktomgirl/ktglib.hpp>

#include <QIcon>
#include <KMenu>
#include <KStatusNotifierItem>

#include "ktgsystray.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	// BEGIN KStatusNotifierItem
  	ktomgirl::KTGSystray *m_tray = new ktomgirl::KTGSystray();

  	m_tray->activate();
	// END KStatusNotifierItem

	return app.exec();
}
// Sun Jul 29 08:01:30 PDT 2012
