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
#include "notebooksdialog.h"
#include "searchbar.h"

// GNOTE INCLUDES
#include <libktomgirl/note.hpp>
#include <libktomgirl/gnote.hpp>
#include <libktomgirl/search.hpp>

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
	void loadNotes(const gnote::Search::ResultsPtr&);

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
    int m_row; // current row
    NotesDialog *m_notesDialog;
    NotebooksDialog *m_notebooksDialog;
    SearchBar *m_searchBar;

    typedef QList<ktomgirl::KTGItem*> NoteItemList;
    NoteItemList m_noteList;
    NoteItemList m_dateList;

signals:
    void sigNotebookClicked(const QString&);
    void signalNotebookClicked(QString&);
    void signalNoteSelected(ktomgirl::KTGItem*);
    void signalHandleSearch(QString);

public slots:
    void emitNoteSelected(QTableWidgetItem*);
    void setItemName(const QString&, const QString&);
    void slotHandleSearch(void);
    void notebookClicked(int, int);
    void noteClicked(int, int);
    void slotAddNotebook(const QString&);
    void slotHeaderClicked(QTableWidgetItem*);
    void slotItemClicked(QTableWidgetItem*);
    void slotSortNotes(void);
    void slotSpewNoteDetail(void);

}; // class SearchWindow
#endif // SEARCH_WINDOW_H
// Tue Sep 29 14:45:24 PDT 2015
