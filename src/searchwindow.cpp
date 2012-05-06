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

SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
 	m_model = new QStringListModel();

	m_list << gnote::Gnote::get_note_list();

	setupUi(this);
	tableNotes->setRowCount(m_list.count()+1);
	tableNotes->setColumnCount(2);
	QStringList qsl; 
	qsl << "Note" << "Last Changed";
	tableNotes->setHorizontalHeaderLabels(qsl);
	tableNotes->verticalHeader()->setVisible(false);
	tableNotes->setShowGrid(false);

	// setStringList(0, m_list, tableNotes, gnote::Gnote::tomboy_data_dir() );
 	connect (tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));
}

SearchWindow::~SearchWindow()
{

}

// FIXME: NOT DONE
void SearchWindow::loadNotes(const gnote::Note::List &notesCopy){
	// qDebug() << __PRETTY_FUNCTION__;
	QString qs;
        
	for(gnote::Note::List::const_iterator iter = notesCopy.begin();
		iter != notesCopy.end(); ++iter) {

		const gnote::Note::Ptr & note(*iter);
		qs = QString::fromStdString(note->get_title());
		// qDebug() << __PRETTY_FUNCTION__ << qs << m_row;

		// BEGIN ITEM ONE
		ktomgirl::KTGItem *item = new ktomgirl::KTGItem(qs, note);
		item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		tableNotes->setItem ( m_row, 0, item );
		// END ITEM ONE

		// BEGIN ITEM TWO
		qs = "";
		item = new ktomgirl::KTGItem(qs, note);
		item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		tableNotes->setItem ( m_row, 1, item );
		// END ITEM TWO

		

		m_row++;
	}
	tableNotes->setRowCount(m_row);
	return;
}

void
SearchWindow::emitNoteSelected(QTableWidgetItem* item){
	emit signalNoteSelected(static_cast<ktomgirl::KTGItem*>(item));

}

void
SearchWindow::setItemName(const QString &neu, const QString &old){
	
	QList<QTableWidgetItem*> ql = tableNotes->findItems ( old, Qt::MatchExactly);
	if (ql.count() < 1){
		qDebug() << __PRETTY_FUNCTION__<< "ERR: no note named" <<  neu << ql.count();
		return;
	}
	ql[0]->setText(neu);
	//ql[0]->get_note()->set_title(neu);
	static_cast<ktomgirl::KTGItem*>(ql[0])->get_note()->set_title(neu.toStdString());
}
// Sun May  6 10:11:19 PDT 2012
