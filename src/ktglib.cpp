/*
 * ktomgirl
 *
 * Copyright (C) 2012 Fred Ollinger
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

#include "ktglib.hpp"
#include <QDir>
#include <QString>

namespace KTGlib {
ustring::ustring() : m_qs() {}
ustring::~ustring() {}
ustring::ustring(QString qs) : m_qs(qs) {}

const char* ustring::c_str(){
  QByteArray ba = m_qs.toLocal8Bit();
  return ba.data();
}

bool str_has_prefix(std::string st, const char *ch){
	size_t found;

	found = st.find_first_of(ch);
	if (0 == found)
		return true;

	return false;
}

QString get_user_data_dir(){ 
	return QDir::homePath() + "/.local/share";
}

} // namespace KTGlib
