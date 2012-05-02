/*******************************************************************
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2007, The KNotes Developers

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

// ktomgirl includes
#include "ktgitem.h"

// knotes includes
#include "knote.h"
#include "version.h"

// gnote includes
#include "note.hpp"
// #include "notemanager.hpp"

// BEGIN KDE INCLUDES
#include <kaction.h>
#include <kactioncollection.h>
#include <kapplication.h>
#include <kcal/journal.h>
#include <kcodecs.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kfiledialog.h>
#include <kfind.h>
#include <kglobalsettings.h>
#include <kicon.h>
#include <kiconeffect.h>
#include <kiconloader.h>
#include <kinputdialog.h>
#include <kio/netaccess.h>
#include <klocale.h>
#include <kmenu.h>
#include <kmessagebox.h>
#include <kprocess.h>
#include <kselectaction.h>
#include <ksocketfactory.h>
#include <kstandardaction.h>
#include <kstandarddirs.h>
#include <ktoggleaction.h>
#include <ktoolbar.h>
#include <kwindowsystem.h>
#include <kxmlguibuilder.h>
#include <kxmlguifactory.h>
#include <netwm.h>
// END KDE INCLUDES

// BEGIN QT INCLUDES
#include <QBoxLayout>
#include <QCheckBox>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QSize>
#include <QSizeGrip>
#include <QTextStream>
#include <QVBoxLayout>
// END QT INCLUDES

#ifdef Q_WS_X11
#include <fixx11h.h>
#include <QX11Info>
#endif

using namespace KCal;

namespace knotes{

static const QString startTitle = "<font color=\"Blue\" size=\"16\"><u>";
static const QString endTitle = "</u></font><br>";

static const QString startNormal = "<font color=\"Black\" size=\"12\">";
static const QString endNormal = "</font><br>";

KNote::KNote( gnote::Note::Ptr gnoteptr, const QDomDocument& buildDoc, Journal *j, QWidget *parent )
  : QFrame( parent), m_label( 0 ), m_grip( 0 ),
//  : QFrame( parent, Qt::FramelessWindowHint ), m_label( 0 ), m_grip( 0 ),
    m_button( 0 ), m_tool( 0 ), m_editor( 0 ), m_config( 0 ), m_journal( j ),
    m_find( 0 ), m_kwinConf( KSharedConfig::openConfig( "kwinrc" ) ), m_blockEmitDataChanged( false ),mBlockWriteConfigDuringCommitData( false )
    , m_gnote(gnoteptr)
{ 
	j->setUid(QString::fromStdString(gnoteptr->uid()));
	init(buildDoc);
  	m_gnote->set_is_open(true);
}

KNote::~KNote()
{
  qDebug() << __PRETTY_FUNCTION__ << text();
  m_gnote->set_is_open(false);

  // FIXME: save before delete
  //void set_text_content(const std::string & text);
  // m_gnote->set_text_content(text().toStdString());
  // m_gnote->save();
  delete m_config;
}

void KNote::load_gnote(){
	setName(QString::fromStdString(m_gnote->get_title()));
	QString content = QString::fromStdString(m_gnote->text_content());
	setText(content);
	formatTitle();
	// qDebug() << __PRETTY_FUNCTION__ << " uid " << QString::fromStdString(m_gnote->uid());
	connect( this, SIGNAL( sigDataChanged(const QString &) ),
              this, SLOT( slotDataChanged(const QString &) ) );
}

void KNote::slotDataChanged(const QString &qs){
  if (m_blockEmitDataChanged) return;

  m_blockEmitDataChanged = true;

  const QString t = getTitle();

  m_gnote->set_title(t.toStdString());

  // Sync title bar with title
  setWindowTitle(t);
  //m_label->setText(t);
  /* Make sure the title is blue, big, and underlined
   * and ensure that other things are not... */
  formatTitle();

  // m_gnote->set_title(t.toStdString());
  // This cues the note up for a save next time it is requested
  // we do this to save resources so we don't save every single note
  // that is closed only those who have changed.
  m_gnote->changed();
  qDebug() << __PRETTY_FUNCTION__ << "emitting name changed" << qs;
  emit sigNameChanged(t);
  emit sigNameChanged(t, QString::fromStdString(m_gnote->get_title()) );

  m_blockEmitDataChanged = false;
}

void KNote::init( const QDomDocument& buildDoc ){
  qDebug() << __PRETTY_FUNCTION__;
  setAcceptDrops( true );
  setAttribute( Qt::WA_DeleteOnClose );
  setDOMDocument( buildDoc );
  setObjectName( m_journal->uid() );
  setXMLFile( componentData().componentName() + "ui.rc", false, false );

  // create the main layout
  m_noteLayout = new QVBoxLayout( this );
  m_noteLayout->setMargin( 0 );

  createActions();

  buildGui();

  prepare();
}

// -------------------- public slots -------------------- //

void KNote::slotKill( bool force )
{
  m_blockEmitDataChanged = true;
  if ( !force &&
       ( KMessageBox::warningContinueCancel( this,
         i18n( "<qt>Do you really want to delete note <b>%1</b>?</qt>",
               m_label->text() ),
         i18n( "Confirm Delete" ),
         KGuiItem( i18n( "&Delete" ), "edit-delete" ),
         KStandardGuiItem::cancel(),
         "ConfirmDeleteNote" ) != KMessageBox::Continue ) ) {
     m_blockEmitDataChanged = false;
     return;
  }
  // delete the configuration first, then the corresponding file
  delete m_config;
  m_config = 0;
  QString configFile = KGlobal::dirs()->saveLocation( "appdata", "notes/" );
  // configFile += m_journal->uid();
  if ( !KIO::NetAccess::del( KUrl( configFile ), this ) ) {
    kError( 5500 ) <<"Can't remove the note config:" << configFile;
  }

  emit sigKillNote( m_journal );
}


