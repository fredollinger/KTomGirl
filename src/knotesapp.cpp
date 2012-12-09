/*
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2009, The KNotes Developers

 2012 Modified by Fred Ollinger <follinge@gmail.com> for KTomGirl

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

// BEGIN gnote INCLUDES
#include <libktomgirl/gnote.hpp>
#include <libktomgirl/note.hpp>
#include <libktomgirl/notemanager.hpp>
// END gnote INCLUDES

// BEGIN KTOMGIRL INCLUDES
#include "ktgconfig.h"
#include "ktgitem.h"
#include "ktgsystray.h"
#include "searchwindow.h"
// END   KTOMGIRL INCLUDES

// BEGIN KNOTES INCLUDES
#include "knote.h"
#include "knotesapp.h"
#include "resourcemanager.h"
// END   KNOTES INCLUDES

// BEGIN KDE INCLUDES
#include <kaction.h>
#include <kactioncollection.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kfind.h>
#include <kfinddialog.h>
#include <khelpmenu.h>
#include <kicon.h>
#include <kiconeffect.h>
#include <klocale.h>
#include <kmenu.h>
#include <kshortcutsdialog.h>
#include <ksocketfactory.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <kwindowsystem.h>
#include <kxmlguibuilder.h>
#include <kxmlguifactory.h>
#include <KStatusNotifierItem>
#include <kcal/calendarlocal.h>
#include <kcal/journal.h>
#include <kiconloader.h>
// END   KDE INCLUDES

// BEGIN QT INCLUDES
#include <QClipboard>
#include <QDebug>
#include <QPixmap>
#include <QTcpServer>
// END QT INCLUDES

#include <dnssd/publicservice.h>

//using namespace ktomgirl;

namespace knotes{
class KNotesKeyDialog
  : public KDialog
{
public:
KNotesKeyDialog( KActionCollection *globals, QWidget *parent )
      : KDialog( parent )
{
	setCaption( i18n( "Configure Shortcuts" ) );
	setButtons( Default | Ok | Cancel );

	m_keyChooser = new KShortcutsEditor( globals, this );

	connect( this, SIGNAL( defaultClicked() ),
               m_keyChooser, SLOT( allDefault() ) );
}

    void insert( KActionCollection *actions )
    {
        m_keyChooser->addCollection( actions, i18n( "Note Actions" ) );
    }

    void configure()
    {
        if ( exec() == Accepted ) {
            m_keyChooser->save();
        }
    }

private:
    KShortcutsEditor *m_keyChooser;
}; // class KNotesKeyDialog

static bool qActionLessThan( const QAction *a1, const QAction *a2 )
{
  return ( a1->text() < a2->text() );
}

KNotesApp::KNotesApp()
  : QWidget(), m_alarm( 0 ), m_listener( 0 ), m_publisher( 0 ), m_find( 0 ), m_findPos( 0 )
{
  m_gnmanager = new gnote::NoteManager();
  kapp->setQuitOnLastWindowClosed( false );

  m_config = new ktomgirl::KTGConfig();

  // BEGIN DOCK WIDGET
  // create the dock widget...

  // BEGIN SEARCH WINDOW
  m_searchWindow = new SearchWindow( this );
  m_searchWindow->loadNotes(m_gnmanager->get_notes());
  m_searchWindow->loadNotebooks();
  m_searchWindow->show();

  connect( this, SIGNAL( sigNewNotebook(const QString&) ), m_searchWindow, SLOT( slotAddNotebook(const QString&)), Qt::QueuedConnection  );

  connect( m_searchWindow->actionQuit, SIGNAL( triggered() ), SLOT( slotQuit() ) );
  connect (m_searchWindow, SIGNAL(signalNoteSelected(ktomgirl::KTGItem*)), this, SLOT(openNote(ktomgirl::KTGItem*)));

  connect( m_searchWindow->actionShow_Open_Notes, SIGNAL( triggered() ), SLOT( slotSpewOpenNotes() ) );

  connect( m_searchWindow->actionNew_Note, SIGNAL( triggered() ), SLOT( createNote() ) );

  connect( m_searchWindow, SIGNAL( signalHandleSearch(QString) ), SLOT( slotHandleSearch(QString) ) );
  // END SEARCH WINDOW

  // BEGIN KStatusNotifierItem
  m_tray = new ktomgirl::KTGSystray(this, "ktomgirl");

  KMenu *m_menu = new KMenu("KTomGirl");

  connect( m_menu, SIGNAL( triggered(QAction*) ), SLOT( slotShowNote(QAction*) ) );

  QAction *quitAct = new QAction("&Quit", m_tray);
  m_menu->addAction(quitAct);
  connect( quitAct, SIGNAL( triggered() ), SLOT( slotQuit() ) );

  KIcon iconSearch = KIcon(":/icons/search.png");
  QAction *searchAct = new QAction(iconSearch, "&Search all notes", m_tray);
  m_menu->addAction(searchAct);
  connect( searchAct, SIGNAL( triggered() ), SLOT( slotShowSearchWindow() ) );

  KIcon iconNewNote = KIcon(":/icons/notebook_edit.png");
  QAction *createAct = new QAction(iconNewNote, "&Create new note", m_tray);
  m_menu->addAction(createAct);
  connect( createAct, SIGNAL( triggered() ), SLOT( createNote() ) );


  m_tray->setContextMenu(m_menu);
  m_tray->activate();
  // END KStatusNotifierItem

  setXMLFile("knotesappui.rc");

  m_guiBuilder = new KXMLGUIBuilder( this );
  m_guiFactory = new KXMLGUIFactory( m_guiBuilder, this );
  m_guiFactory->addClient( this );

  m_contextMenu = static_cast<KMenu *>( m_guiFactory->container(
                                        "knotes_context",
                                        this ) );
  m_noteMenu = static_cast<KMenu *>( m_guiFactory->container(
                                      "notes_menu", this ) );
  //m_tray->setContextMenu( m_contextMenu );
  // get the most recent XML UI file
  QString xmlFileName = componentData().componentName() + "ui.rc";
  QString filter = componentData().componentName() + '/' + xmlFileName;

  const QStringList fileList =
      componentData().dirs()->findAllResources( "data", filter ) +
      componentData().dirs()->findAllResources( "data", xmlFileName );

  QString doc;
  m_noteGUI.setContent( doc );

  KConfigGroup config( KGlobal::config(), "Global Keybindings" );

  m_manager = new KNotesResourceManager();
}
// END KNotesApp::KNotesApp()

KNotesApp::~KNotesApp()
{
 m_config->store();
/*
  saveConfigs();
  saveNotes();

  blockSignals( true );
  qDeleteAll( m_notes );
  m_notes.clear();
  qDeleteAll( m_noteActions );
  m_noteActions.clear();
  blockSignals( false );

  delete m_listener;
  m_listener=0;
  delete m_publisher;
  m_publisher=0;
  delete m_findPos;
  m_findPos = 0;
  //delete m_listener;
*/

  // FIXME: Try to use smart deleting aka deleteLater() or the
  // smart ptr equiv...
  delete m_manager;
  delete m_gnmanager;
  delete m_guiBuilder;
  // delete m_tray;
  m_tray->deleteLater();
}

