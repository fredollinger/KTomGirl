/*******************************************************************
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2007, The KNotes Developers

 2012, 2013 Modified by Fred Ollinger <follinge@gmail.com> for KTomGirl

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
#include "newnotebookdialog.h"
#include "resourcemanager.h"
#include "version.h"

// gnote includes
#include <libktomgirl/ktglib.hpp>
#include <libktomgirl/note.hpp>
#include <libktomgirl/notebookmanager.hpp>

// BEGIN KDE INCLUDES
#include <kaction.h>
#include <kactioncollection.h>
#include <kapplication.h>
//#include <kcal/journal.h>
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
#include <QTextCursor>
#include <QTextDocumentFragment>
#include <QTextStream>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextDocument>
#include <QTimer>
#include <QVBoxLayout>
// END QT INCLUDES

#ifdef Q_WS_X11
#include <fixx11h.h>
#include <QX11Info>
#endif

//using namespace KCal;

namespace knotes{

static const QString startTitle = "<p><font color=\"Blue\" size=\"16\"><u>";
static const QString endTitle = "</u></font></p>";

//static const QString startNormal = "<p><font face=\"georgia, serif\" color=\"Black\" size=\"15\">";
//static const QString endNormal = "</font></p>";


KNote::KNote( gnote::Note::Ptr gnoteptr, const QDomDocument& buildDoc, ktomgirl::Journal *journal,  QWidget *parent )
  : QFrame( parent), m_label( 0 ), m_grip( 0 ),
    m_button( 0 ), m_tool( 0 ), m_editor(0), m_journal(journal), 
    m_find( 0 ), m_kwinConf( KSharedConfig::openConfig( "kwinrc" ) ), m_blockEmitDataChanged( false ),mBlockWriteConfigDuringCommitData( false )
    , m_gnote(gnoteptr)
    , m_content("")
    , m_title("")
{ 
	journal->setUid(QString::fromStdString(gnoteptr->uid()));
	m_noteId=QString::fromStdString(gnoteptr->uid());
	init(buildDoc);

 	m_gnote->set_is_open(true);

	saveTimer = new QTimer(this);
	formatTimer = new QTimer(this);
	connect(saveTimer, SIGNAL(timeout()), this, SLOT(slotSave()));
	connect(formatTimer, SIGNAL(timeout()), this, SLOT(slotFormatTitle()));
	connect(m_editor, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));

	saveTimer->start(4000);
	formatTimer->start(1000);

	setWindowIcon(KIcon(":/icons/notebook.png"));
}

KNote::~KNote()
{
  //qDebug() << __PRETTY_FUNCTION__ << " : " << name() << "\n";

  m_gnote->set_is_open(false);

  saveTimer->stop();
  m_gnote->set_text_content(m_editor->toPlainText().toStdString());
  m_gnote->save();
  //emit sigDataChanged(noteId());

  formatTimer->stop();

  /* Stop saving after we focus out */
  m_blockEmitDataChanged = false;

  // delete m_gnote;

  return;
}

/* This is to be done last */
#if 0
void KNote::init_note(){
  qDebug() << __PRETTY_FUNCTION__ << " BUG: REMOVE DEPRECATED FUNCTION";
	//formatText();
	//connect( this, SIGNAL( sigDataChanged(const QString &) ), this, SLOT( slotDataChanged(const QString &) ) );
}
#endif

void KNote::load_gnote(){
	static bool l_loaded = false;

	/* We are not handling notes being closed properly due to some over cleverness!! */
	if (l_loaded){
  		qDebug() << __PRETTY_FUNCTION__<< "BUG!! Note all ready loaded!!";
		return;
	}

	m_title = QString::fromStdString(m_gnote->get_title());
	setName(m_title);

	m_content = QString::fromStdString(m_gnote->text_content_plain());

	/* Work around for stupid libktomgirl bug */
 	if ("" == m_content){
  		//qDebug() << __PRETTY_FUNCTION__<< "loading text busted***" << m_content << "!!!";
			m_content = QString::fromStdString(m_gnote->text_content());
	}

	setContent(m_title, m_content);

  formatText();
	//init_note();
}

