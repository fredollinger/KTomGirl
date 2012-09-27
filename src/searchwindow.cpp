/*
 * The KTomGirl Project
 * 
 * SearchWindow
 * 
 * This is the window which shows a list of all the notes or the 
 * particular notes that match a search term.
 *
 * Copyright (C) 2012 Fred Ollinger
 * <follinge@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


// BEGIN QT INCLUDES
#include <qdebug.h>
#include <QMainWindow>
#include <QTableWidget>
#include <QDateTime>
// END QT INCLUDES

#include <KParts/MainWindow>
//#include <iostream>

// KTOMGIRL INCLUDES
#include "ktgitem.h"
#include "searchwindow.h"
#include "notesdialog.h"
#include "notebooksdialog.h"

// GNOTE INCLUDES
#include <libktomgirl/gnote.hpp>
#include <libktomgirl/note.hpp>

// BEGIN SEARCH WINDOW
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
	QString qs;
	QStringList qsl; 
	setupUi(this);
	setWindowIcon(KIcon(":/icons/notebook.png"));
	widget->hide();

	m_list = gnote::Gnote::get_note_list();

	// BEGIN NOTES DIALOG
	m_notesDialog = new NotesDialog();
	m_notesDialog->showMaximized();
    m_notesDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
   	addDockWidget(Qt::RightDockWidgetArea, m_notesDialog);

	m_notesDialog->tableNotes->setRowCount(m_list.size()+1);
	m_notesDialog->tableNotes->setShowGrid(false);
	m_notesDialog->tableNotes->horizontalHeader()->setStretchLastSection(true);
	m_notesDialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTableWidgetItem *noteHeader = m_notesDialog->tableNotes->horizontalHeaderItem(0);
	noteHeader->setTextAlignment(Qt::AlignLeft);

    QTableWidgetItem *dateHeader = m_notesDialog->tableNotes->horizontalHeaderItem(1);
	dateHeader->setTextAlignment(Qt::AlignLeft);
	// END NOTES DIALOG

	// BEGIN NOTEBOOKS DIALOG
	m_notebooksDialog = new NotebooksDialog();
	m_notebooksDialog->showMaximized();
   	m_notebooksDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
   	addDockWidget(Qt::LeftDockWidgetArea, m_notebooksDialog);

	m_notebooksDialog->tableNotebooks->setCurrentItem(m_notebooksDialog->tableNotebooks->item(0,0));
    QTableWidgetItem *noteBooksHeader = m_notebooksDialog->tableNotebooks->horizontalHeaderItem(0);
	noteBooksHeader->setTextAlignment(Qt::AlignLeft);
	
 	connect (m_notesDialog->tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));

 	connect (m_notesDialog->tableNotes, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotItemClicked(QTableWidgetItem*)));
	// END NOTEBOOKS DIALOG

} // END SEARCH WINDOW

SearchWindow::~SearchWindow(){}

void SearchWindow::styleNotes(){
	// Loop through all notes and set the background colors...
	int m_rows = m_notesDialog->tableNotes->rowCount();
	for(int i=0;  i < m_rows; i++){
		QTableWidgetItem *item = m_notesDialog->tableNotes->item(i, 0);
		QTableWidgetItem *item2 = m_notesDialog->tableNotes->item(i, 1);
		if (0 != item){
			item->setData(Qt::BackgroundRole, (i%2)>0 ? Qt::white : Qt::lightGray);
			item2->setData(Qt::BackgroundRole, (i%2)>0 ? Qt::white : Qt::lightGray);
		}
	}
}

void SearchWindow::loadNotes(const gnote::Note::List &notesCopy){
	QString qs;

  	KIcon notebookIcon = KIcon(":/icons/notebook.png");
        
	for(gnote::Note::List::const_iterator iter = notesCopy.begin();
		iter != notesCopy.end(); ++iter) {

		const gnote::Note::Ptr & note(*iter);
		qs = QString::fromStdString(note->get_title());

		// BEGIN ITEM ONE
		ktomgirl::KTGItem *item = new ktomgirl::KTGItem(qs, note);
		//item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		item->setIcon(notebookIcon);
		m_notesDialog->tableNotes->setItem ( m_row, 0, item );
		// END ITEM ONE

		// BEGIN ITEM TWO
  		sharp::DateTime qdt = note->data().change_date();
		//qs = QString::fromStdString(qdt.to_iso8601());
		qs = QString::fromStdString(qdt.to_string());
		item = new ktomgirl::KTGItem(qs, note);
		//item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
		m_notesDialog->tableNotes->setItem ( m_row, 1, item );
		// END ITEM TWO

		m_row++;
	}
	m_notesDialog->tableNotes->setRowCount(m_row);
	styleNotes();
	return;
}

/* Handle the case where the itme is clicked once.
   We'd like to highlight the entire row aka both columns.
   The one clicked is all ready highlighted to we just need to find the other one. 
*/
void SearchWindow::slotItemClicked(QTableWidgetItem* item){
	QTableWidget *qtw = item->tableWidget();
	qDebug() << __PRETTY_FUNCTION__ << " qtablewidget selected: " << qtw->objectName();
	
	/* Only operate on tables which we know how to work on. */
	if (qtw->objectName() != "tableNotes") return;

	int col=0;
	int row=0;

	// Select the opposite column of the one selected all ready
	if (0 == item->column()) col = 1;

	row = item->row();

	QTableWidgetItem *qtwi = qtw->item(row, col);
	qtwi->setSelected(true);
	
	return;
}

void SearchWindow::emitNoteSelected(QTableWidgetItem* item){
	emit signalNoteSelected(static_cast<ktomgirl::KTGItem*>(item));
}

void SearchWindow::setItemName(const QString &neu, const QString &old){
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

  	KIcon notebookIcon = KIcon(":/icons/notebook.png");


	QDateTime qdt = QDateTime::currentDateTime();
	//QDateTime qdt = QString::fromStdString(note->date()->create_date());

	// BEGIN DEBUG
  	sharp::DateTime dt = note->data().change_date();
	// END DEBUG

	// FIXME: Need to get the date time in here...

	// BEGIN FIRST ITEM
	ktomgirl::KTGItem *item = new ktomgirl::KTGItem(name, note);
//	item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
	m_notesDialog->tableNotes->insertRow ( 0 );
	m_notesDialog->tableNotes->setItem ( 0 , 0, item );
	item->setIcon(notebookIcon);
	// END FIRST ITEM

	// BEGIN SECOND ITEM
	name = qdt.toString();
	item = new ktomgirl::KTGItem(name, note);
//	item->setData(Qt::BackgroundRole, (m_row%2)>0 ? Qt::white : Qt::lightGray);
	m_notesDialog->tableNotes->setItem ( 0 , 1, item );
	// END SECOND ITEM

	m_row++;
	return;
}

void
SearchWindow::deleteItem(const QString &qs){

	QList<QTableWidgetItem*> ql = 
 	m_notesDialog->tableNotes->findItems ( qs, Qt::MatchExactly);
	if (ql.count() < 1){
		qDebug() << __PRETTY_FUNCTION__<< "ERR: no note named" <<  qs << ql.count();
		return;
	}
	
	int row = m_notesDialog->tableNotes->row(ql[0]);
	m_notesDialog->tableNotes->removeRow (row); 
	// void QTableWidget::removeRow ( int row ) [slot]
	styleNotes();
	return;
}

//QList<QTableWidgetItem *> QTableWidget::findItems ( const QString & text, Qt::MatchFlags flags ) const
// Sat May 26 17:49:08 PDT 2012
