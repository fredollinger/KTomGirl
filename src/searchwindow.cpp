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
#include <QDateTime>
#include <QMainWindow>
#include <QScrollBar>
#include <QTableWidget>
// END QT INCLUDES

#include <KParts/MainWindow>
//#include <iostream>

// KTOMGIRL INCLUDES
#include "ktgitem.h"
#include "searchbar.h"
#include "searchwindow.h"
#include "notesdialog.h"
#include "notebooksdialog.h"

// GNOTE INCLUDES
#include <libktomgirl/ktglib.hpp>
#include <libktomgirl/gnote.hpp>
#include <libktomgirl/note.hpp>
#include <libktomgirl/notebookmanager.hpp>
#include <libktomgirl/tagmanager.hpp>

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

	// BEGIN SEARCH BAR
	m_searchBar = new SearchBar();
  m_searchBar->setFeatures(QDockWidget::NoDockWidgetFeatures);
 	addDockWidget(Qt::TopDockWidgetArea, m_searchBar);
 	connect (m_searchBar->lineEditSearch, SIGNAL(returnPressed()), this, SLOT(slotHandleSearch()));
	//m_searchBar->resize(10, 50);
	// END SEARCH BAR


	// BEGIN NOTEBOOKS DIALOG
	m_notebooksDialog = new NotebooksDialog();
	//m_notebooksDialog->showMaximized();

  m_notebooksDialog->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea );
  m_notebooksDialog->setFeatures(QDockWidget::NoDockWidgetFeatures);

 	addDockWidget(Qt::BottomDockWidgetArea, m_notebooksDialog);
	//m_notebooksDialog->setMinimumSize();

  m_notebooksDialog->tableNotebooks->setCurrentItem(m_notebooksDialog->tableNotebooks->item(0,0));
  QTableWidgetItem *noteBooksHeader = m_notebooksDialog->tableNotebooks->horizontalHeaderItem(0);
	noteBooksHeader->setTextAlignment(Qt::AlignLeft);

  //m_notebooksDialog->tableNotebooks->verticalScrollBar()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  //m_notebooksDialog->tableNotebooks->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

 	connect (m_notebooksDialog->tableNotebooks, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(notebookDoubleClicked(int, int)));

	// END NOTEBOOKS DIALOG

	// BEGIN NOTES DIALOG
	m_notesDialog = new NotesDialog();
	m_notesDialog->showMaximized();

  m_notesDialog->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea );

  //m_notesDialog->setFeatures(QDockWidget::NoDockWidgetFeatures);
  addDockWidget(Qt::BottomDockWidgetArea, m_notesDialog);

	m_notesDialog->tableNotes->setRowCount(m_list.size()+1);
	m_notesDialog->tableNotes->setShowGrid(false);
	m_notesDialog->tableNotes->horizontalHeader()->setStretchLastSection(true);
	m_notesDialog->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QTableWidgetItem *noteHeader = m_notesDialog->tableNotes->horizontalHeaderItem(0);
	noteHeader->setTextAlignment(Qt::AlignLeft);

  //m_notebooksDialog->tableNotebooks->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  //m_notebooksDialog->tableNotebooks->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

  QTableWidgetItem *dateHeader = m_notesDialog->tableNotes->horizontalHeaderItem(1);
	dateHeader->setTextAlignment(Qt::AlignLeft);

 	connect (m_notesDialog->tableNotes, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(emitNoteSelected(QTableWidgetItem*)));

 	connect (m_notesDialog->tableNotes, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotItemClicked(QTableWidgetItem*)));
	// END NOTES DIALOG

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
		item->setIcon(notebookIcon);
		m_notesDialog->tableNotes->setItem ( m_row, 0, item );
		// END ITEM ONE

		// BEGIN ITEM TWO
  	sharp::DateTime qdt = note->data().change_date();
		qs = QString::fromStdString(qdt.to_string());
		item = new ktomgirl::KTGItem(qs, note);
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

