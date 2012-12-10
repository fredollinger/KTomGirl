#include <qdebug.h>

#include "ktgsystray.h"

namespace ktomgirl{
KTGSystray::KTGSystray(QWidget* pParent, const char* szName) : KStatusNotifierItem(0) 

{
	init();
	return;
}

void KTGSystray::init(){
        setIconByPixmap(QIcon(":/icons/notebook.png"));
  	setStatus( KStatusNotifierItem::Active );
  	setCategory( KStatusNotifierItem::ApplicationStatus );
  	setStandardActionsEnabled(false);
  	setTitle(QString("KTomgirl"));
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
}

void KTGSystray::addNoteAction(QAction *act, const QString &uid){
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
		if ( m_actions.key(act) == qs){
			m_actions.remove(qs);
  			contextMenu()->removeAction(act);
			m_qslNotes.removeFirst();
			return;
		}
  	}
}

void KTGSystray::setItemName(const QString &neu, const QString &old){
	// Find the note that matches
	foreach ( QAction *act, m_actions ) {
		if (act->iconText() == old)
			act->setText(neu);
	}
}
} // namespace ktomgirl
// Thu Sep 27 12:54:57 PDT 2012
