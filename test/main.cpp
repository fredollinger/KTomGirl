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

#include "../src/notemanager.hpp"
#include "../src/version.h"


#include <QApplication>
#include <QDebug>

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	gnote::NoteManager *nm = new gnote::NoteManager();
	gnote::Note::List notes = nm->get_notes();

	#if 0
	for(gnote::Note::List::const_iterator iter = notes.begin();
		iter != notes.end(); ++iter) {
		const gnote::Note::Ptr & note(*iter);
		qDebug() << QString::fromStdString(note->get_title());
		qDebug() << QString::fromStdString(note->file_path());
	}
	#endif
	notes[0]->set_title("new_title");
	qDebug() << QString::fromStdString(notes[0]->get_title());
	qDebug() << QString::fromStdString(notes[0]->file_path());
	notes[0]->save();
	// open a specific note and get output into qstring
	// save note into new qstring
	// compare the two, they should be the same
	// quit somehow

	return app.exec();
}