// -------------------- public member functions -------------------- //

void KNote::saveData(bool update )
{
  if(update)
  {
     emit sigDataChanged(noteId());
     // m_editor->document()->setModified( false );
  }
}

void KNote::saveConfig() const
{
#ifdef Q_WS_X11
  NETWinInfo wm_client( QX11Info::display(), winId(),
                        QX11Info::appRootWindow(), NET::WMDesktop );
  if ( ( wm_client.desktop() == NETWinInfo::OnAllDesktops ) ||
     ( wm_client.desktop() > 0 ) ) {
    // m_config->setDesktop( wm_client.desktop() );
  }
#endif

  // actually store the config on disk
  // m_config->writeConfig();
}

QString KNote::noteId() const
{
  return m_journal->uid();
}

QString KNote::name() const
{
  return m_label->text();
}

QString KNote::text() const
{
  //QString qs;
  // return qs;
  return m_editor->text();
}

void KNote::setName( const QString& name )
{
  m_label->setText( name );
  updateLabelAlignment();

  if ( m_editor ) {    // not called from CTOR?
    saveData();
  }
#ifdef Q_WS_X11
  // set the window's name for the taskbar entry to be more helpful (#58338)
  NETWinInfo note_win( QX11Info::display(), winId(), QX11Info::appRootWindow(),
                       NET::WMDesktop );
  note_win.setName( name.toUtf8() );
#endif

  emit sigNameChanged(name);
}

void KNote::setText( const QString& text )
{
  m_editor->setText( text );

  saveData();
}

void KNote::find( KFind* kfind )
{
  m_find = kfind;
  disconnect( m_find );
  connect( m_find, SIGNAL( highlight( const QString &, int, int ) ),
           this, SLOT( slotHighlight( const QString &, int, int ) ) );
  connect( m_find, SIGNAL( findNext() ), this, SLOT( slotFindNext() ) );

  // m_find->setData( m_editor->toPlainText() );
  slotFindNext();
}

void KNote::slotFindNext()
{
  // TODO: honor FindBackwards

  // Let KFind inspect the text fragment, and display a dialog if a match is
  // found
  KFind::Result res = m_find->find();

  if ( res == KFind::NoMatch ) { // i.e. at end-pos

    QTextCursor c = m_editor->textCursor(); //doesn't return by reference, so we use setTextCursor
    c.clearSelection();
    // m_editor->setTextCursor( c );

    disconnect( m_find, 0, this, 0 );
    emit sigFindFinished();
  } else {

    show();
#ifdef Q_WS_X11
    KWindowSystem::setCurrentDesktop( KWindowSystem::windowInfo( winId(),
                                      NET::WMDesktop ).desktop() );
#endif
  }
}

void KNote::slotHighlight( const QString& /*str*/, int idx, int len )
{
  // m_editor->textCursor().clearSelection();
  // m_editor->highlightWord( len, idx );

  // TODO: modify the selection color, use a different QTextCursor?
}

bool KNote::isModified() const
{
  return true;
  // return m_editor->document()->isModified();
}

// ------------------ private slots (menu actions) ------------------ //

void KNote::slotRename()
{
    m_blockEmitDataChanged = true;
  // pop up dialog to get the new name
  bool ok;
  const QString oldName = m_label->text();
  const QString newName = KInputDialog::getText( QString::null, //krazy:exclude=nullstrassign for old broken gcc
    i18n( "Please enter the new name:" ), m_label->text(), &ok, this );
  m_blockEmitDataChanged = false;
  if ( !ok || (oldName == newName) ) { // handle cancel
    return;
  }

  setName( newName );
}

void KNote::slotUpdateReadOnly()
{
  const bool readOnly = m_readOnly->isChecked();

  // m_editor->setReadOnly( readOnly );
  // m_config->setReadOnly( readOnly );

  // enable/disable actions accordingly
  actionCollection()->action( "configure_note" )->setEnabled( !readOnly );
  actionCollection()->action( "insert_date" )->setEnabled( !readOnly );
  actionCollection()->action( "delete_note" )->setEnabled( !readOnly );
  actionCollection()->action( "format_bold" )->setEnabled( !readOnly );
  actionCollection()->action( "format_italic" )->setEnabled( !readOnly );
  actionCollection()->action( "format_underline" )->setEnabled( !readOnly );
  actionCollection()->action( "format_strikeout" )->setEnabled( !readOnly );
  actionCollection()->action( "format_alignleft" )->setEnabled( !readOnly );
  actionCollection()->action( "format_aligncenter" )->setEnabled( !readOnly );
  actionCollection()->action( "format_alignright" )->setEnabled( !readOnly );
  actionCollection()->action( "format_alignblock" )->setEnabled( !readOnly );
  actionCollection()->action( "format_list" )->setEnabled( !readOnly );
  actionCollection()->action( "format_super" )->setEnabled( !readOnly );
  actionCollection()->action( "format_sub" )->setEnabled( !readOnly );
  actionCollection()->action( "format_increaseindent" )->setEnabled( !readOnly );
  actionCollection()->action( "format_decreaseindent" )->setEnabled( !readOnly );
  actionCollection()->action( "text_background_color" )->setEnabled( !readOnly );
  actionCollection()->action( "format_size" )->setEnabled( !readOnly );
  actionCollection()->action( "format_color" )->setEnabled( !readOnly );
  actionCollection()->action( "rename_note" )->setEnabled( !readOnly);

  updateFocus();
}