void KNote::init( const QDomDocument& buildDoc ){
  setAcceptDrops( true );
  setAttribute( Qt::WA_DeleteOnClose );
  setDOMDocument( buildDoc );
  setObjectName( m_noteId );
  setXMLFile( componentData().componentName() + "ui.rc", false, false );

  // create the main layout
  m_noteLayout = new QVBoxLayout( this );
  m_noteLayout->setMargin( 0 );

  createActions();

  buildGui();

  prepare();
}

// -------------------- public member functions -------------------- //

/* TODO: Either fix or eliminate */
#if 0
void KNote::saveData(bool update )
{
  /*
  if(update)
  {
     emit sigDataChanged(noteId());
  }
  */
}
#endif

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
  return m_noteId;
}

QString KNote::text() const
{
  //QString qs;
  // return qs;
  return m_editor->text();
}

void KNote::setName( const QString& name )
{
  updateLabelAlignment();

  if ( m_editor ) {    // not called from CTOR?
		slotSave();
  }
#ifdef Q_WS_X11
  // set the window's name for the taskbar entry to be more helpful (#58338)
  NETWinInfo note_win( QX11Info::display(), winId(), QX11Info::appRootWindow(),
                       NET::WMDesktop );
  note_win.setName( name.toUtf8() );
#endif

  m_gnote->set_title(name.toStdString());
  emit sigNameChanged(name);
}

/* FRED: TODO: Set the title and the text separate using insertBlock() with a custom
 * b lue underline block for the title. Then format Title() will work better plus we 
 * can more easily squeeze out annoying line breaks which messes up the title all the 
 * time!
 */
void KNote::setContent( const QString& title, const QString& text )
{
  QBrush titleColor = QBrush(QColor(Qt::blue));
  QTextCharFormat bodyFormat;
  QTextCursor cursor = m_editor->textCursor();

  QString newtitle = title;
  /* If our title and the first line of the text content do not match then something
   * bad has happened, probably during a save. To recover, we just use the content
   * that we all ready have. */ 
  if (!text.startsWith(title)) {
    m_editor->setText( text );
    slotFormatTitle();
  }

  int pos = cursor.position();

  QStringRef body = text.midRef(title.size(), -1);

  newtitle = startTitle+newtitle.remove("\n")+endTitle.trimmed()+"\n\n";
  //newtitle = startTitle+newtitle.remove("\n")+endTitle.trimmed();

  cursor.movePosition(QTextCursor::Start);

  QTextBlock firstBlock = cursor.block();
  QTextBlockFormat titleFormat = firstBlock.blockFormat();
  titleFormat.setNonBreakableLines(true);
  QTextCharFormat titleCharFormat = firstBlock.charFormat();
  titleCharFormat.setUnderlineStyle(QTextCharFormat::SingleUnderline);
  titleCharFormat.setForeground(titleColor);

  m_editor->insertHtml( newtitle );

  cursor.movePosition(QTextCursor::EndOfBlock);

  cursor.insertBlock();
  m_editor->insertPlainText( body.toString() );
}

void KNote::setText( const QString& text )
{
  m_editor->setText( text );
  slotFormatTitle();
}

