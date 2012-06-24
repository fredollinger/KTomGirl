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

#include <QString>
#include <QStringList>

#include <QApplication>
#include <QDebug>

#include "../../libtomgirl/notemanager.hpp"
#include "../../libtomgirl/note.hpp"
#include "../../libtomgirl/gnote.hpp"
#include "../../libtomgirl/notedata.hpp"
#include "../../libtomgirl/datetime.hpp"
#include "../../libtomgirl/ktglib.hpp"

using namespace gnote;

namespace gnote{
	class NoteData;
}

void notesListTest(){
//	QStringList qsl = Gnote::get_note_list();
//	qDebug() << "Note List: " << qsl;
	KTGlib::StringList qsl = Gnote::get_note_list2();

	//QStringList Gnote::get_note_list(){
	#if 0
	std::string title = "New Note 4";
	gnote::NoteManager *nm = new gnote::NoteManager();
	gnote::Note::Ptr note = nm->find(title);
        QString m_content = QString::fromStdString(note->text_content());
	qDebug() << m_content;
        //QString uid = QString::fromStdString(note->uid());
	//qDebug() << QString::fromStdString(title) << m_content << uid;
	#endif
}

int main( int argc, char *argv[] ) {
	QApplication app(argc, argv);

	notesListTest();

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012