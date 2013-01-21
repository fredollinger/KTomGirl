// BEGIN QT INCLUDES
#include <qdebug.h>
#include <QDialog>
#include <QTableWidget>
// END QT INCLUDES

// KTOMGIRL INCLUDES
#include "notesdialog.h"

NotesDialog::NotesDialog(QWidget* pParent, const char* szName) 
{
	setupUi(this);
}

NotesDialog::~NotesDialog()
{
}

void NotesDialog::contextMenuEvent ( QContextMenuEvent * ev ){
  qDebug() << __PRETTY_FUNCTION__;
  //QPoint globalPos = myWidget->mapToGlobal(pos);
  return;
}
