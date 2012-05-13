#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <QDockWidget>

#include "ui_notedialog.h"

namespace ktomgirl{
	class KTGItem;
}

class NoteDialog : public QDialog, public Ui::NoteDialog
{
	Q_OBJECT
public:
	NoteDialog(QWidget* pParent = NULL, const char* szName = NULL);
	~NoteDialog();

}; // class NoteDialog
#endif // SEARCH_WINDOW_H
// Wed Mar 28 14:36:01 PDT 2012
