/*******************************************************************
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2006, The KNotes Developers

 2012-2015 Modified by Fred Ollinger <follinge@gmail.com> for KTomGirl

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************/

#ifndef KNOTESAPP_H
#define KNOTESAPP_H

#include <QDomDocument>
#include <QList>
#include <QMap>
#include <QString>
#include <QWidget>

#include <kapplication.h>
#include <ksessionmanager.h>
#include <kxmlguiclient.h>

#include "ktgsystray.h"
#include "searchwindow.h"

#include <libktomgirl/notemanager.hpp>

class QTcpServer;

class KAction;
class KFind;
class KMenu;
class KNote;
class KNotesAlarm;
class KNotesResourceManager;
class KXMLGUIBuilder;
class KXMLGUIFactory;
class KStatusNotifierItem;

namespace ktomgirl{
	class KTGConfig;
	class KTGItem;
}

namespace ktomgirl {
class Journal;
}

namespace DNSSD {
class PublicService;
}

namespace knotes{
// BEGIN class KNotesApp
class KNotesApp
  : public QWidget, public KSessionManager, virtual public KXMLGUIClient
{
  Q_OBJECT
  public:
    KNotesApp();
    ~KNotesApp();

    void showNote( const QString &id ) const;
    void hideNote( const QString &id ) const;

    QString name( const QString &id ) const;
    QString text( const QString &id ) const;

    void setName( const QString &id, const QString &newName );
    void setText( const QString &id, const QString &newText );

    QVariantMap notes() const;

  public slots:
    void showAllNotes(void);
    void showFilteredNotes(const QString&);
    void slotNameChanged(const QString &neu, const QString &old, const QString &uuid);
    void showUnFiledNotes(void);

	  // Here we sync up the new notebook
		void slotUpdateNotebook(const QString&);

	  void slotNewNotebook(const QString&);
    void slotHandleSearch(QString);
    void slotCloseNote(const QString&);


    void hideAllNotes() const;
    void openNote(ktomgirl::KTGItem*);
    void showAllNotes() const;
    void slotDeleteNote(const QString&, const QString&);
    void slotShowSearchWindow(void);
    void slotSpewOpenNotes(void);

  protected slots:
    void slotActivateRequested( bool, const QPoint& pos);
    void slotShowNote();
    void slotWalkThroughNotes();
    void slotConfigureAccels();

    void slotNoteKilled( ktomgirl::Journal *journal );

    void slotQuit();

  private slots:
    void acceptConnection();
    void saveNotes();
    void saveNotes( const QString & uid );
    void updateNoteActions();
    void updateNetworkListener();

    void createNote();
    void createNote( ktomgirl::Journal *journal );
    void killNote( ktomgirl::Journal *journal );

// BEGIN KNotesApp private:
  private:
    /* noteInit():  Common code for createNote() and newNote() */
    void noteInit( KNote*);
    void showNote( KNote *note ) const;
    void saveConfigs();
    gnote::NoteManager *m_gnmanager;
    int m_noteNumber;
    SearchWindow *m_searchWindow;
    KMenu *m_sys_menu; // system tray menu
    QMap<QString, KNote *> m_notes;

    QList<QAction *>       m_noteActions;

    KNotesAlarm            *m_alarm;
    QTcpServer             *m_listener;
    DNSSD::PublicService   *m_publisher;

    KFind           *m_find;
    QMap<QString, KNote *>::iterator *m_findPos;

    KMenu           *m_noteMenu;
    KMenu           *m_contextMenu;

    KXMLGUIFactory  *m_guiFactory;
    KXMLGUIBuilder  *m_guiBuilder;
    ktomgirl::KTGSystray *m_tray;
    KAction         *m_findAction;

    QDomDocument    m_noteGUI;
    QString m_noteUidModify;
		QString m_notebook;
    // END KNotesApp private:
signals:
	// Should be connected to all knotes so we can update
	// all their menus in one emission.
	void sigNewNotebook(const QString&);
}; // END class KNotesApp
}  //namespace knotes
#endif
// Mon Nov  2 13:54:36 PST 2015