void KNote::commitData()
{
  mBlockWriteConfigDuringCommitData = true;
}

void KNote::slotClose()
{
#ifdef Q_WS_X11
  NETWinInfo wm_client( QX11Info::display(), winId(),
                        QX11Info::appRootWindow(), NET::WMDesktop );
  if ( ( wm_client.desktop() == NETWinInfo::OnAllDesktops ) ||
       ( wm_client.desktop() > 0 ) ) {
    // m_config->setDesktop( wm_client.desktop() );
  }
#endif

  // m_editor->clearFocus();
  if( !mBlockWriteConfigDuringCommitData )
  {
    // m_config->setHideNote( true );
    // m_config->setPosition( pos() );
    // m_config->writeConfig();
  }
  // just hide the note so it's still available from the dock window
  hide();
}

void KNote::slotInsDate()
{
  // m_editor->insertPlainText(
    // KGlobal::locale()->formatDateTime( QDateTime::currentDateTime() ) );
}

void KNote::slotSetAlarm()
{
    m_blockEmitDataChanged = true;
  #if 0
  // KNoteAlarmDlg dlg( name(), this );
  // dlg.setIncidence( m_journal );

  if ( dlg.exec() == QDialog::Accepted ) {
    emit sigDataChanged(noteId());
  }
  #endif
  m_blockEmitDataChanged = false;
}

void KNote::slotPreferences()
{
    m_blockEmitDataChanged = true;

  #if 0
  // create a new preferences dialog...
  // KNoteSimpleConfigDlg *dialog = new KNoteSimpleConfigDlg( m_config, name(), this, noteId() );
  connect( dialog, SIGNAL( settingsChanged( const QString & ) ) , this,
           SLOT( slotApplyConfig() ) );
  connect( this, SIGNAL( sigNameChanged(const QString &) ), dialog,
           SLOT( slotUpdateCaption(const QString &) ) );
  dialog->exec();
  delete dialog;
  m_blockEmitDataChanged = false;
  saveData();
  #endif
}

void KNote::slotSend()
{
  #if 0
  // pop up dialog to get the IP
  KNoteHostDlg hostDlg( i18n( "Send \"%1\"", name() ), this );
  // const bool ok = ( hostDlg.exec() == QDialog::Accepted );

  if ( !ok ) { // handle cancel
    return;
  }
  const QString host = hostDlg.host();
  quint16 port = hostDlg.port();

  if ( !port ) { // not specified, use default
    port = KNotesGlobalConfig::port();
  }

  if ( host.isEmpty() ) {
    KMessageBox::sorry( this, i18n( "The host cannot be empty." ) );
    return;
  }

  // Send the note

  KNotesNetworkSender *sender = new KNotesNetworkSender(
    KSocketFactory::connectToHost( "knotes", host, port ) );
  sender->setSenderId( KNotesGlobalConfig::senderID() );
  sender->setNote( name(), text() ); // FIXME: plainText ??
  #endif
}

void KNote::slotMail()
{
  #if 0
  // get the mail action command
  // const QStringList cmd_list = KNotesGlobalConfig::mailAction().split( QChar(' '),
      QString::SkipEmptyParts );

  KProcess mail;
  foreach ( const QString &cmd, cmd_list ) {
    if ( cmd == "%f" ) {
      // mail << m_editor->toPlainText();
    } else if ( cmd == "%t" ) {
      mail << m_label->text();
    } else {
      mail << cmd;
    }
  }

  if ( !mail.startDetached() ) {
    KMessageBox::sorry( this, i18n( "Unable to start the mail process." ) );
  }
  #endif
}

void KNote::slotPrint()
{
  QString content;
  /*
  if ( !Qt::mightBeRichText( m_editor->text() ) ) {
    content = Qt::convertFromPlainText( m_editor->text() );
  } else {
    content = m_editor->text();
  }
  KNotePrinter printer;
  printer.setDefaultFont( m_config->font() );
  printer.printNote( name(), content );
  */
}

void KNote::slotSaveAs()
{
  // TODO: where to put pdf file support? In the printer??!??!
  m_blockEmitDataChanged = true;
  QCheckBox *convert = 0;

  if ( m_editor->acceptRichText() ) {
    convert = new QCheckBox( 0 );
    convert->setText( i18n( "Save note as plain text" ) );
  }
  m_blockEmitDataChanged = true;
  KUrl url;
  KFileDialog dlg( url, QString(), this, convert );
  dlg.setOperationMode( KFileDialog::Saving );
  dlg.setCaption( i18n( "Save As" ) );
  dlg.exec();

  QString fileName = dlg.selectedFile();
  if ( fileName.isEmpty() ) {
    m_blockEmitDataChanged = false;
    return;
  }

  QFile file( fileName );

  if ( file.exists() &&
       KMessageBox::warningContinueCancel( this,
          i18n( "<qt>A file named <b>%1</b> already exists.<br />"
                "Are you sure you want to overwrite it?</qt>",
                QFileInfo( file ).fileName() ) ) != KMessageBox::Continue ) {
    m_blockEmitDataChanged = false;
    return;
  }

  if ( file.open( QIODevice::WriteOnly ) ) {
    QTextStream stream( &file );
    if ( convert && !convert->isChecked() ) {
      // stream << m_editor->toHtml();
    } else {
      // stream << m_editor->toPlainText();
    }
  }
  m_blockEmitDataChanged = false;
}