void KNote::setTitleAndBody( const QString &title, const QString &body ){
  QTextCursor cursor = m_editor->textCursor();

  QTextCharFormat bodyFormat;
  bodyFormat.setFontUnderline(false); 
  bodyFormat.setForeground(QBrush(QColor(Qt::black))); 
  bodyFormat.setFontPointSize(14);

  QString formattedText = startTitle + title + endTitle;
  m_editor->setHtml(formattedText);

  cursor.insertBlock();

  int pos = cursor.position();

  cursor.insertText(body);  

  cursor.setPosition(pos, QTextCursor::KeepAnchor);
  cursor.setCharFormat(bodyFormat);

  m_editor->setTextCursor(cursor); 
  
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

// FIXME: implement. Tells if the note changed and is not yet saved.
// if TRUE, then we are NOT saved.
bool KNote::isModified() const
{
  if (m_isModified) return true;

  QString newContent = m_editor->toPlainText();

  if ("" == newContent){
  	//qDebug() << __PRETTY_FUNCTION__ << "Note is Empty, don't save";
		return false;
  }

// FIXME: return false when this is fixed
  if (newContent == m_content){
	return false;
  }

  return true;
}

void KNote::slotRename()
{
  m_blockEmitDataChanged = true;
  m_blockEmitDataChanged = false;
  return;
}

void KNote::slotDebug(){
  qDebug() << __PRETTY_FUNCTION__ << m_editor->toHtml();

}

#if 0
// BEGIN KNote::slotClose()
void KNote::slotClose()
{
 /*
  saveTimer->stop();
  formatTimer->stop();
  m_gnote->set_text_content(m_editor->toPlainText().toStdString());
  m_gnote->save();
  emit sigDataChanged(noteId());
  saveTimer->stop();
  formatTimer->stop();
*/
  // emit sigCloseNote( QString::fromStdString(m_gnote->get_title()));

  emit sigCloseNote( name() );
  // hide();
} // END KNote::slotClose()
#endif

void KNote::slotInsDate()
{
  // m_editor->insertPlainText(
    // KGlobal::locale()->formatDateTime( QDateTime::currentDateTime() ) );
}

void KNote::slotSetAlarm()
{
  m_blockEmitDataChanged = true;
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
  // KNoteHostDlg hostDlg( i18n( "Send \"%1\"", name() ), this );
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
{}

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

void KNote::slotPopupActionToDesktop( int id )
{
  toDesktop( id - 1 ); // compensate for the menu separator, -1 == all desktops
}


void KNote::slotApplyConfig()
{
  #if 0
  m_label->setFont( m_config->titleFont() );
  // m_editor->setRichText( m_config->richText() );
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

void KNote::buildGui()
{
  createNoteHeader();
  createNoteEditor();

  KXMLGUIBuilder builder( this );
  KXMLGUIFactory factory( &builder, this );
  factory.addClient( this );

  // QHBoxLayout *toolLayout = new QHBoxLayout;

  m_menu = dynamic_cast<KMenu*>( factory.container( "note_context", this ) );

  m_noteBookMenu = new KMenu("notebook", this);

  m_notebooks = new QActionGroup(this);
  m_notebooks->setExclusive(true);

  KIcon iconNoteBook = KIcon(":/icons/notebook_edit.png");
  QAction *notebook_action = m_noteBookMenu->addAction(iconNoteBook, i18n("New Notebook..."));
  QAction *nonotebook_action = m_noteBookMenu->addAction(i18n("No Notebook..."));
  m_noteBookMenu->addSeparator();
  connect(notebook_action, SIGNAL(triggered()), this, SLOT(slotNewNoteBook()));

  createNoteFooter();
}

void KNote::createActions()
{
  // create the menu items for the note - not the editor...
  // rename, mail, print, save as, insert date, alarm, close, delete, new note
  KAction *action;

#ifdef Q_WS_X11
  m_toDesktop  = new KSelectAction( i18n( "To Desktop" ), this );
  actionCollection()->addAction( "to_desktop", m_toDesktop );
  connect( m_toDesktop, SIGNAL( triggered( int ) ),
           SLOT( slotPopupActionToDesktop( int ) ) );
  connect( m_toDesktop->menu(), SIGNAL( aboutToShow() ),
           SLOT( slotUpdateDesktopActions() ) );
#endif // Q_WS_X11
}

void KNote::slotShowSearchWindow(){
	emit sigShowSearchWindow();
	
}

void KNote::slotShowNoteBookMenu(){
	m_noteBookMenu->exec(QCursor::pos());
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

  // BEGIN MAKE TOOLBAR
  m_tool =  new KToolBar(this, true, false);

  m_tool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

  KIcon iconSearch = KIcon(":/icons/search.png");
  QAction *search_action = m_tool->addAction(iconSearch, i18n("Search"));
  connect(search_action, SIGNAL(triggered()), this, SLOT(slotShowSearchWindow()));

  KIcon iconTrash = KIcon(":/icons/trash.png");
  QAction *trash_action = m_tool->addAction(iconTrash, i18n("Trash"));
  connect(trash_action, SIGNAL(triggered()), this, SLOT(slotKill()));

	//setWindowIcon(KIcon(":/icons/notebook.png"));

  KIcon iconNewNote = KIcon(":/icons/notebook.png");
  QAction *new_note_action = m_tool->addAction(iconNewNote, i18n("New Note"));
  connect(new_note_action, SIGNAL(triggered()), this, SLOT(emitNewNote()));

  KIcon iconNoteBook = KIcon(":/icons/notebook_edit.png");
  QAction *noteBook_action = m_tool->addAction(iconNoteBook, i18n("Notebook"));
  connect(noteBook_action, SIGNAL(triggered()), this, SLOT(slotShowNoteBookMenu()));

  KIcon iconBug = KIcon(":/icons/bug.png");
  QAction *bug_action = m_tool->addAction(iconBug, i18n("Debug"));
  connect(bug_action, SIGNAL(triggered()), this, SLOT(slotDebug()));

  m_noteLayout->addWidget( m_tool );
  m_noteLayout->setAlignment( m_tool, Qt::AlignTop);
  // END MAKE TOOLBAR
                                        // action menu )
  //setName( m_journal->summary() );      // don't worry, no signals are
                                        // connected at this stage yet
}

void KNote::createNoteEditor()
{
  m_editor = new KNoteEdit( actionCollection(), this );
  m_noteLayout->addWidget( m_editor );
  m_editor->setNote( this );
  m_editor->installEventFilter( this ); // receive focus events for modified
  setFocusProxy( m_editor );
}

/* I have no clue what this does. */
void KNote::slotRequestNewNote()
{
    //Be sure to save before to request a new note
    saveConfig();
		slotSave();
    //saveData();
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

  gripLayout->addWidget( m_grip );
  gripLayout->setAlignment( m_grip, Qt::AlignBottom | Qt::AlignRight );
  m_noteLayout->addItem( gripLayout );

}

void KNote::prepare()
{
  // set up the look&feel of the note
  setFrameStyle( Panel | Raised );
  setMinimumSize( 20, 20 );
  setBackgroundRole( QPalette::Base );

  m_editor->setContentsMargins( 0, 0, 0, 0 );
  m_editor->setBackgroundRole( QPalette::Base );
  m_editor->setFrameStyle( NoFrame );

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

  // m_label->setPalette( p );

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
  #if 0
  // if the name is too long to fit, left-align it, otherwise center it (#59028)
  const QString labelText = m_label->text();
  if ( m_label->fontMetrics().boundingRect( labelText ).width() >
       m_label->width() ) {
    m_label->setAlignment( Qt::AlignLeft );
  } else {
    m_label->setAlignment( Qt::AlignHCenter );
  }
  #endif
}

void KNote::updateFocus()
{
  #if 0
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
            //m_tool->hide();
            setGeometry( x(), y(), width(), height() - m_tool->height() );
            updateLayout();     // to update the minimum height
        }
        //m_grip->hide();
    }
  }
  else
  {
    //m_grip->hide();

    if ( m_tool && !m_tool->isHidden() )
    {
      //m_tool->hide();
      setGeometry( x(), y(), width(), height() - m_tool->height() );
      updateLayout();     // to update the minimum height
    }
  }
#endif
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
  saveTimer->start();
  formatTimer->start();
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

  event->ignore(); //We don't want to close (and delete the widget). Just hide it

  emit sigCloseNote( QString::fromStdString(m_gnote->uid()) );

  // slotClose();
}

