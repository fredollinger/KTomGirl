// BEGIN QT INCLUDES
#include <qdebug.h>
#include <QMainWindow>
#include <QTableWidget>
// END QT INCLUDES

#include <KParts/MainWindow>
//#include <iostream>

// KTOMGIRL INCLUDES
#include "searchwindow.h"
#include "notesdialog.h"
//#include "ktgitem.h"

// GNOTE INCLUDES
//#include "gnote.hpp"
//#include "note.hpp"

SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
	m_notesDialog = new NotesDialog();
	m_notesDialog->show();
        m_notesDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        addDockWidget(Qt::BottomDockWidgetArea, m_notesDialog);
}

SearchWindow::~SearchWindow()
{
}
