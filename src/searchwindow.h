#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QTableWidget>

#include "ui_searchwindow.h"

namespace ktomgirl{
class SearchWindow : public QMainWindow, public Ui::SearchWindow
{
	Q_OBJECT
public:
	SearchWindow(QWidget* pParent = NULL, const char* szName = NULL);
	~SearchWindow();

	static void setStringList(int, QStringList&, QTableWidget*, QString);
private:
	QStringList m_list;
	QStringListModel *m_model;
signals:
	void signalNoteSelected(QString);

public slots:
	void emitNoteSelected(QTableWidgetItem*);
/*
private slots:
	void slotLs(void);
	void slotLsDone(uint);
	void slotReadStd(void);
	void slotStarted(void);
*/
}; // class SearchWindow
} // namespace ktomgirl
#endif // SEARCH_WINDOW_H
// Wed Mar 28 14:36:01 PDT 2012
