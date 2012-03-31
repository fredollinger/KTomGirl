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
     	// m_model->setStringList(m_list);

	setupUi(this);
	tableNotes->setRowCount(m_list.count());
	tableNotes->setColumnCount(10);

	// tableNotes->setModel(m_model);
//	tableNotes << m_list;
	
	setStringList(0, m_list, tableNotes, gnote::Gnote::tomboy_data_dir() );
 	connect (tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));
}

SearchWindow::~SearchWindow()
{

}

void SearchWindow::setStringList(int col, QStringList &qsl, QTableWidget *qtw, QString filepath){
	int row = 0;
	foreach (QString name, qsl){
		qDebug() << name << row << col;
		QTableWidgetItem *item = new QTableWidgetItem(name);
		qtw->setItem ( row, col, item );
		qtw->setToolTip(filepath);
		row++;
	}
}

void
SearchWindow::emitNoteSelected(QTableWidgetItem* item){
	qDebug() << __PRETTY_FUNCTION__;
	// emit signalNoteSelected(tableNotes->item(row, col)->data(Qt::DisplayRole).toString());
	emit signalNoteSelected(item->toolTip() + "/" + item->data(Qt::DisplayRole).toString());

}

} // namespace ktomgirl
// Thu Mar 29 19:54:15 PDT 2012
