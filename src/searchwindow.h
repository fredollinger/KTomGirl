#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>

#include "ui_searchwindow.h"

namespace ktomgirl{
class SearchWindow : public QMainWindow, public Ui::SearchWindow
{
	Q_OBJECT
public:
	SearchWindow(QWidget* pParent = NULL, const char* szName = NULL);
	~SearchWindow();
private:
	QStringList m_list;
	QStringListModel *m_model;
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
