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

void dateTest(std::string &st, std::string success){
	qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(st);
	// test the strip delim separately
	std::string res = DateTime::strip_delimiters_from_iso8601(st);
	qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(res);

	// now test the converter
	DateTime dt = DateTime::from_iso8601(st);
	res = dt.to_iso8601();
	qDebug() << QString::fromStdString(res);

	if (success.compare(st)) 
		qDebug() << "success";
	else{
	  qDebug() << "date test failed: " <<  QString::fromStdString(res) << " != " << QString::fromStdString(success);
	  exit(0); 
	}
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	std::string st;
	std::string success;

	success = "2002-02-01T07:59:59";

	#if 0
	// Test a valid string
	st = "20020131T235959";
	dateTest(st, success);

	st = "2002-0131T235959";
	dateTest(st, success);

	st = "this is really fucked up and should not pass";
	dateTest(st, success);
	#endif

	// Test a valid string
	success = "2012-02-01T07:59:59";
	st = "20120131T235959";
	dateTest(st, success);

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