void KNote::slotPopupActionToDesktop( int id )
{
  toDesktop( id - 1 ); // compensate for the menu separator, -1 == all desktops
}


// ------------------ private slots (configuration) ------------------ //

void KNote::slotApplyConfig()
{
  #if 0
  m_label->setFont( m_config->titleFont() );
  // m_editor->setRichText( m_config->richText() );
  m_editor->setTextFont( m_config->font() );
  m_editor->setTabStop( m_config->tabSize() );
  m_editor->setAutoIndentMode( m_config->autoIndent() );

  setColor( m_config->fgColor(), m_config->bgColor() );

  updateLayout();
  slotUpdateShowInTaskbar();
  #endif
}

void KNote::slotKeepAbove()
{
    if ( m_keepBelow->isChecked() )
    {
        m_keepBelow->setChecked( false );
    }
    slotUpdateKeepAboveBelow();
}

void KNote::slotKeepBelow()
{
    if ( m_keepAbove->isChecked() )
    {
        m_keepAbove->setChecked( false );
    }
    slotUpdateKeepAboveBelow();
}

void KNote::slotUpdateKeepAboveBelow()
{
#ifdef Q_WS_X11
  unsigned long state = KWindowInfo( KWindowSystem::windowInfo( winId(), NET::WMState ) ).state();
#else
  unsigned long state = 0; // neutral state, TODO
#endif
  #if 0
  if ( m_keepAbove->isChecked() ) {
    m_config->setKeepAbove( true );
    m_config->setKeepBelow( false );
    KWindowSystem::setState( winId(), state | NET::KeepAbove );
  } else if ( m_keepBelow->isChecked() ) {
    m_config->setKeepAbove( false );
    m_config->setKeepBelow( true );
    KWindowSystem::setState( winId(), state | NET::KeepBelow );
  } else {
    m_config->setKeepAbove( false );
    KWindowSystem::clearState( winId(), NET::KeepAbove );
    m_config->setKeepBelow( false );
    KWindowSystem::clearState( winId(), NET::KeepBelow );
  }
  #endif
}

void KNote::slotUpdateShowInTaskbar()
{
#if 0
#ifdef Q_WS_X11
  if ( !m_config->showInTaskbar() ) {
    KWindowSystem::setState( winId(), KWindowSystem::windowInfo( winId(),
                             NET::WMState ).state() | NET::SkipTaskbar );
  } else {
    KWindowSystem::clearState( winId(), NET::SkipTaskbar );
  }
#endif
#endif
}

void KNote::slotUpdateDesktopActions()
{
#ifdef Q_WS_X11
  NETRootInfo wm_root( QX11Info::display(), NET::NumberOfDesktops |
                       NET::DesktopNames );
  NETWinInfo wm_client( QX11Info::display(), winId(),
                        QX11Info::appRootWindow(), NET::WMDesktop );

  QStringList desktops;
  desktops.append( i18n( "&All Desktops" ) );
  desktops.append( QString::null );           // Separator
                                              // krazy:exclude=nullstrassign
                                              // for old broken gcc

  const int count = wm_root.numberOfDesktops();
  for ( int n = 1; n <= count; n++ ) {
    desktops.append( QString( "&%1 %2" ).arg( n ).arg(
      QString::fromUtf8( wm_root.desktopName( n ) ) ) );
  }
  m_toDesktop->setItems( desktops );

  if ( wm_client.desktop() == NETWinInfo::OnAllDesktops ) {
    m_toDesktop->setCurrentItem( 0 );
  } else {
    m_toDesktop->setCurrentItem( wm_client.desktop() + 1 ); // compensate for
                                                            // separator (+1)
  }
#endif
}


// -------------------- private methods -------------------- //

void KNote::buildGui()
{
  qDebug() << __PRETTY_FUNCTION__;
  createNoteHeader();
  createNoteEditor();

  KXMLGUIBuilder builder( this );
  KXMLGUIFactory factory( &builder, this );
  factory.addClient( this );

  m_menu = dynamic_cast<KMenu*>( factory.container( "note_context", this ) );
  m_tool = dynamic_cast<KToolBar*>( factory.container( "note_tool", this ) );

  createNoteFooter();
}

