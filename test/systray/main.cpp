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

  	ktomgirl::Stub *m_stub = new ktomgirl::Stub();

	// BEGIN KMenu
	KMenu *m_menu = new KMenu("KTomGirl");
	m_menu->addAction(quitAct);
	// END KMenu

	QAction *searchAct = new QAction("&Search all notes", m_tray);
	QObject::connect(searchAct, SIGNAL(triggered()), m_stub, SLOT(slotSearch()));

	QAction *createAct = new QAction("&Create new note", m_tray);
	QObject::connect(createAct, SIGNAL(triggered()), m_stub, SLOT(slotCreateNewNote()));

	QAction *quitAct = new QAction("&Quit", m_tray);
	QObject::connect(quitAct, SIGNAL(triggered()), m_stub, SLOT(slotQuit()));

	m_tray->setContextMenu(m_menu);
  	m_tray->activate();
	// END KStatusNotifierItem

	return app.exec();
}
// Sun Jul 29 08:01:30 PDT 2012