/*
bool KNotesApp::commitData( QSessionManager & )
{
  foreach ( KNote *note, m_notes ) {
    //note->commitData();
  }
  saveConfigs();
  return true;
}
*/

// -------------------- public D-Bus interface -------------------- //

QString KNotesApp::newNote( const QString &name, const QString &text )
{
  KCal::Journal *journal = new KCal::Journal();
  m_manager->addNewNote( journal );
  showNote( journal->uid() );
  return journal->uid();
}

QString KNotesApp::newNoteFromClipboard( const QString &name )
{
  const QString &text = KApplication::clipboard()->text();
  return newNote( name, text );
}

void KNotesApp::hideAllNotes() const
{
  qDebug() << __PRETTY_FUNCTION__ << "FIXME: STUB";
/*
  foreach ( KNote *note, m_notes ) {
    note->slotClose();
  }
*/
}

void KNotesApp::showAllNotes() const
{
  foreach ( KNote *note, m_notes ) {
    // workaround to BUG 149116
    note->hide();

    note->show();
  }
}

void KNotesApp::showNote( const QString &id ) const
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    showNote( note );
  } else {
    kWarning( 5500 ) << "showNote: no note with id:" << id;
  }
}

void KNotesApp::hideNote( const QString &id ) const
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    note->hide();
  } else {
    kWarning( 5500 ) << "hideNote: no note with id:" << id;
  }
}

