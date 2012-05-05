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
#include "../src/notedata.hpp"
#include "../src/version.h"


#include <QApplication>
#include <QDebug>

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);
	gnote::NoteManager *nm = new gnote::NoteManager();
	gnote::Note::List notes = nm->get_notes();

	QString qs = "new_title\nsome shit";

	notes[0]->set_title("new_title");
	notes[0]->set_text_content(qs.toStdString() );

	qDebug() << QString::fromStdString(notes[0]->get_title());
	qDebug() << QString::fromStdString(notes[0]->text_content_plain());
	//qDebug() << QString::fromStdString(notes[0]->get_text());

	return app.exec();
}
