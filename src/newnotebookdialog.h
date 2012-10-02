#ifndef _KTOMGIRL_NEW_NOTEBOOK_DIALOG_H
#define _KTOMGIRL_NEW_NOTEBOOK_DIALOG_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <QDockWidget>

#include "ui_newnotebookdialog.h"

class NewNoteBookDialog : public QDialog, public Ui::NewNoteBookDialog
{
	Q_OBJECT
public:
	NewNoteBookDialog(QWidget* pParent = NULL, const char* szName = NULL);
	~NewNoteBookDialog();

}; // class NewNoteBookDialog
#endif // _KTOMGIRL_NEW_NOTEBOOK_DIALOG_H
