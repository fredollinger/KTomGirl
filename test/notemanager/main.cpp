/*******************************************************************
 KNotes -- Notes for the KDE project

 Copyright (c) 1997-2007, The KNotes Developers

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*******************************************************************/

#include "../../src/version.h"

#include <QApplication>
#include <QDebug>

#include "../../src/notemanager.hpp"
#include "../../src/note.hpp"

using namespace gnote;

namespace gnote{
	class NoteData;
}

void deleteNoteTest(){
	std::string title = "New Note 1";
	gnote::NoteManager *nm = new gnote::NoteManager();
	gnote::Note::Ptr note = nm->find(title);
        QString m_content = QString::fromStdString(note->text_content());
        QString uid = QString::fromStdString(note->uid());
	qDebug() << QString::fromStdString(title) << m_content << uid;
}

void dateTest(){
	std::string title = "New Note 1";
	gnote::NoteManager *nm = new gnote::NoteManager();
	gnote::Note::Ptr note = nm->find(title);
  	//QDateTime qdt = note->data().change_date();
  	note->data().change_date();
	//qDebug() << qdt.toString();
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	dateTest();

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
