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

#include "resourcemanager.h"
#include <QDebug>
// #include "resourcelocal.h"

//#include <kcal/journal.h>

namespace ktomgirl{
	Journal::Journal(){
    sharp::uuid *uid = new sharp::uuid();
    m_uid = QString::fromStdString(uid->string());
    delete uid;
  }
	Journal::~Journal(){}
	void Journal::setUid(const QString &uid){
		m_uid = uid;
	}
} //namespace ktomgirl


KNotesResourceManager::KNotesResourceManager()
  : QObject( 0 )
{
  setObjectName( "KNotes Resource Manager" );
}

KNotesResourceManager::~KNotesResourceManager()
{
}

void KNotesResourceManager::load()
{

}

void KNotesResourceManager::save()
{
}

// when adding a new note, make sure a config file exists!!
void KNotesResourceManager::addNewNote( ktomgirl::Journal *journal )
{
  qDebug() << __PRETTY_FUNCTION__;
  // TODO: Make this configurable
  /*
  ResourceNotes *resource = m_manager->standardResource();
  if ( resource ) {
    resource->addNote( journal );
    registerNote( resource, journal );
  } else {
    kWarning( 5500 ) << "no resource!";
  }
*/
}

void KNotesResourceManager::registerNote( ResourceNotes *resource,
                                          ktomgirl::Journal *journal )
{
  // TODO: only emit the signal if the journal is new?
  // m_resourceMap.insert( journal->uid(), resource );
  //emit sigRegisteredNote( journal );
}


void KNotesResourceManager::deleteNote( ktomgirl::Journal *journal )
{
  const QString uid = journal->uid();
  
  // Remove the journal from the resource it came from
  // m_resourceMap.value( uid )->deleteNote( journal );
  // m_resourceMap.remove( uid );
  
  // libkcal does not delete the journal immediately, therefore it is ok to
  // emit the journal here
  //emit sigDeregisteredNote( journal );
}

/*
ktomgirl::Alarm::List KNotesResourceManager::alarms( const KDateTime &from,
                                                 const KDateTime &to )
{
  ktomgirl::Alarm::List result;
  return result;
}
*/

void KNotesResourceManager::resourceAdded( ResourceNotes *resource )
{
  #if 0
  kDebug( 5500 ) << "Resource added:" << resource->resourceName();
  
  if ( !resource->isActive() ) {
    return;
  }
  
  resource->setManager( this );
  if ( resource->open() ) {
    resource->load();
  }
  #endif
}

void KNotesResourceManager::resourceModified( ResourceNotes *resource )
{
}

void KNotesResourceManager::resourceDeleted( ResourceNotes *resource )
{
}


// Mon May  6 14:31:10 PDT 2013
