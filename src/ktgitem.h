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
#ifndef __KTG_ITEM_HPP__
#define __KTG_ITEM_HPP__

#include <QTableWidget>
#include <QObject>
#include "note.hpp"

namespace ktomgirl {
class KTGItem : public QTableWidgetItem {
public:
  KTGItem(QString&, const gnote::Note::Ptr&);
  ~KTGItem(void); 
  gnote::Note::Ptr get_note(void);
private:
  const gnote::Note::Ptr &m_note;
};
} // namespace ktomgirl 
#endif // __KTG_ITEM_HPP__
// Sat Apr  7 19:43:45 PDT 2012