void KNote::createActions()
{
  // create the menu items for the note - not the editor...
  // rename, mail, print, save as, insert date, alarm, close, delete, new note
  KAction *action;

  action  = new KAction( KIcon( "document-new" ), i18n( "New" ),  this );
  actionCollection()->addAction( "new_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotRequestNewNote() ) );

  action  = new KAction( KIcon( "edit-rename" ), i18n( "Rename..." ), this );
  actionCollection()->addAction( "rename_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotRename() ) );

  m_readOnly  = new KToggleAction( KIcon( "object-locked" ),
                                   i18n( "Lock" ), this );
  actionCollection()->addAction( "lock_note", m_readOnly );
  connect( m_readOnly, SIGNAL( triggered( bool ) ),
          SLOT( slotUpdateReadOnly() ) );
  m_readOnly->setCheckedState( KGuiItem( i18n( "Unlock" ), "object-unlocked" ) );

  action  = new KAction( KIcon( "window-close" ), i18n( "Hide" ), this );
  actionCollection()->addAction( "hide_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotClose() ) );
  action->setShortcut( QKeySequence( Qt::Key_Escape ) );

  action  = new KAction( KIcon( "edit-delete" ), i18n( "Delete" ), this );
  actionCollection()->addAction( "delete_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotKill() ),Qt::QueuedConnection );

  action  = new KAction( KIcon( "knotes_date" ), i18n( "Insert Date" ), this );
  actionCollection()->addAction( "insert_date", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotInsDate() ) );

  action  = new KAction( KIcon( "knotes_alarm" ), i18n( "Set Alarm..." ),
                         this );
  actionCollection()->addAction( "set_alarm", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotSetAlarm() ) );

  action  = new KAction( KIcon( "network-wired" ), i18n( "Send..." ), this );
  actionCollection()->addAction( "send_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotSend() ) );

  action  = new KAction( KIcon( "mail-send" ), i18n( "Mail..." ), this );
  actionCollection()->addAction( "mail_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotMail() ) );

  action  = new KAction( KIcon( "document-save-as" ), i18n( "Save As..." ),
                                this );
  actionCollection()->addAction( "save_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotSaveAs() ) );
  actionCollection()->addAction( KStandardAction::Print,  "print_note", this,
                                 SLOT( slotPrint() ) );

  action  = new KAction( KIcon( "configure" ), i18n( "Preferences..." ), this );
  actionCollection()->addAction( "configure_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotPreferences() ) );


  m_keepAbove  = new KToggleAction( KIcon( "go-up" ),
                                    i18n( "Keep Above Others" ), this );
  actionCollection()->addAction( "keep_above", m_keepAbove );
  connect( m_keepAbove, SIGNAL( triggered( bool ) ),
           SLOT( slotKeepAbove() ) );

  m_keepBelow  = new KToggleAction( KIcon( "go-down" ),
                                    i18n( "Keep Below Others" ), this );
  actionCollection()->addAction( "keep_below", m_keepBelow );
  connect( m_keepBelow, SIGNAL( triggered( bool ) ),
           SLOT( slotKeepBelow() ) );

#ifdef Q_WS_X11
  m_toDesktop  = new KSelectAction( i18n( "To Desktop" ), this );
  actionCollection()->addAction( "to_desktop", m_toDesktop );
  connect( m_toDesktop, SIGNAL( triggered( int ) ),
           SLOT( slotPopupActionToDesktop( int ) ) );
  connect( m_toDesktop->menu(), SIGNAL( aboutToShow() ),
           SLOT( slotUpdateDesktopActions() ) );
#endif

  // invisible action to walk through the notes to make this configurable
  action  = new KAction( i18n( "Walk Through Notes" ), this );
  actionCollection()->addAction( "walk_notes", action );
  connect( action, SIGNAL( triggered( bool ) ), SIGNAL( sigShowNextNote() ) );
  action->setShortcut( QKeySequence( Qt::SHIFT + Qt::Key_Backtab ) );

  actionCollection()->addAssociatedWidget( this );
  foreach (QAction* action, actionCollection()->actions())
    action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
}

void KNote::createNoteHeader()
{
  // load style configuration
  KConfigGroup styleGroup( m_kwinConf, "Style" );

  QBoxLayout::Direction headerLayoutDirection = QBoxLayout::LeftToRight;

  if ( styleGroup.readEntry( "CustomButtonPositions", false ) ) {
    if ( styleGroup.readEntry( "ButtonsOnLeft" ).contains( 'X' ) ) {
      headerLayoutDirection = QBoxLayout::RightToLeft;
    }
  }

  QBoxLayout *headerLayout = new QBoxLayout( headerLayoutDirection);


  // create header label
  m_label = new QLabel( this );
  headerLayout->addWidget( m_label );
  m_label->setFrameStyle( NoFrame );
  m_label->setBackgroundRole( QPalette::Base );
  m_label->setLineWidth( 0 );
  m_label->setAutoFillBackground( true );
  m_label->installEventFilter( this );  // receive events ( for dragging &
                                        // action menu )
  // setName( m_journal->summary() );      // don't worry, no signals are
                                        // connected at this stage yet
  #if 0
  m_button = new KNoteButton( "knotes_close", this );
  headerLayout->addWidget( m_button );

  connect( m_button, SIGNAL( clicked() ), this, SLOT( slotClose() ) );

  m_noteLayout->addItem( headerLayout );
  #endif
}

void KNote::createNoteEditor()
{
  qDebug() << __PRETTY_FUNCTION__;
  m_editor = new KNoteEdit( actionCollection(), this );
  m_noteLayout->addWidget( m_editor );
  m_editor->setNote( this );
  m_editor->installEventFilter( this ); // receive focus events for modified
  setFocusProxy( m_editor );

  #if 0
  // BEGIN SET TITLE BLUE
  m_textCursor = new QTextCursor( m_editor->document() );
  QTextBlockFormat format = m_textCursor->blockFormat();

  // format.setTextColor(QColor(Qt:blue));
  format.setNonBreakableLines(true);
  m_textCursor->setBlockFormat(format);
  m_textCursor->movePosition(QTextCursor::Start); 

  QString line = titleHtml + "Title" + endHtml;

  m_textCursor->insertHtml(line);
  m_textCursor->insertBlock();

  format.setNonBreakableLines(false);
  format.setBackground(QColor("lightyellow"));
  m_textCursor->setBlockFormat(format);
  //m_textCursor->insertText(tr("The background color of a text block can be "
                         //"changed to highlight text."));
  m_textCursor->insertBlock();
  // END SET TITLE BLUE
  #endif
}

void KNote::slotRequestNewNote()
{
    //Be sure to save before to request a new note
    saveConfig();
    saveData();
    emit sigRequestNewNote();
}

void KNote::createNoteFooter()
{
  if ( m_tool ) {
    m_tool->setIconSize( QSize( 10, 10 ) );
    m_tool->setFixedHeight( 24 );
    m_tool->setToolButtonStyle( Qt::ToolButtonIconOnly );
  }

  // create size grip
  QHBoxLayout *gripLayout = new QHBoxLayout;
  m_grip = new QSizeGrip( this );
  m_grip->setFixedSize( m_grip->sizeHint() );

  if ( m_tool ) {
    gripLayout->addWidget( m_tool );
    gripLayout->setAlignment( m_tool, Qt::AlignBottom | Qt::AlignLeft );
    m_tool->hide();
  }

  gripLayout->addWidget( m_grip );
  gripLayout->setAlignment( m_grip, Qt::AlignBottom | Qt::AlignRight );
  m_noteLayout->addItem( gripLayout );

  // if there was just a way of making KComboBox adhere the toolbar height...
  if ( m_tool ) {
    foreach ( KComboBox *combo, m_tool->findChildren<KComboBox *>() ) {
      QFont font = combo->font();
      font.setPointSize( 7 );
      combo->setFont( font );
      combo->setFixedHeight( 14 );
    }
  }
}

void KNote::prepare()
{
  // the config file location
  // const QString configFile = KGlobal::dirs()->saveLocation( "appdata", "notes/" ) + m_journal->uid();

  // no config file yet? -> use the default display config if available
  // we want to write to configFile, so use "false"
  // const bool newNote = !KIO::NetAccess::exists( KUrl( configFile ),
                                          //KIO::NetAccess::DestinationSide, 0 );

  #if 0
  m_config = new KNoteConfig( KSharedConfig::openConfig( configFile,
                                                         KConfig::NoGlobals ) );
  m_config->readConfig();
  m_config->setVersion( KNOTES_VERSION );

  if ( newNote ) {
    // until kdelibs provides copying of KConfigSkeletons (KDE 3.4)
    KNotesGlobalConfig *globalConfig = KNotesGlobalConfig::self();
    m_config->setBgColor( globalConfig->bgColor() );
    m_config->setFgColor( globalConfig->fgColor() );
    m_config->setWidth( globalConfig->width() );
    m_config->setHeight( globalConfig->height() );

    m_config->setFont( globalConfig->font() );
    m_config->setTitleFont( globalConfig->titleFont() );
    m_config->setAutoIndent( globalConfig->autoIndent() );
    m_config->setRichText( globalConfig->richText() );
    m_config->setTabSize( globalConfig->tabSize() );
    m_config->setReadOnly( globalConfig->readOnly() );

    m_config->setDesktop( globalConfig->desktop() );
    m_config->setHideNote( globalConfig->hideNote() );
    m_config->setPosition( globalConfig->position() );
    m_config->setShowInTaskbar( globalConfig->showInTaskbar() );
    m_config->setRememberDesktop( globalConfig->rememberDesktop() );
    m_config->setKeepAbove( globalConfig->keepAbove() );
    m_config->setKeepBelow( globalConfig->keepBelow() );

    m_config->writeConfig();
  }
  #endif

  // set up the look&feel of the note
  setFrameStyle( Panel | Raised );
  setMinimumSize( 20, 20 );
  setBackgroundRole( QPalette::Base );

  m_editor->setContentsMargins( 0, 0, 0, 0 );
  m_editor->setBackgroundRole( QPalette::Base );
  m_editor->setFrameStyle( NoFrame );
  // m_editor->setText( m_journal->description() );

  // load the display configuration of the note
  #if 0
  uint width = m_config->width();
  uint height = m_config->height();
  resize( width, height );

  // let KWin do the placement if the position is illegal--at least 10 pixels
  // of a note need to be visible
  const QPoint& position = m_config->position();
  QRect desk = kapp->desktop()->rect();
  desk.adjust( 10, 10, -10, -10 );
  if ( desk.intersects( QRect( position, QSize( width, height ) ) ) ) {
    move( position );           // do before calling show() to avoid flicker
  }

  // config items in the journal have priority
  QString property = m_journal->customProperty( "KNotes", "FgColor" );
  if ( !property.isNull() ) {
    m_config->setFgColor( QColor( property ) );
  } else {
    m_journal->setCustomProperty( "KNotes", "FgColor",
                                  m_config->fgColor().name() );
  }

  property = m_journal->customProperty( "KNotes", "BgColor" );
  if ( !property.isNull() ) {
    m_config->setBgColor( QColor( property ) );
  } else {
    m_journal->setCustomProperty( "KNotes", "BgColor",
                                  m_config->bgColor().name() );
  }
  property = m_journal->customProperty( "KNotes", "RichText" );
  if ( !property.isNull() ) {
    m_config->setRichText( property == "true" ? true : false );
  } else {
    m_journal->setCustomProperty( "KNotes", "RichText",
                                  m_config->richText() ? "true" : "false" );
  }

  // read configuration settings...
  slotApplyConfig();

  // if this is a new note put on current desktop - we can't use defaults
  // in KConfig XT since only _changes_ will be stored in the config file
  int desktop = m_config->desktop();

#ifdef Q_WS_X11
  if ( ( desktop < 0 && desktop != NETWinInfo::OnAllDesktops ) ||
       !m_config->rememberDesktop() )
    desktop = KWindowSystem::currentDesktop();
#endif

  // show the note if desired
  if ( desktop != 0 && !m_config->hideNote() ) {
    // to avoid flicker, call this before show()
    toDesktop( desktop );
    show();

    // because KWin forgets about that for hidden windows
#ifdef Q_WS_X11
    if ( desktop == NETWinInfo::OnAllDesktops ) {
      toDesktop( desktop );
    }
#endif
  }

  m_readOnly->setChecked( m_config->readOnly() );
  slotUpdateReadOnly();

  if ( m_config->keepAbove() ) {
    m_keepAbove->setChecked( true );
  } else if ( m_config->keepBelow() ) {
    m_keepBelow->setChecked( true );
  } else {
    m_keepAbove->setChecked( false );
    m_keepBelow->setChecked( false );
  }
  slotUpdateKeepAboveBelow();

  // HACK: update the icon color - again after showing the note, to make kicker
  // aware of the new colors
  KIconEffect effect;
  QPixmap icon = effect.apply( qApp->windowIcon().pixmap(
                                 IconSize( KIconLoader::Desktop ),
                                 IconSize( KIconLoader::Desktop ) ),
                               KIconEffect::Colorize,
                               1, m_config->bgColor(), false );
  QPixmap miniIcon = effect.apply( qApp->windowIcon().pixmap(
                                     IconSize( KIconLoader::Small ),
                                     IconSize( KIconLoader::Small ) ),
                                   KIconEffect::Colorize,
                                   1, m_config->bgColor(), false );
#ifdef Q_WS_X11
  KWindowSystem::setIcons( winId(), icon, miniIcon );
#endif
  m_editor->document()->setModified( false );
  #endif
}

void KNote::toDesktop( int desktop )
{
  if ( desktop == 0 ) {
    return;
  }

#ifdef Q_WS_X11
  if ( desktop == NETWinInfo::OnAllDesktops ) {
    KWindowSystem::setOnAllDesktops( winId(), true );
  } else {
    KWindowSystem::setOnDesktop( winId(), desktop );
  }
#endif
}

void KNote::setColor( const QColor &fg, const QColor &bg )
{
  QPalette p = palette();

  // better: from light(150) to light(100) to light(75)
  // QLinearGradient g( width()/2, 0, width()/2, height() );
  // g.setColorAt( 0, bg );
  // g.setColorAt( 1, bg.dark(150) );

  p.setColor( QPalette::Window,     bg );
  // p.setBrush( QPalette::Window,     g );
  p.setColor( QPalette::Base,       bg );
  // p.setBrush( QPalette::Base,       g );

  p.setColor( QPalette::WindowText, fg );
  p.setColor( QPalette::Text,       fg );

  p.setColor( QPalette::Button,     bg.dark( 116 ) );
  p.setColor( QPalette::ButtonText, fg );

  //p.setColor( QPalette::Highlight,  bg );
  //p.setColor( QPalette::HighlightedText, fg );

  // order: Light, Midlight, Button, Mid, Dark, Shadow

  // the shadow
  p.setColor( QPalette::Light, bg.light( 180 ) );
  p.setColor( QPalette::Midlight, bg.light( 150 ) );
  p.setColor( QPalette::Mid, bg.light( 150 ) );
  p.setColor( QPalette::Dark, bg.dark( 108 ) );
  p.setColor( QPalette::Shadow, bg.dark( 116 ) );

  setPalette( p );

  // darker values for the active label
  p.setColor( QPalette::Active, QPalette::Base, bg.dark( 116 ) );

  m_label->setPalette( p );

  // set the text color
  m_editor->setTextColor( fg );

  // update the icon color
  KIconEffect effect;
  QPixmap icon = effect.apply( qApp->windowIcon().pixmap(
                                 IconSize( KIconLoader::Desktop ),
                                 IconSize( KIconLoader::Desktop ) ),
                               KIconEffect::Colorize, 1, bg, false );
  QPixmap miniIcon = effect.apply( qApp->windowIcon().pixmap(
                                     IconSize( KIconLoader::Small ),
                                     IconSize( KIconLoader::Small ) ),
                                   KIconEffect::Colorize, 1, bg, false );
#ifdef Q_WS_X11
  KWindowSystem::setIcons( winId(), icon, miniIcon );
#endif
  // update the color of the title
  updateFocus();
  emit sigColorChanged();
}

void KNote::updateLabelAlignment()
{
  // if the name is too long to fit, left-align it, otherwise center it (#59028)
  const QString labelText = m_label->text();
  if ( m_label->fontMetrics().boundingRect( labelText ).width() >
       m_label->width() ) {
    m_label->setAlignment( Qt::AlignLeft );
  } else {
    m_label->setAlignment( Qt::AlignHCenter );
  }
}

void KNote::updateFocus()
{
  if ( hasFocus() )
  {

    if ( !m_editor->isReadOnly() )
    {
        if ( m_tool && m_tool->isHidden() && m_editor->acceptRichText() )
        {
            m_tool->show();
            setGeometry( x(), y(), width(), height() + m_tool->height() );
        }
        m_grip->show();
    }
    else
    {
        if ( m_tool && !m_tool->isHidden() ) {
            m_tool->hide();
            setGeometry( x(), y(), width(), height() - m_tool->height() );
            updateLayout();     // to update the minimum height
        }
        m_grip->hide();
    }
  }
  else
  {
    m_grip->hide();

    if ( m_tool && !m_tool->isHidden() )
    {
      m_tool->hide();
      setGeometry( x(), y(), width(), height() - m_tool->height() );
      updateLayout();     // to update the minimum height
    }
  }
}

void KNote::updateLayout()
{
  // TODO: remove later if no longer needed.
  updateLabelAlignment();
}

// -------------------- protected methods -------------------- //

void KNote::contextMenuEvent( QContextMenuEvent *e )
{
  if ( m_menu ) {
    m_menu->popup( e->globalPos() );
  }
}

void KNote::showEvent( QShowEvent * )
{
  #if 0
  if ( m_config->hideNote() ) {
    // KWin does not preserve these properties for hidden windows
    slotUpdateKeepAboveBelow();
    slotUpdateShowInTaskbar();
    toDesktop( m_config->desktop() );
    move( m_config->position() );
    m_config->setHideNote( false );
  }
  #endif
}

void KNote::resizeEvent( QResizeEvent *qre )
{
  QFrame::resizeEvent( qre );
  updateLayout();
}

void KNote::closeEvent( QCloseEvent * event )
{
  qDebug() << __PRETTY_FUNCTION__ << text();
  // m_gnote->set_text_content(text().toStdString());
  m_gnote->save(text().toStdString());
  event->ignore(); //We don't want to close (and delete the widget). Just hide it
  slotClose();
}

void KNote::dragEnterEvent( QDragEnterEvent *e )
{
  /*
  if ( !m_config->readOnly() ) {
    e->setAccepted( e->mimeData()->hasColor() );
  }
  */
}

void KNote::dropEvent( QDropEvent *e )
{
  /*
  if ( m_config->readOnly() ) {
    return;
  }

  const QMimeData *md = e->mimeData();
  if ( md->hasColor() ) {
       QColor bg =  qvariant_cast<QColor>( md->colorData() );
       setColor( palette().color( foregroundRole() ), bg );
       m_journal->setCustomProperty( "KNotes", "BgColor", bg.name() );
       m_config->setBgColor( bg );
  }
  */
}

bool KNote::event( QEvent *ev )
{
  if ( ev->type() == QEvent::LayoutRequest ) {
    updateLayout();
    return true;
  } else {
    return QFrame::event( ev );
  }
}

bool KNote::eventFilter( QObject *o, QEvent *ev )
{
  if ( ev->type() == QEvent::DragEnter &&
    static_cast<QDragEnterEvent*>( ev )->mimeData()->hasColor() ) {
    dragEnterEvent( static_cast<QDragEnterEvent *>( ev ) );
    return true;
  }

  if ( ev->type() == QEvent::Drop &&
       static_cast<QDropEvent *>( ev )->mimeData()->hasColor() ) {
    dropEvent( static_cast<QDropEvent *>( ev ) );
    return true;
  }

  if ( o == m_label ) {
    QMouseEvent *e = ( QMouseEvent * )ev;

    if ( ev->type() == QEvent::MouseButtonDblClick ) {
       if(!m_editor->isReadOnly())
          slotRename();
    }

    if ( ev->type() == QEvent::MouseButtonPress &&
        ( e->button() == Qt::LeftButton || e->button() == Qt::MidButton ) ) {
#ifdef Q_WS_X11
      e->button() == Qt::LeftButton ? KWindowSystem::raiseWindow( winId() )
                                    : KWindowSystem::lowerWindow( winId() );

      XUngrabPointer( QX11Info::display(), QX11Info::appTime() );
      NETRootInfo wm_root( QX11Info::display(), NET::WMMoveResize );
      wm_root.moveResizeRequest( winId(), e->globalX(), e->globalY(),
                                 NET::Move );
#endif
      return true;
    }

    if ( ev->type() == QEvent::MouseButtonRelease ) {
#ifdef Q_WS_X11
        NETRootInfo wm_root( QX11Info::display(), NET::WMMoveResize );
        wm_root.moveResizeRequest( winId(), e->globalX(), e->globalY(),
                                   NET::MoveResizeCancel );
#endif
        return false;
    }

    return false;
  }

  if ( o == m_editor ) {
    if ( ev->type() == QEvent::FocusOut ) {
          QFocusEvent *fe = static_cast<QFocusEvent *>( ev );
          if ( fe->reason() != Qt::PopupFocusReason &&
               fe->reason() != Qt::MouseFocusReason ) {
            updateFocus();
            if ( isModified() ) {
              saveConfig();
              if ( !m_blockEmitDataChanged )
                  saveData();
            }
          }
    } else if ( ev->type() == QEvent::FocusIn ) {
      updateFocus();
    }

    return false;
  }

  return false;
}

QString KNote::getTitle(){
	QString t = m_editor->toPlainText();
	QString str = t.section('\n', 0, 1);
	qDebug() << __PRETTY_FUNCTION__ << str;
	return str.trimmed();
}

void KNote::formatTitle(){
  QTextCursor cursor(m_editor->document());

  //BEGIN BLUE
  /* Make the first line blue and underlined */
  cursor.setPosition(0, QTextCursor::MoveAnchor);  
  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);  
  QString s=cursor.selectedText();
  cursor.insertHtml(startTitle+s+endTitle);  
  //END BLUE

  // BEGIN SECOND LINE
  /* Make second line normal: sometimes we get spillover from first line 
  cursor.setPosition(1, QTextCursor::MoveAnchor);  
  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);  
  s=cursor.selectedText();
  cursor.insertHtml(startNormal+s+endNormal);  
*/
  // END SECOND LINE
}

}// namespace knotes
// Sun Apr 29 15:05:32 PDT 2012
