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
#include "gnote.hpp"
#include "note.hpp"

// BEGIN SEARCH WINDOW
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
	QStringList qsl; 
	setupUi(this);
	widget->hide();

 	// m_model = new QStringListModel();
	m_list << gnote::Gnote::get_note_list();
 	// m_model = new QStringListModel();
	m_list << gnote::Gnote::get_note_list();

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
	m_notesDialog->tableNotes->setRowCount(m_list.count()+1);
	m_notesDialog->tableNotes->setShowGrid(false);
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

void SearchWindow::loadNotes(const gnote::Note::List &notesCopy){
	QString qs;
        
	for(gnote::Note::List::const_iterator iter = notesCopy.begin();
		iter != notesCopy.end(); ++iter) {

		const gnote::Note::Ptr & note(*iter);
		qs = QString::fromStdString(note->get_title());

		// BEGIN ITEM ONE
		ktomgirl::KTGItem *item = new ktomgirl::KTGItem(qs, note);
		item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		m_notesDialog->tableNotes->setItem ( m_row, 0, item );
		// END ITEM ONE

		// BEGIN ITEM TWO
		qs = "";
		item = new ktomgirl::KTGItem(qs, note);
		item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		m_notesDialog->tableNotes->setItem ( m_row, 1, item );
		// END ITEM TWO

		m_row++;
	}
	m_notesDialog->tableNotes->setRowCount(m_row);
	return;
}

void
SearchWindow::emitNoteSelected(QTableWidgetItem* item){
	emit signalNoteSelected(static_cast<ktomgirl::KTGItem*>(item));
}

void
SearchWindow::setItemName(const QString &neu, const QString &old){
	
	qDebug() << __PRETTY_FUNCTION__<< "Changing" <<  old << "to: " << neu;
	QList<QTableWidgetItem*> ql = 
 	m_notesDialog->tableNotes->findItems ( old, Qt::MatchExactly);
	if (ql.count() < 1){
		qDebug() << __PRETTY_FUNCTION__<< "ERR: no note named" <<  neu << ql.count();
		return;
	}
	ql[0]->setText(neu);
}

void
SearchWindow::newItem(gnote::Note::Ptr & note){
	QString name = QString::fromStdString(note->get_title());

	qDebug() << __PRETTY_FUNCTION__<< "creating: " << name;

	QDateTime qdt = QDateTime::currentDateTime();

	// BEGIN FIRST ITEM
	ktomgirl::KTGItem *item = new ktomgirl::KTGItem(name, note);
	item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
	m_notesDialog->tableNotes->insertRow ( 0 );
	m_notesDialog->tableNotes->setItem ( 0 , 0, item );
	// END FIRST ITEM

	// BEGIN SECOND ITEM
	name = qdt.toString();
	item = new ktomgirl::KTGItem(name, note);
	item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
	m_notesDialog->tableNotes->setItem ( 0 , 1, item );
	// END SECOND ITEM

	m_row++;
	return;
}
// Sat May 19 08:22:42 PDT 2012