/*
void KNotesApp::killNote( const QString &id, bool force )
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    note->slotKill();
  } else {
    kWarning( 5500 ) << "killNote: no note with id:" << id;
  }
}
*/

void KNotesApp::slotCloseNote( const QString &id )
{
  KNote *note = 0;
  // If we delete the note from the list, we are screwed
  // note = m_notes.take(id);
  note = m_notes[id];

  if ( 0 == note ){
  	qDebug() << __PRETTY_FUNCTION__ <<  "NULL note. Not closing!";
    return;
  }

  note->hide();
  //m_gnmanager->delete_note( note->m_gnote );
  //delete note;

  // QMap<QString, KNote *> m_notes;
  //KNote *note = m_notes.value(id);
  // Remove the note associated with the value

  // note->hide();

//usr/include/qt4/QtCore/qmap.h:688:24: note: candidate is: T QMap<Key, T>::take(const Key&) [with Key = QString, T = knotes::KNote*]
  // delete note;
  // slotSpewNotes();
  // killNote( id, false );
}

QVariantMap KNotesApp::notes() const
{
  QVariantMap notes;

  foreach ( KNote *note, m_notes ) {
    notes.insert( note->noteId(), note->name() );
  }

  return notes;
}

QString KNotesApp::name( const QString &id ) const
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    return note->name();
  } else {
    return QString();
  }
}

QString KNotesApp::text( const QString &id ) const
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    return note->text();
  } else {
    return QString();
  }
}

void KNotesApp::setName( const QString &id, const QString &newName )
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    note->setName( newName );
  } else {
    kWarning( 5500 ) << "setName: no note with id:" << id;
  }
}

void KNotesApp::setText( const QString &id, const QString &newText )
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    note->setText( newText );
  } else {
    kWarning( 5500 ) << "setText: no note with id:" << id;
  }
}

// -------------------- protected slots -------------------- //

void KNotesApp::slotActivateRequested( bool, const QPoint&)
{
    if ( m_notes.size() == 1 ) {
        showNote( *m_notes.begin() );
    } else if ( m_notes.size() != 1 ) {
        m_noteMenu->popup( QCursor::pos ());
    }
}

void KNotesApp::slotSecondaryActivateRequested( const QPoint & )
{
    newNote();
}

void KNotesApp::slotShowNote()
{
  // tell the WM to give this note focus
  showNote( sender()->objectName() );
}

void KNotesApp::slotWalkThroughNotes()
{
  // show next note
  QMap<QString, KNote *>::const_iterator it = m_notes.constBegin();
  for ( ; it != m_notes.constEnd(); ++it ) {
    if ( ( *it )->hasFocus() ) {
      if ( ++it != m_notes.constEnd() ) {
        showNote( *it );
      } else {
        showNote( *m_notes.constBegin() );
      }
      break;
    }
  }
}

#if 0
// BEGIN DEPRECATED
void KNotesApp::slotOpenFindDialog()
{
  KFindDialog findDia( this );
  findDia.setObjectName( "find_dialog" );
  findDia.setHasSelection( false );
  findDia.setHasCursor( false );
  findDia.setSupportsBackwardsFind( false );

  if ( (findDia.exec() != QDialog::Accepted) ) {
      delete m_findPos;
      m_findPos = 0;
      delete m_find;
      m_find = 0;
      return;
  }

  delete m_findPos;
  m_findPos = new QMap<QString, KNote *>::iterator();
  *m_findPos = m_notes.begin();

  // this could be in an own method if searching without a dialog
  // should be possible
  delete m_find;
  m_find = new KFind( findDia.pattern(), findDia.options(), this );

  slotFindNext();
}

