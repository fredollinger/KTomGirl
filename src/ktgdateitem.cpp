
/*
 * ktomgirl
 * 
 * KTGDateItem is an item in the search window which can have date info about the
 * notes. 
 *
 * Copyright (C) 2012-2014 Fred Ollinger
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

#include <QDebug>
#include "ktgdateitem.h"

namespace ktomgirl{
KTGDateItem::KTGDateItem(QString &qs, const gnote::Note::Ptr &note) 
: KTGItem(qs, note) 
{
  // qDebug() << __PRETTY_FUNCTION__;
}

KTGDateItem::~KTGDateItem()
{
}

bool KTGDateItem::operator<( const QTableWidgetItem &other ) const{
  qDebug() << __PRETTY_FUNCTION__;
  return QTableWidgetItem ::operator<(other);
}

} // namespace ktomgirl
// Sat Jan  3 09:55:38 PST 2015
