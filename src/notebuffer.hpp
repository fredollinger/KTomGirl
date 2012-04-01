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


#ifndef __NOTE_BUFFER_HPP_
#define __NOTE_BUFFER_HPP_

#include <queue>

#include <QString>
#include <QSharedPointer>
#include <QWidget>

namespace sharp {
  class XmlReader;
  class XmlWriter;
}

namespace gnote {

  class Note;

class NoteBuffer 
  : public QWidget
{
	Q_OBJECT;
public:
  typedef QSharedPointer<NoteBuffer> Ptr;

  bool get_enable_auto_bulleted_lists() const;

  ~NoteBuffer();

  void toggle_active_tag(const std::string &);
  void set_active_tag(const std::string &);
  void remove_active_tag(const std::string &);
  bool is_active_tag(const std::string & );
  bool is_bulleted_list_active();
  bool can_make_bulleted_list();
  bool add_tab();
  bool remove_tab();
  bool add_new_line(bool soft_break);
  bool delete_key_handler();
  bool backspace_key_handler();
  void check_selection();
  bool run_widget_queue();
  std::string get_selection() const;
  void toggle_selection_bullets();
  void increase_cursor_depth()
    {
      change_cursor_depth(true);
    }
  void decrease_cursor_depth()
    {
      change_cursor_depth(false);
    }
  void change_cursor_depth_directional(bool right);

protected: 
  NoteBuffer(Note &);

private:
  void change_cursor_depth(bool increase);

  static const QChar s_indent_bullets[];

  struct WidgetInsertData
  {
    bool adding;
    QSharedPointer<QString> buffer;
    QSharedPointer<QWidget>   position;
    QWidget                  *widget;
  };
  std::queue<WidgetInsertData> m_widget_queue;

  // The note that owns this buffer
  Note &                       m_note;
};

class NoteBufferArchiver
{
};

} // namespace gnote

#endif

