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

//#include "../src/notemanager.hpp"
//#include "../src/notedata.hpp"
//#include "../src/notebuffer.hpp"
#include "../src/version.h"
//#include "searchwindow.h"
#include "knote.h"

#include <QApplication>
#include <QDebug>

void showXmlWindow(){
	KNote *sw = new KNote();
	sw->show();
}

void showSearchWindow(){
	/*
	SearchWindow *sw = new SearchWindow();
	sw->show();
	*/
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	showXmlWindow();

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
