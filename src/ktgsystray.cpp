#include <qdebug.h>

#include "ktgsystray.h"

namespace ktomgirl{
KTGSystray::KTGSystray(QWidget* pParent, const char* szName) : KStatusNotifierItem(0)
{

	// BEGIN KStatusNotifierItem
    setIconByPixmap(QIcon(":/icons/notebook.png"));
  	setStatus( KStatusNotifierItem::Active );
  	setCategory( KStatusNotifierItem::ApplicationStatus );
  	setStandardActionsEnabled(false);
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