void KNote::dragEnterEvent( QDragEnterEvent *e )
{
}

void KNote::dropEvent( QDropEvent *e )
{
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

void KNote::keyPressEvent(QKeyEvent *event) {
        // BEGIN RETURN KEY
        if (event->key() == Qt::Key_Return) {
          int line = m_editor->textCursor().blockNumber() + 1;
          if (1 == line ){
           //qDebug() << __PRETTY_FUNCTION__ <<  line;
		       slotFormatTitle();
         	 QWidget::keyPressEvent(event);
          }
        } // END RETURN KEY
        else if ((event->key()==Qt::Key_Z) && (event->modifiers()==Qt::ControlModifier)){
                qDebug() << __PRETTY_FUNCTION__ << " CTRL-Z Pressed";
        }
        else {
            QWidget::keyPressEvent(event);
        }
}

bool KNote::eventFilter( QObject *o, QEvent *ev ) {

#if 0
  if(ev->type() == QEvent::KeyPress) {
    // Stupid Qtism: Key_Return is keyboard vs Key_Enter on keypad.
    // * Sigh *
    if( static_cast<QKeyEvent*>(ev)->key() == Qt::Key_Enter ||
        static_cast<QKeyEvent*>(ev)->key() == Qt::Key_Return ){
        int line = m_editor->textCursor().blockNumber() + 1;
        if (1 == line ){
        }
      }
  }
#endif

  if ( ev->type() == QEvent::FocusOut ){
	formatTimer->stop();
	saveTimer->stop();
  }
  if ( ev->type() == QEvent::FocusIn ){
	formatTimer->start();
	saveTimer->start();
  }

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

  if ( o == m_editor ) {
    if ( ev->type() == QEvent::FocusOut ) {
          QFocusEvent *fe = static_cast<QFocusEvent *>( ev );
          if ( fe->reason() != Qt::PopupFocusReason &&
               fe->reason() != Qt::MouseFocusReason ) {
            updateFocus();
            if ( isModified() ) {
              saveConfig();
              if ( !m_blockEmitDataChanged )
                  slotSave();
            }
          }
    } else if ( ev->type() == QEvent::FocusIn ) {
      updateFocus();
    }

    return false;
  }

  return false;
}

QString KNote::name(){
  QTextCursor cursor = m_editor->textCursor();
	cursor.setPosition(0, QTextCursor::MoveAnchor);  
  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);  
  QString newtitle=cursor.selectedText();
  newtitle.remove("\n");

  //int n = m_config->noteNumber();
	if ( "" != newtitle ){
		m_title=newtitle;
	}
	else{
		qDebug() << __PRETTY_FUNCTION__ << " Blank name!";
		// FRED: TODO: NEED TO PASTE THE PROPER NAME HERE...
	}

  return m_title;
}

