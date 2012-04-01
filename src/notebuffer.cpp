/*
 * gnote
 *
 * Copyright (C) 2009 Hubert Figuiere
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

#include <algorithm>
#include <tr1/array>


#include "debug.hpp"
#include "notebuffer.hpp"
#include "note.hpp"
#include "preferences.hpp"

#include "xmlreader.hpp"
#include "xmlwriter.hpp"

namespace gnote {

#define NUM_INDENT_BULLETS 3

  NoteBuffer::NoteBuffer(Note & note) :
     m_note(note)
  {
  }

  NoteBuffer::~NoteBuffer()
  {
  }
} // namespace gnote