void KNotesApp::slotFindNext()
{
  if ( *m_findPos != m_notes.end() ) {
    KNote *note = * ( (*m_findPos)++ );
    note->find( m_find );
  } else {
    m_find->displayFinalDialog();
    m_find->deleteLater(); //we can't delete m_find now because it is the signal emitter
    m_find = 0;
    delete m_findPos;
    m_findPos = 0;
  }
}
// END DEPRECATED
#endif

void KNotesApp::slotPreferences()
{
  #if 0
  // create a new preferences dialog...
  KNoteConfigDlg *dialog = new KNoteConfigDlg( i18n( "Settings" ), this);
  connect( dialog, SIGNAL( configWrote( ) ),
           this,   SLOT( updateNetworkListener() ) );
  connect( dialog, SIGNAL( configWrote(  ) ),
           this,   SLOT( updateStyle() ) );
  dialog->show();
  #endif
}

void KNotesApp::slotConfigureAccels()
{
  KNotesKeyDialog keys( actionCollection(), this );

  QMap<QString, KNote *>::const_iterator it = m_notes.constBegin();

  if ( !m_notes.isEmpty() ) {
    keys.insert( ( *it )->actionCollection() );
  }

  keys.configure();

  // update GUI doc for new notes
  m_noteGUI.setContent(
    KXMLGUIFactory::readConfigFile( componentData().componentName() + "ui.rc",
                                    componentData() )
                      );

  if ( m_notes.isEmpty() ) {
    return;
  }

  foreach ( QAction *action, ( *it )->actionCollection()->actions() ) {
    it = m_notes.constBegin();
    for ( ++it; it != m_notes.constEnd(); ++it ) {
/*
    // Not sure if this is what this message has in mind but since both
    // action->objectName() and KAction::action() are QStrings, this
    // might be fine.
    // Correct me if I am wrong... ~ gamaral
#ifdef __GNUC__
#warning Port KAction::action() to QString
#endif
*/
      QAction *toChange =
        ( *it )->actionCollection()->action( action->objectName() );

      if ( toChange ) {
        toChange->setShortcuts( action->shortcuts() );
      }
    }
  }
}

void KNotesApp::slotNoteKilled( KCal::Journal *journal )
{
  #if 0
  m_noteUidModify.clear();
  m_manager->deleteNote( journal );
  saveNotes();
  #endif
}

void KNotesApp::slotQuit()
{
  qDebug() << __PRETTY_FUNCTION__ << "FIXME: need to sync and store notes";
/*
  foreach ( KNote *note, m_notes ) {
  	qDebug() << __PRETTY_FUNCTION__;
    if ( note->isModified() ) {
	  // We should probably be saving the m_notes (gnote) not knote, or perhaps
	  // we can sync them...
      note->saveData(false);
    }
  }
*/
  // saveConfigs();
  kapp->quit();
}

void KNotesApp::slotShowSearchWindow(){
	m_searchWindow->show();
	m_searchWindow->raise();
	m_searchWindow->activateWindow();
}

void KNotesApp::createNote( KCal::Journal *journal ){
  int n = m_config->noteNumber();

  QString title = tr("New Note ") + QString::number(n, 10); 

  gnote::Note::Ptr new_gnote = m_gnmanager->create_new_note(title.toStdString(), journal->uid().toStdString());
  m_manager->addNewNote( journal );

  KNote *newNote = new KNote( new_gnote, m_noteGUI, journal, 0);
  newNote->setText(QString::fromStdString(title.toStdString()));
  newNote->setObjectName( journal->uid() );

  m_notes.insert( journal->uid(), newNote );
  m_searchWindow->newItem(new_gnote);
  m_searchWindow->styleNotes();

  newNote->init_note();

  noteInit( newNote );

  showNote( journal->uid() );

  saveConfigs();
  //m_config->store();
}

