#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>
#include <KParts/MainWindow>

#include "ui_searchwindow.h"
#include "notesdialog.h"
#include "notesdialog.h"
#include "notebooksdialog.h"
#include "searchbar.h"

// GNOTE INCLUDES
#include <libktomgirl/note.hpp>
#include <libktomgirl/gnote.hpp>

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
	//void setStringList(int, QStringList&, QTableWidget*, QString);

	// Given an item title, remove that item
	void deleteItem(const QString&);

	/* iterate through note list and load up the titles into the qtablewidget */
	void loadNotes(const gnote::Note::List&);

	// iterate through notebooks and add them to the qtablewidget
	//void loadNotebooks(const QStringList&);
	void loadNotebooks();

	void newItem(gnote::Note::Ptr&);

	// add the background gray and anything else to the notes...
	void styleNotes(void);

  void showAllNotes(void);
  void showUnfilteredNotes(void);
  void showFilteredNotes(const QString&);

private:
	KTGlib::StringList m_list;
	// QStringListModel *m_model;
	int m_row; // current row
	NotesDialog *m_notesDialog;
	NotebooksDialog *m_notebooksDialog;
	SearchBar *m_searchBar;

signals:
	void signalNoteSelected(ktomgirl::KTGItem*);
	void signalHandleSearch(QString);

public slots:
	void slotAddNotebook(const QString&);
	void emitNoteSelected(QTableWidgetItem*);
	void setItemName(const QString&, const QString&);
	void slotItemClicked(QTableWidgetItem*);
	void slotHandleSearch(void);
  void notebookClicked(int, int);

}; // class SearchWindow
#endif // SEARCH_WINDOW_H
// Thu Dec 13 18:07:53 PST 2012
