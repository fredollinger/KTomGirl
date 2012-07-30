#include <qdebug.h>

#include "ktgsystray.h"

namespace ktomgirl{
KTGSystray::KTGSystray(QWidget* pParent, const char* szName) : KStatusNotifierItem(0)
{
	// BEGIN KMenu
	QAction *quitAct = new QAction("&Quit", this);
	KMenu *m_menu = new KMenu("KTomGirl");
	m_menu->addAction(quitAct);
	connect(quitAct, SIGNAL(triggered()), this, SLOT(slotQuit()));
	// END KMenu

	// BEGIN KStatusNotifierItem
	setIconByPixmap(QIcon("../../src/ui/notebook.png"));
  	setStatus( KStatusNotifierItem::Active );
  	setCategory( KStatusNotifierItem::ApplicationStatus );
  	setStandardActionsEnabled(false);
	setContextMenu(m_menu);
  	setTitle(QString("KTomgirl"));
	// END KStatusNotifierItem
	return;
}

KTGSystray::~KTGSystray()
{
}

void KTGSystray::slotQuit(){
	qDebug() << __PRETTY_FUNCTION__;
}

} // namespace ktomgirl
