// BEGIN QT INCLUDES
#include <qdebug.h>
#include <QMainWindow>
#include <QTableWidget>
// END QT INCLUDES

#include <iostream>

// KTOMGIRL INCLUDES
#include "searchwindow.h"
#include "ktgitem.h"

// GNOTE INCLUDES
#include "gnote.hpp"
#include "note.hpp"

namespace ktomgirl{
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
 	m_model = new QStringListModel();

	m_list << gnote::Gnote::get_note_list();

	setupUi(this);
	tableNotes->setRowCount(m_list.count()+100);
	tableNotes->setColumnCount(10);

	// setStringList(0, m_list, tableNotes, gnote::Gnote::tomboy_data_dir() );
 	connect (tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));
}

SearchWindow::~SearchWindow()
{

}

// FIXME: NOT DONE
void SearchWindow::loadNotes(const gnote::Note::List &notesCopy){
	qDebug() << __PRETTY_FUNCTION__;
	QString qs;
        
	for(gnote::Note::List::const_iterator iter = notesCopy.begin();
		iter != notesCopy.end(); ++iter) {

		const gnote::Note::Ptr & note(*iter);
		qs = QString::fromStdString(note->get_title());
		qDebug() << __PRETTY_FUNCTION__ << qs << m_row;

		KTGItem *item = new KTGItem(qs, note);
		tableNotes->setItem ( m_row, 0, item );
		m_row++;
	}
	return;
}

/*
void SearchWindow::setStringList(int col, QStringList &qsl, QTableWidget *qtw, QString filepath){
	foreach (QString name, qsl){
		KTGItem *item = new KTGItem(name);
		qtw->setItem ( m_row, col, item );
		qtw->setToolTip(filepath+"/"+name);
		m_row++;
	}
}
*/

void
SearchWindow::emitNoteSelected(QTableWidgetItem* item){
	emit signalNoteSelected(static_cast<KTGItem*>(item));

}

} // namespace ktomgirl
// Thu Mar 29 19:54:15 PDT 2012
