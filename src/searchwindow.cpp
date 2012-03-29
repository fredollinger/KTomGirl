#include <qdebug.h>

#include <QMainWindow>
#include <QTableWidget>

#include "searchwindow.h"
#include "gnote.hpp"

namespace ktomgirl{
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) 
{
 	m_model = new QStringListModel();

	m_list << gnote::Gnote::get_note_list();
     	// m_list << "a" << "b" << "c";
     	m_model->setStringList(m_list);

	setupUi(this);

	tableNotes->setModel(m_model);
 	connect (tableNotes, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(emitNoteSelected(int, int)));
 	connect (tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));
}

SearchWindow::~SearchWindow()
{

}

void
SearchWindow::emitNoteSelected(int row, int col){
	emit signalNoteSelected(row, col);
}

void
SearchWindow::emitNoteSelected(QTableWidgetItem *item){
	qDebug() << __PRETTY_FUNCTION__;
	// emit signalItemNoteSelected(QTableViewItem*);
}

} // namespace ktomgirl
// Wed Mar 28 14:37:50 PDT 2012
