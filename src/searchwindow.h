#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <KParts/MainWindow>

#include "ui_searchwindow.h"
#include "notesdialog.h"
#include "notebooksdialog.h"

// GNOTE INCLUDES
#include "note.hpp"


namespace ktomgirl{
	class KTGItem;
}

class SearchWindow : public QMainWindow, public Ui::SearchWindow
{
	Q_OBJECT
public:
	SearchWindow(QWidget* pParent = NULL, const char* szName = NULL);
	~SearchWindow();

	/* add notes to a row from a string list */
	void setStringList(int, QStringList&, QTableWidget*, QString);

	/* iterate through note list and load up the titles into the qtablewidget */
	void loadNotes(const gnote::Note::List&);

	void newItem(gnote::Note::Ptr&);

	// Given an item title, remove that item
	void deleteItem(const QString&);

private:
	QStringList m_list;
	// QStringListModel *m_model;
	int m_row; // current row
	NotesDialog *m_notesDialog;
	NotebooksDialog *m_notebooksDialog;
signals:
	void signalNoteSelected(ktomgirl::KTGItem*);


public slots:
	void emitNoteSelected(QTableWidgetItem*);
	void setItemName(const QString&, const QString&);


}; // class SearchWindow
#endif // SEARCH_WINDOW_H
// Wed Mar 28 14:36:01 PDT 2012
