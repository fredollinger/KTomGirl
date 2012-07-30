#include <QApplication>
#include <QDebug>

#include <string>
#include <iostream>

// #include <libktomgirl/ktglib.hpp>

#include <QIcon>
#include <KMenu>
#include <KStatusNotifierItem>

#include "ktgsystray.h"
#include "stub.h"

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	// BEGIN KStatusNotifierItem
  	ktomgirl::KTGSystray *m_tray = new ktomgirl::KTGSystray();

	// BEGIN KMenu
	QAction *quitAct = new QAction("&Quit", m_tray);
	KMenu *m_menu = new KMenu("KTomGirl");
	m_menu->addAction(quitAct);

	QObject::connect(quitAct, SIGNAL(triggered()), m_tray, SLOT(slotQuit()));

	m_tray->setContextMenu(m_menu);
	// END KMenu

  	m_tray->activate();
	// END KStatusNotifierItem

	return app.exec();
}
// Sun Jul 29 08:01:30 PDT 2012
