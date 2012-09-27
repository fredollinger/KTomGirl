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
	KTGSystray(void);
	~KTGSystray();
	void quit(void);
	void addNoteAction(QAction*, const QString&);

public slots:
	void slotQuit(void);
	// common code in all constructors
	void init(void);

private:
	QStringList m_qslNotes;

}; // class KTGSystray
} // namespace ktomgirl
#endif // _KTOMGIRL_SYS_TRAY_
// Sun May 13 15:55:55 PDT 2012
