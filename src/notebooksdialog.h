#ifndef _KTOMGIRL_NOTEBOOK_DIALOG_H
#define _KTOMGIRL_NOTEBOOK_DIALOG_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <QDockWidget>

#include <ui_notebooksdialog.h>

namespace ktomgirl{
	class KTGItem;
}

class NotebooksDialog : public QDockWidget, public Ui::NotebooksDialog
{
	Q_OBJECT
public:
	NotebooksDialog(QWidget* pParent = NULL, const char* szName = NULL);
	~NotebooksDialog();

}; // class NotebooksDialog
#endif // _KTOMGIRL_NOTEBOOK_DIALOG_H