// BEGIN slotFormatTitle()
/* This slot is called every second. We don't reformat title each time b/c that
 * would be annoying. Thus, we do a few checks and only do so when the time is
 * right aka, we won't be fighting with the user as they try to edit the title.
 * Our sister function format Title() does the same thing, but without the check
 * which is because we use format Title() for those times when we know we really
 * want the title formatted such as upon first startup. 
 */ 
void KNote::slotFormatTitle(){
  static bool bNeedsFormatting = false;
  QTextCursor cursor = m_editor->textCursor();
  int pos = cursor.position();
  int col = cursor.columnNumber();

  // if we are on the first line don't change anything
  // otherwise, we'll wind up running over things
  if (pos == col){
    bNeedsFormatting = true;
    return;
  }

  if (!bNeedsFormatting){
    return;
  }
  bNeedsFormatting = false;

  QString newContent;

  /*
  QTextCharFormat bodyFormat;
  bodyFormat.setFontUnderline(false); 
  bodyFormat.setForeground(QBrush(QColor(Qt::black))); 
  bodyFormat.setFontPointSize(14);
  */

  cursor.setPosition(0, QTextCursor::MoveAnchor);  
  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);  
  QString s=cursor.selectedText();
  s.remove("\n");
  m_title = s;
  QString newtitle = startTitle+s+endTitle.trimmed()+"\n";

  if (m_htmlTitle == newtitle) {
	  return;
  }
  //qDebug() << __PRETTY_FUNCTION__ << " Calling formatText()";
  formatText();

  return;
}
// END slotFormatTitle()

// BEGIN KNOTE SLOTS
void KNote::slotDataChanged(const QString &qs){
  //qDebug() << __PRETTY_FUNCTION__ << "***" << qs << "!!!";

  const QString newTitle = name();

  if ("" == newTitle){
	  qDebug() << "refusing to deal with blank note!!";
	return;
  }

  if (m_blockEmitDataChanged) return;

  // If we aren't changed, we got here by accident.
  // This shouldn't happen, but we really should protect against it.
  if (! isModified() ) return;

  m_blockEmitDataChanged = true;

  // OK, we actually changed. Handle that:
  //qDebug() << __PRETTY_FUNCTION__ << "We really did change. New title: " << newTitle;

  std::string oldTitle = m_gnote->get_title();

  //slotFormatTitle();

  // Sync title bar with title
  setWindowTitle(newTitle);
  /* Make sure the title is blue, big, and underlined
   * and ensure that other things are not... */

  slotSave();

  m_blockEmitDataChanged = false;
}

