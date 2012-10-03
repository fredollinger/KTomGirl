// BEGIN QT INCLUDES
#include <qdebug.h>
#include <QDialog>
// END QT INCLUDES

// KTOMGIRL INCLUDES
#include "newnotebookdialog.h"

NewNoteBookDialog::NewNoteBookDialog(QWidget* pParent, const char* szName) 
{
	setupUi(this);
}

NewNoteBookDialog::~NewNoteBookDialog() 
{
	qDebug() << __PRETTY_FUNCTION__;
}
// Tue Oct  2 19:08:11 PDT 2012
