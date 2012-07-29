#include <QApplication>
#include <QDebug>

#include <string>
#include <iostream>

// #include <libktomgirl/ktglib.hpp>

#include <QIcon>

#include <KStatusNotifierItem>


int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

  	KStatusNotifierItem *m_tray = new KStatusNotifierItem(0);

  	m_tray->setIconByPixmap(QIcon("../../src/ui/notebook.png"));

  	m_tray->setStatus( KStatusNotifierItem::Active );
  	m_tray->setCategory( KStatusNotifierItem::ApplicationStatus );
  	m_tray->setStandardActionsEnabled(false);
  	m_tray->activate();

	return app.exec();
}
// Sun Jul 29 08:01:30 PDT 2012
