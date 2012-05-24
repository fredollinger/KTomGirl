#include "knote.h"

#include <QToolBar>

#include <KActionCollection>
#include <KApplication>
#include <KAction>
#include <KIcon>
#include <KLocale>
#include <KTextEdit>
#include <KToolBar>
#include <kxmlguibuilder.h>
#include <kxmlguifactory.h>

#include <QDebug>


KNote::KNote(QWidget *parent)
    : KXmlGuiWindow(parent)

{
  KXMLGUIBuilder builder( this );
  KXMLGUIFactory factory( &builder, this );
  factory.addClient( this );

  qDebug() << "setting: " << componentData().componentName() + "ui.rc";
   createGUI( componentData().componentName() + "ui.rc");
  // KToolBar *toolbar = dynamic_cast<KToolBar*>( factory.container( "note_tool", this ) ); 
  KIcon icon = KIcon("delete_note");
  KIcon search = KIcon("search_note");
  //QToolBar *toolbar = addToolBar("main toolbar");
  KToolBar *toolbar =  new KToolBar(this, true, false);
  addToolBar(toolbar);
  icon.addFile(":/ktg/trashcan_empty.xpm", QSize(64, 64), QIcon::Normal, QIcon::On);
  //search.addFile("trashcan_empty.xpm", QSize(64, 64), QIcon::Normal, QIcon::On);
  search.addPixmap(QPixmap(":/ktg/trashcan_empty.xpm"));
  KAction *action  = new KAction( icon, i18n( "Trash" ),  this );
  KAction *search_action  = new KAction( search, i18n( "Search" ),  this );
  toolbar->addAction(icon, "Trash");
  toolbar->addAction(search, "Search");
  //action->setText(i18n("Trash"));
  //actionCollection()->addAction( "delete_note", action );
	  QAction *trash = toolbar->addAction(icon, QString("Quit Application"));
	  //QAction *search_action = toolbar->addAction(icon, QString("Quit Application"));
 //KAction *newAct = new KAction(i18n("&New"), KIcon("document-new"),
  //                             KStandardShortcut::shortcut(KStandardShortcut::New), this,
   //                            SLOT(slotDeleteNote()), actionCollection());
	//KAction *newAct = KStandardAction::openNew(toolbar, SLOT(slotDeleteNote()),
     //                                  actionCollection());

  //toolbar->addAction(newAct, "Search");
  connect( search_action, SIGNAL( triggered( bool ) ), SLOT( slotDeleteNote() ) );
}

void
KNote::slotDeleteNote(){
	qDebug() << __PRETTY_FUNCTION__;
}
