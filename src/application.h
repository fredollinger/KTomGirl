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

#ifndef APPLICATION_H
#define APPLICATION_H

#include "knotesapp.h"
#include <kcmdlineargs.h>
#include <KParts/MainWindow>
#include <KApplication>

class KCmdLineArgs;

namespace knotes{
class Application : public KApplication
{
  public:
    Application();
    ~Application();

    virtual int newInstance();
    KCmdLineArgs *args() const;
  private:
    KNotesApp *mMainWindow;
    KCmdLineArgs *mCmdLineArguments;
};
} // namespace knotes
#endif
// Mon Mar 19 14:34:37 PDT 2012
