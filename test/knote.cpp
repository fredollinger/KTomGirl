#include "knote.h"

#include <KActionCollection>
#include <KApplication>
#include <KLocale>
#include <KTextEdit>

#include <QDebug>

KNote::KNote(QWidget *parent)
    : KXmlGuiWindow(parent)
{
  qDebug() << "setting: " << componentData().componentName() + "ui.rc";
//  createGUI( componentData().componentName() + "ui.rc", false, false );
  createGUI( componentData().componentName() + "ui.rc");
}