void KNotesApp::slotDeleteNote(const QString &qsTitle){
	m_searchWindow->deleteItem(qsTitle);	

	// BEGIN FIND THE GNOTE
	std::string title = qsTitle.toStdString();
	gnote::Note::Ptr gnote = m_gnmanager->find(qsTitle.toStdString());
	QString m_content = QString::fromStdString(gnote->text_content());
	QString uid = QString::fromStdString(gnote->uid());
	// END FIND THE GNOTE
	
	// delete note from systray
	m_tray->removeNoteAction(uid);

	// delete knote from list
	KNote *knote = m_notes.value(uid);

	// delete actual knote
	delete knote;

	// delete gnote from list
	m_gnmanager->delete_note(gnote);
}


void KNotesApp::createNote()
{
  KCal::Journal *journal = new KCal::Journal();
  createNote(journal);

  return; 
}

void KNotesApp::killNote( KCal::Journal *journal )
{
}

void KNotesApp::acceptConnection()
{
  // Accept the connection and make KNotesNetworkReceiver do the job
  QTcpSocket *s = m_listener->nextPendingConnection();
}

void KNotesApp::saveNotes( const QString & uid )
{
  m_noteUidModify = uid;
  saveNotes();
}

void KNotesApp::saveNotes()
{
}

void KNotesApp::saveConfigs()
{
  m_config->store();
}

void KNotesApp::updateNoteActions()
{
  unplugActionList( "notes" );
  m_noteActions.clear();

  foreach ( KNote *note, m_notes ) {

    KAction *action = new KAction( note->name().replace( "&", "&&" ), this );
		action->setObjectName( note->noteId() );
    connect( action, SIGNAL( triggered( bool ) ), SLOT( slotOpenNote() ) );
    KIconEffect effect;
    QPixmap icon =
      effect.apply( qApp->windowIcon().pixmap( IconSize( KIconLoader::Small ),
                                               IconSize( KIconLoader::Small ) ),
                    KIconEffect::Colorize,
                    1,
                    note->palette().color( note->backgroundRole() ),
                    false );

    action->setIcon( icon );
    m_noteActions.append( action );
  }

  if ( m_noteActions.isEmpty() ) {
      actionCollection()->action( "hide_all_notes" )->setEnabled( false );
      actionCollection()->action( "show_all_notes" )->setEnabled( false );
      m_findAction->setEnabled( false );
    KAction *action = new KAction( i18n( "No Notes" ), this );
    m_noteActions.append( action );
  }
  else
  {
      qSort( m_noteActions.begin(), m_noteActions.end(), qActionLessThan );
      actionCollection()->action( "hide_all_notes" )->setEnabled( true );
      actionCollection()->action( "show_all_notes" )->setEnabled( true );
      m_findAction->setEnabled( true );
  }
  plugActionList( "notes", m_noteActions );
}

void KNotesApp::updateNetworkListener()
{
    delete m_listener;
    m_listener=0;
    delete m_publisher;
    m_publisher=0;
}

/*
void KNotesApp::updateStyle()
{
  foreach ( KNote *note, m_notes ) {
    QApplication::postEvent( note, new QEvent( QEvent::LayoutRequest ) );
  }
}
*/

/* Common code for createNote() and newNote() */
// BEGIN noteInit()
void KNotesApp::noteInit( KNote *newNote){
  connect( newNote, SIGNAL( sigNameChanged(const QString&, const QString&) ), m_searchWindow, SLOT( setItemName(const QString&, const QString&)), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigNameChanged(const QString&, const QString&) ), m_tray, SLOT( setItemName(const QString&, const QString&)), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigKillNote(const QString&) ), this, SLOT( slotDeleteNote(const QString&)), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigShowSearchWindow() ), this, SLOT( slotShowSearchWindow()), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigNewNote() ), this, SLOT( createNote()), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigCloseNote(const QString&) ), this, SLOT( slotCloseNote(const QString&)), Qt::QueuedConnection  );

  connect( newNote, SIGNAL( sigNewNotebook(const QString&) ), this, SLOT( slotNewNotebook(const QString&)), Qt::QueuedConnection  );

  connect( this, SIGNAL( sigNewNotebook(const QString&) ), newNote, SLOT( slotAddNotebook(const QString&)), Qt::QueuedConnection  );

  newNote->loadNotebooks();
} // END noteInit()

