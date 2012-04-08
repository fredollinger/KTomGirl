#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>

#include "ui_searchwindow.h"

// GNOTE INCLUDES
#include "note.hpp"


namespace ktomgirl{
class KTGItem;
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

private:
	QStringList m_list;
	QStringListModel *m_model;
	int m_row; // current row
signals:
	void signalNoteSelected(KTGItem*);

public slots:
	void emitNoteSelected(QTableWidgetItem*);

// private slots:

}; // class SearchWindow
} // namespace ktomgirl
#endif // SEARCH_WINDOW_H
// Wed Mar 28 14:36:01 PDT 2012
