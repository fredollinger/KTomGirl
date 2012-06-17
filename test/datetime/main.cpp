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

#include <string>
#include <iostream>

#include "../../libtomgirl/datetime.hpp"

using namespace sharp;

void dateTest(){
	//qDebug() << QString::fromStdString(qdt.to_iso8601());
	std::string st = "2012-06-12T10:24:00";
	std::string res = DateTime::strip_delimiters_from_iso8601(st);
	std::cout << res;
	
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	dateTest();

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