void SearchWindow::newItem(gnote::Note::Ptr & note){
	QString name = QString::fromStdString(note->get_title());

  	KIcon notebookIcon = KIcon(":/icons/notebook.png");


	QDateTime qdt = QDateTime::currentDateTime();

	// BEGIN DEPRECATED
  	// sharp::DateTime dt = note->data().change_date();
	// END DEPRECATED

	// BEGIN FIRST ITEM
	ktomgirl::KTGItem *item = new ktomgirl::KTGItem(name, note);
	m_notesDialog->tableNotes->insertRow ( 0 );
	m_notesDialog->tableNotes->setItem ( 0 , 0, item );
	item->setIcon(notebookIcon);
	// END FIRST ITEM

	// BEGIN SECOND ITEM
	name = qdt.toString();
	item = new ktomgirl::KTGItem(name, note);
	m_notesDialog->tableNotes->setItem ( 0 , 1, item );
	// END SECOND ITEM

	m_row++;
	return;
}

void SearchWindow::deleteItem(const QString &qs){

	QList<QTableWidgetItem*> ql = 
 	m_notesDialog->tableNotes->findItems ( qs, Qt::MatchExactly);
	if (ql.count() < 1){
		qDebug() << __PRETTY_FUNCTION__<< "ERR: no note named" <<  qs << ql.count();
		return;
	}
	
	int row = m_notesDialog->tableNotes->row(ql[0]);
	m_notesDialog->tableNotes->removeRow (row); 
	styleNotes();
	return;
}

void SearchWindow::slotHandleSearch(){
	emit signalHandleSearch(m_searchBar->lineEditSearch->text() );
}

void SearchWindow::slotAddNotebook(const QString &nb){
  if (nb.trimmed().size() < 1) return;

	int rows = m_notebooksDialog->tableNotebooks->rowCount();

 	KIcon notebookIcon = KIcon(":/icons/notebook_edit.png");

	m_notebooksDialog->tableNotebooks->setRowCount(rows+1);
	
	QTableWidgetItem *item = new QTableWidgetItem ();
	item->setText(nb);
	item->setIcon(notebookIcon);
	
	m_notebooksDialog->tableNotebooks->setItem ( rows, 0, item );
}

void SearchWindow::loadNotebooks(){

  KTGlib::StringList nbs = gnote::notebooks::NotebookManager::instance().get_notebook_list();  
   for(KTGlib::StringList::const_iterator iter = nbs.begin();
           iter != nbs.end(); ++iter) {
           const std::string & nb (*iter);
		       slotAddNotebook(QString::fromStdString(nb));
    }

	return;
}

/* FRED: FIXME: Need to be able to search for the tag in the note...
 * Need to figure out how to get tagmanager, probably best to let note class in libktgirl
 * to do this...
 */
void SearchWindow::showFilteredNotes(const QString &filter){
  gnote::notebooks::Notebook::Ptr notebook = gnote::notebooks::NotebookManager::instance().get_notebook ( filter.toStdString() );

  int m_rows = m_notesDialog->tableNotes->rowCount();
	for(int i=0;  i < m_rows; i++){
	  ktomgirl::KTGItem	*item = static_cast<ktomgirl::KTGItem*> (m_notesDialog->tableNotes->item(i, 0));
		if (0 != item){
      gnote::Note::Ptr note = item->get_note();
      if (notebook->contains_note(note)){
        qDebug() << __PRETTY_FUNCTION__ << " not hidden: " <<QString::fromStdString(note->get_title());
        m_notesDialog->tableNotes->setRowHidden(i,false);
      }
      else {
        qDebug() << __PRETTY_FUNCTION__ << " hidden: " <<QString::fromStdString(note->get_title());
        m_notesDialog->tableNotes->setRowHidden(i,true);
       }
    }

	  //emit signalNoteSelected(static_cast<ktomgirl::KTGItem*>(item));
    // TODO: If we are valid, then we need to check to see if we are in the notebook
    // aka the filter. If so show it, otherwise, hide it...
	}
}

void SearchWindow::notebookDoubleClicked(int row, int col){
	QTableWidgetItem *item = m_notebooksDialog->tableNotebooks->item(row, col);
  showFilteredNotes(item->text());
  qDebug() << __PRETTY_FUNCTION__ << item->text();
}
// Sat Jan 19 14:26:33 PST 2013
