/*
 * ktomgirl
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
#ifndef __KTG_MENU_HPP__
#define __KTG_MENU_HPP__

#include <QTableWidget>
#include <QObject>
#include <libktomgirl/note.hpp>

namespace ktomgirl {
class KTGMenu : public KMenu {
public:
  KTGMenu(void);
  ~KTGMenu(void); 
};
} // namespace ktomgirl 
//Q_DECLARE_METATYPE( ktomgirl::KTGMenu );
#endif // __KTG_MENU_HPP__
// Sat Apr  7 19:43:45 PDT 2012
