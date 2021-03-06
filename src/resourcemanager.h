/*******************************************************************
 This file is part of KNotes.

 Copyright (c) 2004, Bo Thorsen <bo@sonofthor.dk>
               2004, Michael Brade <brade@kde.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA  02110-1301, USA.

 In addition, as a special exception, the copyright holders give
 permission to link the code of this program with any edition of
 the Qt library by Trolltech AS, Norway (or with modified versions
 of Qt that use the same license as Qt), and distribute linked
 combinations including the two.  You must obey the GNU General
 Public License in all respects for all of the code used other than
 Qt.  If you modify this file, you may extend this exception to
 your version of the file, but you are not obligated to do so.  If
 you do not wish to do so, delete this exception statement from
 your version.
*******************************************************************/


#ifndef KNOTES_RESOURCEMANAGER_H
#define KNOTES_RESOURCEMANAGER_H

#include <QObject>
#include <QString>
#include "uuid.hpp"

class KNote;

namespace ktomgirl{
class Journal{
public:
	Journal(void);
	~Journal(void);
  QString uid(void) { return m_uid; }
	void setUid(const QString&);
private:
	QString m_uid;
}; // class Journal
} //namespace ktomgirl{

class ResourceNotes{
};

class KNotesResourceManager
  : public QObject //, public KRES::ManagerObserver<ResourceNotes>
{
  Q_OBJECT
  public:
    KNotesResourceManager();
    virtual ~KNotesResourceManager();
    
    void load();
    void save();
    
    void addNewNote( ktomgirl::Journal *journal );
    void registerNote( ResourceNotes *resource, ktomgirl::Journal *journal );
    
    void deleteNote( ktomgirl::Journal *journal );
    
    //KCal::Alarm::List alarms( const KDateTime &from, const KDateTime &to );
    
    // from the ManagerObserver interface
    virtual void resourceAdded( ResourceNotes *resource );
    virtual void resourceModified( ResourceNotes *resource );
    virtual void resourceDeleted( ResourceNotes *resource );
    
  signals:
    //void sigRegisteredNote( KCal::Journal *journal );
    //void sigDeregisteredNote( KCal::Journal *journal );
    
  private:
    ResourceNotes *m_manager;
    // KRES::Manager<ResourceNotes> *m_manager;
    // QMultiHash<QString,ResourceNotes*> m_resourceMap;
};

#endif // RESOURCEMANAGER_H
// Mon May  6 14:31:17 PDT 2013
