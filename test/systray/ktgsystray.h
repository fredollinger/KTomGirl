#ifndef _KTOMGIRL_SYS_TRAY_
#define _KTOMGIRL_SYS_TRAY_

#include <QIcon>

#include <KMenu>
#include <KStatusNotifierItem>

namespace ktomgirl{
class KTGSystray : public KStatusNotifierItem
{
	Q_OBJECT
public:
	KTGSystray(QWidget* pParent = NULL, const char* szName = NULL);
	~KTGSystray();
	KMenu *m_menu;
	void quit(void);

public slots:
	void slotQuit(void);

}; // class KTGSystray
} // namespace ktomgirl
#endif // _KTOMGIRL_SYS_TRAY_
// Sun May 13 15:55:55 PDT 2012
