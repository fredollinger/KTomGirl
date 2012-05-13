/*******************************************************************
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
#include "gnote.hpp"
#include "note.hpp"
#include "notemanager.hpp"
// END gnote INCLUDES

// BEGIN KTOMGIRL INCLUDES
//#include "ktgconfig.h"
#include "ktgitem.h"
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

using namespace ktomgirl;

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
  // qDebug() << __PRETTY_FUNCTION__;
  m_gnmanager = new gnote::NoteManager();
  kapp->setQuitOnLastWindowClosed( false );

  //m_config = new ktomgirl::KTGConfig();

  // create the dock widget...
  m_tray = new KStatusNotifierItem(0);

  m_tray->setToolTipTitle( i18n( "KTomGirl: Sticky Notes Clone of TomBoy for KDE" ) );
  m_tray->setIconByName( "gnote" );
  m_tray->setStatus( KStatusNotifierItem::Active );
  m_tray->setCategory( KStatusNotifierItem::ApplicationStatus );
  m_tray->setStandardActionsEnabled(false);
  m_tray->activate();

  // create the GUI...
  KAction *action  = new KAction( KIcon( "document-new" ),
                                  i18n( "New Note" ), this );
  actionCollection()->addAction( "new_note", action );
  action->setGlobalShortcut( KShortcut( Qt::ALT + Qt::SHIFT + Qt::Key_N ),
                             KAction::DefaultShortcut );

  connect( action, SIGNAL( triggered() ), SLOT( createNote() ) );


  action  = new KAction( KIcon( "edit-paste" ),
                         i18n( "New Note From Clipboard" ), this );
  actionCollection()->addAction( "new_note_clipboard", action );
  action->setGlobalShortcut( KShortcut( Qt::ALT + Qt::SHIFT + Qt::Key_C ),
                             KAction::DefaultShortcut );
  connect( action, SIGNAL( triggered() ), SLOT( newNoteFromClipboard() ) );

  action  = new KAction( KIcon( "knotes" ), i18n( "Show All Notes" ), this );
  actionCollection()->addAction( "show_all_notes", action );
  action->setGlobalShortcut( KShortcut( Qt::ALT + Qt::SHIFT + Qt::Key_S ),
                             KAction::DefaultShortcut );
  connect( action, SIGNAL( triggered() ), SLOT( showAllNotes() ) );

  action  = new KAction( KIcon( "window-close" ),
                         i18n( "Hide All Notes" ), this );
  actionCollection()->addAction( "hide_all_notes", action );
  action->setGlobalShortcut( KShortcut( Qt::ALT + Qt::SHIFT + Qt::Key_H ),
                             KAction::DefaultShortcut );
  connect( action, SIGNAL( triggered() ), SLOT( hideAllNotes() ) );

  new KHelpMenu( this, KGlobal::mainComponent().aboutData(), false,
                 actionCollection() );

  m_findAction = KStandardAction::find( this, SLOT( slotOpenFindDialog() ),
                         actionCollection() );
  KStandardAction::preferences( this, SLOT( slotPreferences() ),
                         actionCollection() );
  KStandardAction::keyBindings( this, SLOT( slotConfigureAccels() ),
                         actionCollection() );
  //FIXME: no shortcut removing!?
  KStandardAction::quit( this, SLOT( slotQuit() ),
                         actionCollection() )->setShortcut( 0 );

  setXMLFile("knotesappui.rc");

  m_guiBuilder = new KXMLGUIBuilder( this );
  m_guiFactory = new KXMLGUIFactory( m_guiBuilder, this );
  m_guiFactory->addClient( this );

  m_contextMenu = static_cast<KMenu *>( m_guiFactory->container(
                                        "knotes_context",
                                        this ) );
  m_noteMenu = static_cast<KMenu *>( m_guiFactory->container(
                                      "notes_menu", this ) );
  m_tray->setContextMenu( m_contextMenu );
  // get the most recent XML UI file
  QString xmlFileName = componentData().componentName() + "ui.rc";
  QString filter = componentData().componentName() + '/' + xmlFileName;
  const QStringList fileList =
      componentData().dirs()->findAllResources( "data", filter ) +
      componentData().dirs()->findAllResources( "data", xmlFileName );

  QString doc;
  KXMLGUIClient::findMostRecentXMLFile( fileList, doc );
  m_noteGUI.setContent( doc );

  KConfigGroup config( KGlobal::config(), "Global Keybindings" );

  // clean up old config files
  // KNotesLegacy::cleanUp();

  m_manager = new KNotesResourceManager();

  if ( m_notes.size() == 0 && !kapp->isSessionRestored() ) {
      newNote();
  }

  m_searchWindow = new SearchWindow( this );
  m_searchWindow->loadNotes(m_gnmanager->get_notes());
  m_searchWindow->show();

  connect( m_searchWindow->actionNew_Note, SIGNAL( triggered() ), SLOT( createNote() ) );

  // qRegisterMetaType<ktomgirl::KTGItem>( "ktomgirl::KTGItem" );

  connect (m_searchWindow, SIGNAL(signalNoteSelected(ktomgirl::KTGItem*)), this, SLOT(openNote(ktomgirl::KTGItem*)));
}
// END KNotesApp::KNotesApp()

KNotesApp::~KNotesApp()
{
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
  delete m_manager;
  delete m_gnmanager;
  delete m_guiBuilder;
  delete m_tray;
}

bool KNotesApp::commitData( QSessionManager & )
{
  foreach ( KNote *note, m_notes ) {
    note->commitData();
  }
  saveConfigs();
  return true;
}

// -------------------- public D-Bus interface -------------------- //

QString KNotesApp::newNote( const QString &name, const QString &text )
{
  qDebug() << __PRETTY_FUNCTION__ << "FIXME: STUB";
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
  foreach ( KNote *note, m_notes ) {
    note->slotClose();
  }
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
  qDebug() << __PRETTY_FUNCTION__ << id;
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

void KNotesApp::killNote( const QString &id, bool force )
{
  KNote *note = m_notes.value( id );
  if ( note ) {
    note->slotKill( force );
  } else {
    kWarning( 5500 ) << "killNote: no note with id:" << id;
  }
}

// "bool force = false" doesn't work with dcop
void KNotesApp::killNote( const QString &id )
{
  killNote( id, false );
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
  foreach ( KNote *note, m_notes ) {
    if ( note->isModified() ) {
      note->saveData(false);
    }
  }
  saveConfigs();
  kapp->quit();
}

// -------------------- private methods -------------------- //

void KNotesApp::showNote( KNote *note ) const
{
  note->show();
#ifdef Q_WS_X11
  KWindowSystem::setCurrentDesktop( KWindowSystem::windowInfo( note->winId(),
                                    NET::WMDesktop ).desktop() );
  KWindowSystem::forceActiveWindow( note->winId() );
#endif
  note->setFocus();
}

void KNotesApp::createNote( KCal::Journal *journal ){
  qDebug() << __PRETTY_FUNCTION__;

  // FIXME: Need to be flexible about this:
  QString title = "New Note";

  gnote::Note::Ptr new_gnote = m_gnmanager->create_new_note(title.toStdString(), journal->uid().toStdString());
  m_manager->addNewNote( journal );

  KNote *newNote = new KNote( new_gnote, m_noteGUI, journal, 0);
  newNote->load_gnote();
  newNote->setText(QString::fromStdString(title.toStdString()));
  newNote->setObjectName( journal->uid() );

  m_notes.insert( journal->uid(), newNote );
  m_searchWindow->newItem(new_gnote);

  showNote( journal->uid() );
}

void KNotesApp::createNote()
{
  qDebug() << __PRETTY_FUNCTION__;

  KCal::Journal *journal = new KCal::Journal();
  createNote(journal);

  return; 
}

void KNotesApp::killNote( KCal::Journal *journal )
{
  qDebug() << __PRETTY_FUNCTION__ << "FIXME: STUB";
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
}

void KNotesApp::updateNoteActions()
{
  unplugActionList( "notes" );
  m_noteActions.clear();

  foreach ( KNote *note, m_notes ) {

    KAction *action = new KAction( note->name().replace( "&", "&&" ), this );
		action->setObjectName( note->noteId() );
    connect( action, SIGNAL( triggered( bool ) ), SLOT( slotShowNote() ) );
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

void KNotesApp::updateStyle()
{
  foreach ( KNote *note, m_notes ) {
    QApplication::postEvent( note, new QEvent( QEvent::LayoutRequest ) );
  }
}

void KNotesApp::openNote(ktomgirl::KTGItem *item){

  if (item->get_note()->is_open()) {
  	showNote(QString::fromStdString ( item->get_note()->uid() ));
	return;
  }

  const std::string abs_path = item->get_note()->file_path();

  KCal::Journal *journal = new KCal::Journal();

  KNote *newNote = new KNote( item->get_note(), m_noteGUI, journal, 0);
  newNote->load_gnote();

  m_notes.insert( newNote->noteId(), newNote );

  m_noteUidModify = journal->uid();
  newNote->setObjectName( journal->uid() );

  showNote(journal->uid() );

  connect( newNote, SIGNAL( sigNameChanged(const QString&, const QString&) ), m_searchWindow, SLOT( setItemName(const QString&, const QString&)), Qt::QueuedConnection  );

  return;
}

} // namespace knotes
// Sun May 13 10:24:13 PDT 2012
