#ifndef _KTOMGIRL_SEARCH_BAR_H
#define _KTOMGIRL_SEARCH_BAR_H

#include <QMainWindow>
#include <QDockWidget>

#include "ui_searchbar.h"

class SearchBar : public QDockWidget, public Ui::SearchBar
{
	Q_OBJECT
public:
	SearchBar(QWidget* pParent = NULL, const char* szName = NULL);
	~SearchBar();

}; // class SearchBar
#endif // _KTOMGIRL_SEARCH_BAR_H
