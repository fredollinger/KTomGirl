#include "knote.h"

#include <KActionCollection>
#include <KApplication>
#include <KAction>
#include <KIcon>
#include <KLocale>
#include <KTextEdit>

#include <QDebug>


KNote::KNote(QWidget *parent)
    : KXmlGuiWindow(parent)

{
  qDebug() << "setting: " << componentData().componentName() + "ui.rc";
  createGUI( componentData().componentName() + "ui.rc");
  KAction *action  = new KAction( KIcon( "delete_note" ), i18n( "Trash" ),  this );
  actionCollection()->addAction( "delete_note", action );
  connect( action, SIGNAL( triggered( bool ) ), SLOT( slotDeleteNote() ) );
}

void
KNote::slotDeleteNote(){
}