void KNote::slotKill()
{
  m_blockEmitDataChanged = true;

  //if ( !force &&
  if (  KMessageBox::warningContinueCancel( this,
         i18n( "<qt>Do you really want to delete note <b>%1</b>?</qt>",
               name() ),
//               m_label->text() ),
         i18n( "Confirm Delete" ),
         KGuiItem( i18n( "&Delete" ), "edit-delete" ),
         KStandardGuiItem::cancel(),
         "ConfirmDeleteNote" ) != KMessageBox::Continue )  {
     m_blockEmitDataChanged = false;
     return;
  }

  // delete the configuration first, then the corresponding file
  // delete m_config;
  // m_config = 0;
  //QString configFile = KGlobal::dirs()->saveLocation( "appdata", "notes/" );
  // configFile += m_journal->uid();
/*
  if ( !KIO::NetAccess::del( KUrl( configFile ), this ) ) {
    kError( 5500 ) <<"Can't remove the note config:" << configFile;
  }
*/

  /* We do this mess to get the note title b/c it's more likely to
   * be a match for some reason. Originally, we used name() but that
   * failed. Obviously, there are other problems, but we'll work that 
   * out when we get to it.
   * Ultimately, we should either use name, or better yet switch to 
   * always constant (per note) and reliable uids. 
   */
	emit sigKillNote(QString::fromStdString(m_gnote->get_title()));
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

void KNote::slotNameChanged(){
  std::string oldTitle = m_gnote->get_title();
  const QString newTitle = name();
  setWindowTitle(newTitle);
  emit sigNameChanged(newTitle, QString::fromStdString(oldTitle) );
}

void KNote::emitNewNote(){
	emit sigNewNote();
}

void KNote::slotSave(){
  // only save if we changed
  if (! isModified() ){
  //	qDebug() << __PRETTY_FUNCTION__ << " not modified. Not saving.";
	return;
  }

  // qDebug() << __PRETTY_FUNCTION__; 
  // Update the title everywhere
  //qDebug() << __PRETTY_FUNCTION__; 
  slotNameChanged();

  // cache content so we know if we are modified/saved in the future
  m_content = m_editor->toPlainText();

  m_gnote->set_text_content(m_content.toStdString());
  m_gnote->set_title(name().toStdString());
  m_gnote->save();

  // This cues the note up for a save next time it is requested
  // we do this to save resources so we don't save every single note
  // that is closed only those who have changed.
  m_gnote->changed();
  m_isModified = false;
}

void KNote::slotMakeNoteBook(){
	//qDebug()<< __PRETTY_FUNCTION__;
  
  QString nb = m_dlg->lineEdit->text();

  // Refuse to add stupid blank notebooks
  if (nb.trimmed().size() < 1) return;

	m_dlg->hide();
	emit sigNewNotebook(nb);
}

void KNote::slotNewNoteBook(){
	//qDebug()<< __PRETTY_FUNCTION__;

	m_dlg = new NewNoteBookDialog();

	connect(m_dlg->buttonCancel, SIGNAL(clicked()), m_dlg, SLOT(hide()));
	connect(m_dlg->buttonOK, SIGNAL(clicked()), this, SLOT(slotMakeNoteBook()));

	m_dlg->exec();

	delete m_dlg;

	return;
}

 void KNote::slotAddNotebook(const QString &text){
  qDebug() << __PRETTY_FUNCTION__ << " 1 ";
  gnote::notebooks::NotebookManager::instance().get_or_create_notebook(text.toStdString());
  qDebug() << __PRETTY_FUNCTION__ << " 2 ";
  gnote::notebooks::NotebookManager::instance().move_note_to_notebook(m_gnote, text.toStdString() );
  qDebug() << __PRETTY_FUNCTION__ << " 3 ";
 slotAddNotebookMenu(text);
}

 /* Add a new notebook to the menu */
 void KNote::slotAddNotebookMenu(const QString &text){
  gnote::notebooks::Notebook::Ptr notebook = gnote::notebooks::NotebookManager::instance().get_notebook_from_note(m_gnote);
 	KIcon iconNoteBook = KIcon(":/icons/notebook_edit.png");
  QAction *notebook_action = m_noteBookMenu->addAction(iconNoteBook, text);
	notebook_action->setCheckable(true);
	// add to qactiongroup to make checkable
	m_notebooks->addAction(notebook_action);
 	// FIXME: Need to connect to actually naming notebook
  connect(m_notebooks, SIGNAL(triggered(QAction*)), this, SLOT(slotMoveToNotebook(QAction*)));
  if (notebook){
	  //qDebug()<< __PRETTY_FUNCTION__ << "match? " << QString::fromStdString(notebook->get_name()) << " : " << text;
    if (QString::fromStdString(notebook->get_name()) == text){
	          //qDebug()<< __PRETTY_FUNCTION__ << " match! ";
            notebook_action->setChecked(true);
    }
  }
  //else
	  //qDebug()<< __PRETTY_FUNCTION__ << " no notebook for this note! " << text;
}


void KNote::loadNotebooks(){
  KTGlib::StringList nbs = gnote::notebooks::NotebookManager::instance().get_notebook_list();  
  for(KTGlib::StringList::const_iterator iter = nbs.begin();
           iter != nbs.end(); ++iter) {
           const std::string & nb (*iter);
           QString newNotebook = QString::fromStdString(nb);
           //qDebug() << "adding: " << newNotebook;
		       slotAddNotebookMenu(newNotebook);
  }
	return;
}

void KNote::slotMoveToNotebook(QAction *act){
  QString qs = act->text().remove("&"); 
  //qDebug() << __PRETTY_FUNCTION__ << qs;
  gnote::notebooks::NotebookManager::instance().move_note_to_notebook(m_gnote, qs.toStdString() );

  act->setIconText(qs);
  m_isModified=true;
  slotSave();
}

// BEGIN formatText()
/* This slot is called every second. We don't reformat title each time b/c that
 * would be annoying. Thus, we do a few checks and only do so when the time is
 * right aka, we won't be fighting with the user as they try to edit the title.
 * Our sister function format Title() does the same thing, but without the check
 * which is because we use format Title() for those times when we know we really
 * want the title formatted such as upon first startup. 
 */ 
void KNote::formatText(){
  QTextCursor cursor = m_editor->textCursor();
  int pos = cursor.position();
  int col = cursor.columnNumber();
  int line = m_editor->textCursor().blockNumber() + 1;
  //qDebug() << "BEGIN: " << __PRETTY_FUNCTION__ << " pos: " << pos << " line: " << line << " block: " << cursor.block().blockNumber();
  bool nasty_fix=false;


  // Nasty hack to make sure we wind up in the right spot at the end.
  // If we are on line 2, only, does this manifest
  if ( 2 == line ) nasty_fix=true;

  QString newContent;

  QTextCharFormat bodyFormat;
  bodyFormat.setFontUnderline(false); 
  bodyFormat.setForeground(QBrush(QColor(Qt::black))); 
  bodyFormat.setFontPointSize(14);

  cursor.setPosition(0, QTextCursor::MoveAnchor);  
  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);  
  QString s=cursor.selectedText();
  s.remove("\n");
  m_title = s;
  QString newtitle = startTitle+s+endTitle.trimmed();

  cursor.removeSelectedText();	

  cursor.insertHtml(newtitle);  
  int bodyPos = cursor.position();
  m_htmlTitle = newtitle;

  cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);  
  cursor.setCharFormat(bodyFormat);
  cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);  
  newContent = cursor.selectedText();

  cursor.removeSelectedText();	

  // FRED
  //cursor.insertHtml(newContent);  
  //cursor.insertBlock();
  cursor.insertText(newContent);  

  cursor.setPosition(bodyPos, QTextCursor::KeepAnchor );
  cursor.setCharFormat(bodyFormat);

  cursor.setPosition(pos, QTextCursor::MoveAnchor);  

  // This line is needed to actually set the text cursor
  m_editor->setTextCursor(cursor); 

/*
  line = m_editor->textCursor().blockNumber() + 1;
  if (nasty_fix && line > 2){
    qDebug() << " need to go up: "<< line-2;
    qDebug() << "new pos: " << __PRETTY_FUNCTION__ << " pos: " << cursor.position() << " line: " << m_editor->textCursor().blockNumber() + 1;
    //int pos = cursor.position();
    //cursor.setPosition(0, QTextCursor::KeepAnchor);  
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor, 1);  
    //cursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor, 1);  
  }
  */

  //qDebug() << "END: " << __PRETTY_FUNCTION__ << " pos: " << pos << " line: " << line << " block: " << cursor.block().blockNumber();
}
// END formatText()

void KNote::slotTextChanged(){
 // qDebug() << __PRETTY_FUNCTION__;
  //QTextCursor cursor = m_editor->textCursor();
}

// END KNOTE SLOTS
}// namespace knotes
// Sun Mar 10 12:50:51 PDT 2013
