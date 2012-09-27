#include <qdebug.h>

#include "ktgsystray.h"

namespace ktomgirl{
KTGSystray::KTGSystray(QWidget* pParent, const char* szName) : KStatusNotifierItem(0) 

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

void KTGSystray::addNoteAction(QAction *act, const QString &uid){
	qDebug() << __PRETTY_FUNCTION__ << "title: " << " uid: " << uid;

	static bool first_run = true;

	if (first_run){
		qDebug() << "first run";
  		contextMenu()->addSeparator();
		first_run = false;
	}

	QString qs = act->text();

	/* If we have all ready added the note to the list, don't add it again. */
	if (m_qslNotes.contains(uid)) return;
 
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
	m_qslNotes.append(uid);
	return;
}


} // namespace ktomgirl
