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
	void removeNoteAction(const QString&);
	void init(void);

public slots:
	void slotQuit(void);
	void setItemName(const QString&, const QString&);

private:
	QStringList m_qslNotes; // list of the notes uids
    	QMap<QString, QAction*> m_actions; // uid, qaction for quick look up

}; // class KTGSystray
} // namespace ktomgirl
#endif // _KTOMGIRL_SYS_TRAY_
// Sun May 13 15:55:55 PDT 2012