/* Perhaps we can combine this with the latter part of the
 * other openNote(ktgitem*) ?
 */
void KNotesApp::openNote(QString &qs){
  gnote::Note::Ptr gnote = m_gnmanager->find(qs.toStdString());

  if (! gnote){
        qDebug() << __PRETTY_FUNCTION__<< "failed to load gnote";
	return;

  }

  KCal::Journal *journal = new KCal::Journal();

  KNote *newNote = new KNote( gnote, m_noteGUI, journal, 0);
  newNote->load_gnote();

  m_notes.insert( newNote->noteId(), newNote );

  m_noteUidModify = journal->uid();
  newNote->setObjectName( journal->uid() );

  showNote(journal->uid() );

  noteInit( newNote );

  return;
}


void KNotesApp::openNote(ktomgirl::KTGItem *item){
  QMap<QString, KNote*>::const_iterator i = m_notes.find(QString::fromStdString(item->uid()));
  if (i != m_notes.end()) {
	  	showNote(QString::fromStdString ( item->uid() ));
	return;
  }

  gnote::Note::Ptr gnote = m_gnmanager->find(item->text().toStdString());

  if (! gnote){
    qDebug() << __PRETTY_FUNCTION__<< "failed to load gnote";
	  return;
  }

  KCal::Journal *journal = new KCal::Journal();

  KNote *newNote = new KNote( gnote, m_noteGUI, journal, 0);
  newNote->load_gnote();

  m_notes.insert( newNote->noteId(), newNote );

  m_noteUidModify = journal->uid();
  newNote->setObjectName( journal->uid() );

  showNote(journal->uid() );

  noteInit( newNote );

  return;

}

/* This is here purely for debugging purposes. 
   Here we spew the open note titles to console.
 */
void KNotesApp::slotSpewOpenNotes(){
	foreach ( KNote *note, m_notes ) {
    		qDebug() << note->name();
  	}
}

// BEGIN KNotesApp::showNote(KNote *)
void KNotesApp::showNote( KNote *note ) const
{
  QString l_uid = m_notes.key( note );
  KMenu *m_menu = m_tray->contextMenu();
  KIcon iconNote = KIcon(":/icons/notebook.png");
  QAction *act = new QAction(iconNote, note->name(), m_tray);
  connect( m_menu, SIGNAL( triggered(QAction*) ), SLOT( slotOpenNote(QAction*) ) );
  m_tray->addNoteAction(act, l_uid);

  note->show();
#ifdef Q_WS_X11
  KWindowSystem::setCurrentDesktop( KWindowSystem::windowInfo( note->winId(),
                                    NET::WMDesktop ).desktop() );
  KWindowSystem::forceActiveWindow( note->winId() );
#endif
  note->setFocus();
}
// END KNotesApp::showNote(KNote *)

// BEGIN KNotesApp::slotOpenNote(QAction*)
void  KNotesApp::slotOpenNote(QAction *act){
	// FIXME: Open note here...
	QVariant qvar = act->data();
	QString l_uid = qvar.toString();
	if ( 0 == l_uid.length() ){
		return;
	}

	KNote *note = m_notes.value(l_uid);
  	if ( note ) {
		showNote(l_uid);
		return;	
  	}

	openNote(l_uid);
	return;	
}	
// END KNotesApp::slotOpenNote(QAction*)

void  KNotesApp::slotNewNotebook(const QString &qs){
	// tell all knotes to add the menu
	emit sigNewNotebook(qs);
  	m_config->addNotebook(qs);
	m_config->store();
}

void  KNotesApp::slotHandleSearch(QString qs){
	qDebug() << __PRETTY_FUNCTION__ << qs; 

#if 0
	// BEGIN FIND THE GNOTE
	std::string title = qsTitle.toStdString();
	gnote::Note::Ptr gnote = m_gnmanager->find(qsTitle.toStdString());
	QString m_content = QString::fromStdString(gnote->text_content());
	QString uid = QString::fromStdString(gnote->uid());
	// END FIND THE GNOTE
#endif
}

} // namespace knotes
// Sun Sep 30 10:42:55 PDT 2012
