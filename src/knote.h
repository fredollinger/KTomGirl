/*******************************************************************
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2006, The KNotes Developers

 2012-2014 Modified by Fred Ollinger <follinge@gmail.com> for KTomGirl

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

#ifndef KNOTE_H
#define KNOTE_H

#include <QColor>
#include <QDomDocument>
#include <QEvent>
#include <QFrame>
#include <QString>

#include <kconfig.h>
#include <kxmlguiclient.h>
#include <KSharedConfig>

#include "knoteedit.h"
#include "ktgitem.h"
#include "newnotebookdialog.h"

#include <libktomgirl/note.hpp>


class QLabel;
class QLayout;
class QSizeGrip;

class KFind;
class KMenu;
class KNoteButton;
class KNoteEdit;
class KSelectAction;
class KToggleAction;
class KToolBar;

namespace ktomgirl {
	class KTGItem;	
}

namespace ktomgirl {
class Journal;
}

namespace knotes{
// BEGIN class KNote
class KNote
  : public QFrame, virtual public KXMLGUIClient
{
  Q_OBJECT
// BEGIN class KNote public:
  public:
    // CONSTRUCTOR
    KNote( gnote::Note::Ptr, const QDomDocument& buildDoc, ktomgirl::Journal *journal, QWidget *parent = 0 );
    ~KNote();
    void formatText(void);
    void init( const QDomDocument& );
    bool load_gnote(void);
    void loadNotebooks();

    gnote::Note::Ptr m_gnote;

    void saveConfig() const;

    QString noteId() const;

    QString name();
    QString text() const;

    void setName( const QString &name );
    void setText( const QString &text );
    void setTitleAndBody( const QString &title, const QString &body ); 

    void saveCB(const QString&, bool);

    void find( KFind* kfind );

    bool isModified() const;
    void blockEmitDataChanged( bool _b ) { m_blockEmitDataChanged = _b;}
// END class KNote public:

  public slots:
    void slotAddNotebook(const QString &text);
    void slotAddNotebookMenu(const QString&);
    void slotDebug(void);
    void slotMoveToNotebook(QAction*);
    void slotMakeNoteBook();
    void emitNewNote();
    void slotKill(void);
    void slotNewNoteBook(void);
    void slotShowNoteBookMenu(void);
    void slotShowSearchWindow(void);
    void slotTextChanged(void);

  signals:
    void sigNewNotebook(const QString&);
    void sigNewNote(void);
    void sigRequestNewNote(void);
    void sigNameChanged(const QString &);
    void sigNameChanged(const QString &, ktomgirl::KTGItem&);
    void sigNameChanged(const QString&, const QString& );
    void sigNameChanged(const QString&, const QString&, const QString& );
    void sigDataChanged(const QString &);
    void sigColorChanged();
    void sigCloseNote(const QString &);
    void sigKillNote(const QString &, const QString &);
    void sigShowNextNote();
    void sigFindFinished();
    void sigShowSearchWindow(void);

  protected:
    void keyPressEvent(QKeyEvent *event);
    virtual void contextMenuEvent( QContextMenuEvent * );
    virtual void showEvent( QShowEvent * );
    virtual void resizeEvent( QResizeEvent * );
    virtual void closeEvent( QCloseEvent * );
    virtual void dropEvent( QDropEvent * );
    virtual void dragEnterEvent( QDragEnterEvent * );

    virtual bool event( QEvent * );
    virtual bool eventFilter( QObject *, QEvent * );

// BEGIN PRIVATE SLOTS
  private slots:
    void slotDataChanged(const QString&);

    void slotFormatTitle(void);
    void slotNameChanged(void);
    void slotSave(void);

    void slotSetAlarm();

    void slotPreferences();
    void slotPopupActionToDesktop( int id );

    void slotFindNext();
    void slotHighlight( const QString &txt, int idx, int len );

    void slotApplyConfig();
    void slotUpdateKeepAboveBelow();
    void slotUpdateShowInTaskbar();
    void slotUpdateDesktopActions();

    void slotKeepAbove();
    void slotKeepBelow();

    void slotRequestNewNote();
// END PRIVATE SLOTS

// BEGIN KNOTE PRIVATE 
  private:

    // BEGIN KNOTE PRIVATE VARIABLES
    NewNoteBookDialog *m_dlg;

    QString m_content;
    // m_noteNumber is only for the corner cases where we have a blank title.
    // Ideally this will never be used, b/c we should actually fix the problem
    // of blank titles.
    // But it's a bug fix hack for now.
    int m_noteNumber; 
    QString m_title;
    std::string m_oldTitle;
    QString m_noteId; // note uuid; replaces the kcal::Journal
    QTimer *saveTimer;
    QTimer *formatTimer;

    QLayout       *m_noteLayout;
    QLabel        *m_label;
    QSizeGrip     *m_grip;
    QString 	  m_htmlTitle;

    // Notebooks is the actions which are in the notebook menu
    // Bundle then in a QActionGroup so they are checkable
    QActionGroup  *m_notebooks;
    KNoteButton   *m_button;
    KToolBar      *m_tool; 
    KNoteEdit     *m_editor;

    //ktomgirl::Journal *m_journal;

    KFind         *m_find;
    KMenu         *m_menu;
    KMenu         *m_noteBookMenu;

    KToggleAction *m_readOnly;

#ifdef Q_WS_X11
    KSelectAction   *m_toDesktop;
#endif
    KToggleAction *m_keepAbove;
    KToggleAction *m_keepBelow;

    KSharedConfig::Ptr m_kwinConf;
    bool m_blockEmitDataChanged;
    bool mBlockWriteConfigDuringCommitData;
    bool m_isModified;
    // END KNOTE PRIVATE VARIABLES

    // BEGIN KNOTE PRIVATE METHODS
    void setContent( const QString &title, const QString &text );
    void buildGui();
    void createActions();
    void createNoteEditor();
    void createNoteFooter();
    void createNoteHeader();

    void setCursorBody(void);
    void setCursorTitle(void);
    void prepare();

    void updateFocus();
    void updateLayout();
    void updateLabelAlignment();

    void setColor( const QColor &, const QColor & );

    void toDesktop( int desktop );
    // END KNOTE PRIVATE METHODS

// END class KNote private:
}; // class KNote
} // namespace knotes
#endif // KNOTE_H
// Sat Oct 24 13:20:31 PDT 2015
