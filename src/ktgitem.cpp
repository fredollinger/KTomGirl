
/*
 * ktomgirl
 * 
 * KTGItem is an item in the search window which can have more info about the
 * notes than a normal QTableWidgetItem.
 *
 * Copyright (C) 2012 Fred Ollinger
 * <follinge@gmail.com>
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

#include <QTableWidget>
#include "ktgitem.h"

#include "note.hpp"

namespace ktomgirl{
KTGItem::KTGItem(QString &qs, const gnote::Note::Ptr &note) : QTableWidgetItem(qs)
, m_note(note)
{
}

KTGItem::~KTGItem()
{
}

gnote::Note::Ptr KTGItem::get_note(){
	return m_note;
}

} // namespace ktomgirl
// Sat Apr  7 19:39:40 PDT 2012
