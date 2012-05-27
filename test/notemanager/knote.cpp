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


  createGUI( ":/xml/" + componentData().componentName() + "ui.rc");

  KToolBar *toolbar =  new KToolBar(this, true, false);

  KIcon trash = KIcon("edit-delete");
  toolbar->addAction(trash, i18n("Trash"));

  KIcon search = KIcon("system-search");
  toolbar->addAction(search, i18n("Search"));

}

void
KNote::slotDeleteNote(){
	qDebug() << __PRETTY_FUNCTION__;
}
