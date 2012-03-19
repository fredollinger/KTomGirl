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


//#if HAVE_CONFIG_H
#include "config.h"
// #endif

#include <map>

#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/find.hpp>

#include <libxml/parser.h>

#if 0
#include <glibmm/i18n.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#endif

#include "note.hpp"
#include "tag.hpp"
#include "sharp/string.hpp"
//#include "notemanager.hpp"
#if 0
#include "notetag.hpp"
#include "notewindow.hpp"
#include "tagmanager.hpp"
#include "utils.hpp"
#include "debug.hpp"
#include "sharp/exception.hpp"
#include "sharp/files.hpp"
#include "sharp/map.hpp"
#include "sharp/xml.hpp"
#include "sharp/xmlconvert.hpp"
#include "sharp/xmlreader.hpp"
#include "sharp/xmlwriter.hpp"
#endif

namespace gnote {

  namespace noteutils {

#if 0
    void show_deletion_dialog (const std::list<Note::Ptr> & notes, Gtk::Window * parent)
    {
      std::string message;

      // %1% is the number of note.
      message = ngettext("Really delete this note?", "Really delete these %1% notes?", notes.size());

      boost::format format(message);
      // make sure an exception is not raised for the non-plural form
      format.exceptions( boost::io::all_error_bits ^ 
                         ( boost::io::too_many_args_bit | 
                           boost::io::too_few_args_bit )  );

      utils::HIGMessageDialog dialog(parent, GTK_DIALOG_DESTROY_WITH_PARENT,
                                     Gtk::MESSAGE_QUESTION,
                                     Gtk::BUTTONS_NONE,
                                     str(format % notes.size()),
                                     _("If you delete a note it is permanently lost."));

      Gtk::Button *button;

      button = manage(new Gtk::Button(Gtk::Stock::CANCEL));
      button->property_can_default().set_value(true);
      button->show ();
      dialog.add_action_widget(*button, Gtk::RESPONSE_CANCEL);
      dialog.set_default_response(Gtk::RESPONSE_CANCEL);

      button = manage(new Gtk::Button (Gtk::Stock::DELETE));
      button->property_can_default().set_value(true);
      button->show ();
      dialog.add_action_widget(*button, 666);

      int result = dialog.run();
      if (result == 666) {
        for(Note::List::const_iterator iter = notes.begin();
            iter != notes.end(); ++iter) {
          const Note::Ptr & note(*iter);
          note->manager().delete_note(note);
        }
      }
    }

  }
#endif

  namespace {
    
#if 0
    void show_io_error_dialog (Gtk::Window * parent)
    {
      utils::HIGMessageDialog dialog(
                              parent,
                              GTK_DIALOG_DESTROY_WITH_PARENT,
                              Gtk::MESSAGE_ERROR,
                              Gtk::BUTTONS_OK,
                              _("Error saving note data."),
                              _("An error occurred while saving your notes. "
                                "Please check that you have sufficient disk "
                                "space, and that you have appropriate rights "
                                "on ~/.gnote. Error details can be found in "
                                "~/.gnote.log."));
      dialog.run();
    }

  }
#endif

  class NoteData
  {
  public:
    typedef std::map<std::string, Tag::Ptr> TagMap;
    NoteData(const std::string & _uri);

