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

void KTGSystray::addNoteAction(QAction *act, const QString &qs, const QString &uid){

	/* If we have all ready added the note to the list, don't add it again. */
	if (m_qslNotes.contains(qs)) return;
 
	// FIXME: IMPLEMENT THIS
	/* If we are 10 notes then we remove the oldest. */
	#if 0
	if (m_qslNotes.length() > 9){
	//	m_qslNotes.removeFirst();	
		//QList<QAction *> qla = contextMenu->actions() 
  	//	contextMenu()->removeItem(4);
	}
	#endif

	QVariant var = QVariant(uid);
	act->setData(var);
  	contextMenu()->addAction(act);
	m_qslNotes.append(qs);
	return;
}


} // namespace ktomgirl
