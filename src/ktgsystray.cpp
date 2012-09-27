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
	qDebug() << __PRETTY_FUNCTION__ << "title: " << " uid: " << uid<< m_qslNotes.length();

	static bool first_run = true;

	if (first_run){
		qDebug() << "first run";
  		contextMenu()->addSeparator();
		first_run = false;
	}

	QString qs = act->text();

	/* If we have all ready added the note to the list, don't add it again. */
	if (m_qslNotes.contains(uid)) return;
 
	/* If we are 10 notes then we remove the oldest. */
	if (m_qslNotes.length() > 8){
		removeNoteAction(m_qslNotes.first());
	}

	QVariant var = QVariant(uid);
	act->setData(var);
  	contextMenu()->addAction(act);
	m_qslNotes.append(uid);
	m_actions.insert(uid, act);
	return;
}

void KTGSystray::removeNoteAction(const QString &qs){
	foreach ( QAction *act, m_actions ) {
		qDebug() << __PRETTY_FUNCTION__ << m_actions.key(act);
		if ( m_actions.key(act) == qs){
			m_actions.remove(qs);
  			contextMenu()->removeAction(act);
			m_qslNotes.removeFirst();
			return;
		}
  	}
}


} // namespace ktomgirl
