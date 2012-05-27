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
#include "notebooksdialog.h"
#include "../src/ktgitem.h"

// GNOTE INCLUDES
//#include "gnote.hpp"
//#include "note.hpp"

// BEGIN SEARCH WINDOW
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
	QStringList qsl; 
	setupUi(this);
	widget->hide();

	// BEGIN NOTES DIALOG
	m_notesDialog = new NotesDialog();
	m_notesDialog->showMaximized();
        m_notesDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        addDockWidget(Qt::RightDockWidgetArea, m_notesDialog);

	qsl << "Note" << "Last Changed";
	m_notesDialog->tableNotes->setColumnCount(2);
	m_notesDialog->tableNotes->setHorizontalHeaderLabels(qsl);
	m_notesDialog->tableNotes->horizontalHeader()->setVisible(true);
	m_notesDialog->tableNotes->verticalHeader()->setVisible(false);
	//m_notesDialog->tableNotes->setRowCount(m_list.count()+1);
	// END NOTES DIALOG

	// BEGIN NOTEBOOKS DIALOG
	m_notebooksDialog = new NotebooksDialog();
	m_notebooksDialog->showMaximized();
        m_notebooksDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
        addDockWidget(Qt::LeftDockWidgetArea, m_notebooksDialog);

	qsl.clear();
	qsl << "Notebooks";
	m_notebooksDialog->tableNotebooks->setColumnCount(1);
	m_notebooksDialog->tableNotebooks->setHorizontalHeaderLabels(qsl);
	m_notebooksDialog->tableNotebooks->horizontalHeader()->setVisible(true);
	// END NOTEBOOKS DIALOG


} // END SEARCH WINDOW

SearchWindow::~SearchWindow()
{
}
