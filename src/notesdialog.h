#ifndef _KTOMGIRL_NOTES_DIALOG_H
#define _KTOMGIRL_NOTES_DIALOG_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <QDockWidget>

#include "ui_notesdialog.h"

namespace ktomgirl{
	class KTGItem;
}

class NotesDialog : public QDockWidget, public Ui::NotesDialog
{
	Q_OBJECT
public:
	NotesDialog(QWidget* pParent = NULL, const char* szName = NULL);
	~NotesDialog();

}; // class NotesDialog
#endif //_KTOMGIRL_NOTES_DIALOG_H
// Sun May 13 15:55:55 PDT 2012
