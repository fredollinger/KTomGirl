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
#ifndef __KTG_DATE_ITEM_HPP__
#define __KTG_DATE_ITEM_HPP__

#include "ktgitem.h"

namespace ktomgirl {
class KTGDateItem : public KTGItem {
public:
  KTGDateItem(QString&, const gnote::Note::Ptr&);
  ~KTGDateItem(void); 
  bool operator<( const QTableWidgetItem& ) const;
}; // END KTGDATEITEM
} // namespace ktomgirl 
#endif  //_KTG_DATE_ITEM_HPP__
// Sat Jan  3 10:19:39 PST 2015
