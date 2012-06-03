/*
 * The KTomGirl Project
 * 
 * SeachWindow
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
// END QT INCLUDES

#include <KParts/MainWindow>
//#include <iostream>

// KTOMGIRL INCLUDES
#include "searchwindow.h"
#include "notesdialog.h"
#include "notebooksdialog.h"
#include "../src/ktgitem.h"

// GNOTE INCLUDES
#include "../libtomgirl/gnote.hpp"
#include "../libtomgirl/note.hpp"

// BEGIN SEARCH WINDOW
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) :
m_row(0)
{
	QString qs;
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

	qs="All Notes";
	QTableWidgetItem *item = new QTableWidgetItem(qs);
	m_notebooksDialog->tableNotebooks->setItem ( 0 , 0, item );
	// END NOTEBOOKS DIALOG

 	connect (m_notesDialog->tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));

	// connect( actionQuit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );


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
	//QDateTime qdt = QString::fromStdString(note->date()->create_date());

	// BEGIN DEBUG
  	sharp::DateTime dt = note->data().change_date();
	qDebug() << QString::fromStdString(dt.to_iso8601());
	// END DEBUG

	// FIXME: Need to get the date time in here...

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

void
SearchWindow::deleteItem(const QString &qs){

	QList<QTableWidgetItem*> ql = 
 	m_notesDialog->tableNotes->findItems ( qs, Qt::MatchExactly);
	if (ql.count() < 1){
		qDebug() << __PRETTY_FUNCTION__<< "ERR: no note named" <<  qs << ql.count();
		return;
	}
	
	int row = m_notesDialog->tableNotes->row(ql[0]);
	qDebug() << __PRETTY_FUNCTION__ << ql[0]->text() << row;
	m_notesDialog->tableNotes->removeRow (row); 
	// void QTableWidget::removeRow ( int row ) [slot]

	return;
}

//QList<QTableWidgetItem *> QTableWidget::findItems ( const QString & text, Qt::MatchFlags flags ) const
// Sat May 26 17:49:08 PDT 2012
