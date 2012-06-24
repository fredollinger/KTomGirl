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

#include <list>
#include <iostream>
#include <string>

using namespace gnote;

namespace gnote{
	class NoteData;
}

void notesListTest(){
//	QStringList qsl = Gnote::get_note_list();
//	qDebug() << "Note List: " << qsl;
	KTGlib::StringList qsl = Gnote::get_note_list2();

	std::cout << "test";

	for (std::list<std::string>::const_iterator itr = qsl.begin(); itr != qsl.end(); ++itr){
		std::cout << *itr << "\n";
	}

}

int main( int argc, char *argv[] ) {
	notesListTest();
//	QApplication app(argc, argv);

//	return app.exec();
	return 0;
}
// Sat May  5 10:26:49 PDT 2012