    const std::string & uri() const
      {
        return m_uri;
      }
    const std::string & title() const
      {
        return m_title;
      }
    std::string & title()
      {
        return m_title;
      }
    const std::string & text() const
      { 
        return m_text;
      }
    std::string & text()
      { 
        return m_text;
      }
    const sharp::DateTime & create_date() const
      {
        return m_create_date;
      }
    sharp::DateTime & create_date()
      {
        return m_create_date;
      }
    const sharp::DateTime & change_date() const
      {
        return m_change_date;
      }
    void set_change_date(const sharp::DateTime & date)
      {
        m_change_date = date;
        m_metadata_change_date = date;
      }
    const sharp::DateTime & metadata_change_date() const
      {
        return m_metadata_change_date;
      }
    sharp::DateTime & metadata_change_date()
      {
        return m_metadata_change_date;
      }
    int cursor_position() const
      {
        return m_cursor_pos;
      }
    void set_cursor_position(int new_pos)
      {
        m_cursor_pos = new_pos;
      }
    int width() const
      {
        return m_width;
      }
    int & width()
      {
        return m_width;
      }
    int height() const
      {
        return m_height;
      }
    int & height()
      {
        return m_height;
      }
    int x() const
      {
        return m_x;
      }
    int & x()
      {
        return m_x;
      }
    int y() const
      {
        return m_y;
      }
    int & y()
      {
        return m_y;
      }
    const TagMap & tags() const
      {
        return m_tags;
      }
    TagMap & tags()
      {
        return m_tags;
      }
    
    bool is_open_on_startup() const
      {
        return m_open_on_startup;
      }
    void set_is_open_on_startup(bool v)
      {
        m_open_on_startup = v;
      }
    void set_position_extent(int x, int y, int width, int height);
    bool has_position();
    bool has_extent();

  private:
    const std::string m_uri;
    std::string       m_title;
    std::string       m_text;
    sharp::DateTime             m_create_date;
    sharp::DateTime             m_change_date;
    sharp::DateTime             m_metadata_change_date;
    int               m_cursor_pos;
    int               m_width, m_height;
    int               m_x, m_y;
    bool              m_open_on_startup;

    TagMap m_tags;
  
    static const int  s_noPosition;
  };


  const int  NoteData::s_noPosition = -1;

  NoteData::NoteData(const std::string & _uri)
    : m_uri(_uri)
    , m_cursor_pos(0)
    , m_width(0)
    , m_height(0)
    , m_x(s_noPosition)
    , m_y(s_noPosition)
    , m_open_on_startup(false)
  {
  }


  void NoteData::set_position_extent(int _x, int _y, int _width, int _height)
  {
    if (_x < 0 || _y < 0)
      return;
    if (_width <= 0 || _height <= 0)
      return;

    m_x = _x;
    m_y = _y;
    m_width = _width;
    m_height = _height;
  }

  bool NoteData::has_position()
  {
    return (m_x != s_noPosition) && (m_y != s_noPosition);
  }

  bool NoteData::has_extent()
  {
    return (m_width != 0) && (m_height != 0);
  }

  NoteDataBufferSynchronizer::~NoteDataBufferSynchronizer()
  {
    delete m_data;
  }

  void NoteDataBufferSynchronizer::set_buffer(const Glib::RefPtr<NoteBuffer> & b)
  {
    m_buffer = b;
    m_buffer->signal_changed().connect(sigc::mem_fun(*this, &NoteDataBufferSynchronizer::buffer_changed));
    m_buffer->signal_apply_tag()
      .connect(sigc::mem_fun(*this, &NoteDataBufferSynchronizer::buffer_tag_applied));
    m_buffer->signal_remove_tag()
      .connect(sigc::mem_fun(*this, &NoteDataBufferSynchronizer::buffer_tag_removed));

    synchronize_buffer();

    invalidate_text();
  }

  const std::string & NoteDataBufferSynchronizer::text()
  {
    synchronize_text();
    return m_data->text();
  }

  void NoteDataBufferSynchronizer::set_text(const std::string & t)
  {
    m_data->text() = t;
    synchronize_buffer();
  }

  void NoteDataBufferSynchronizer::invalidate_text()
  {
    m_data->text() = "";
  }

  bool NoteDataBufferSynchronizer::is_text_invalid() const
  {
    return m_data->text().empty();
  }

  void NoteDataBufferSynchronizer::synchronize_text() const
  {
    if(is_text_invalid() && m_buffer) {
      m_data->text() = NoteBufferArchiver::serialize(m_buffer);
    }
  }

}
