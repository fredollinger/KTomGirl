#include <qdebug.h>

#include "ktgsystray.h"

namespace ktomgirl{
KTGSystray::KTGSystray(QWidget* pParent, const char* szName) : KStatusNotifierItem(0) 
	, m_menu()

{
	init();
	return;
}

void KTGSystray::init(){
	// BEGIN KStatusNotifierItem
        setIconByPixmap(QIcon(":/icons/notebook.png"));
  	setStatus( KStatusNotifierItem::Active );
  	setCategory( KStatusNotifierItem::ApplicationStatus );
  	setStandardActionsEnabled(false);
  	setTitle(QString("KTomgirl"));
	// END KStatusNotifierItem
	return;
} 


KTGSystray::KTGSystray() : KStatusNotifierItem(0) 
	, m_menu()

{
	init();
	return;
}

KTGSystray::~KTGSystray()
{
}

void KTGSystray::slotQuit(){
	qDebug() << __PRETTY_FUNCTION__;
}
/*
void KTGSystray::setNoteMenu(KTGMenu *menu){
	setContextMenu(menu);
	m_menu = menu;
}
*/

KTGMenu* KTGSystray::noteMenu(){
	return m_menu;
}

} // namespace ktomgirl
