/*
 * ktomgirl
 * 
 * KTGItem is an item in the search window which can have more info about the
 * notes than a normal QTableWidgetItem.
 *
 * Copyright (C) 2012-2015 Fred Ollinger <follinge@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>
#include <QTableWidget>
#include "ktgitem.h"

#include <libktomgirl/note.hpp>

namespace ktomgirl{
KTGItem::KTGItem(QString &qs, const gnote::Note::Ptr &note) 
: QTableWidgetItem(qs)
, m_note(note)
, m_uid(note->uid())
{
	//qDebug() << __PRETTY_FUNCTION__ << " note addy: [" << &note << "] path: [" << QString::fromStdString(m_note->file_path()) << "]";
				//qDebug() << __PRETTY_FUNCTION__ << " uid: [" << QString::fromStdString(m_uid) << "]";
				//qDebug() << __PRETTY_FUNCTION__ << " note text: [" << QString::fromStdString(m_note->get_title()) << "]";
}

KTGItem::~KTGItem()
{
}

const gnote::Note::Ptr KTGItem::get_note(){
  return m_note;
}

void KTGItem::setName(const QString &qs){
  setText(qs);
}

///home/follinge/projects/deb/ktomgirl-deb/KTomGirl/src/searchwindow.cpp :394: error: no matching function for call to 'ktomgirl::KTGItem::KTGItem(QString&, const gnote::Note&)

} // namespace ktomgirl
// Sat Apr  7 19:39:40 PDT 2012
