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


#include <string.h>

#include <exception>

#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "datetime.hpp"
#include "debug.hpp"
#include "gnote.hpp"
#include "note.hpp"
#include "notemanager.hpp"
#include "preferences.hpp"
#include "sharp.hpp"
#include "string.hpp"

namespace gnote {
NoteManager::NoteManager(const std::string & directory, const std::string & backup) : QWidget()
  {
    _common_init(directory, backup);
  }

void NoteManager::_common_init(const std::string & directory, const std::string & backup_directory)
  {
    m_addin_mgr = NULL;

    Preferences & prefs(Preferences::obj());
    // Watch the START_NOTE_URI setting and update it so that the
    // StartNoteUri property doesn't generate a call to
    // Preferences.Get () each time it's accessed.
    /*
    m_start_note_uri = prefs.get<std::string>(Preferences::START_NOTE_URI);
    prefs.signal_setting_changed().connect(
      sigc::mem_fun(*this, &NoteManager::on_setting_changed));
    */
    // m_note_template_title = tr("New Note Template");

    DBG_OUT("NoteManager created with note path \"%s\".", directory.c_str());

    m_notes_dir = directory;
    m_backup_dir = backup_directory;

#if 0
    bool is_first_run = first_run ();
    create_notes_dir ();

    const std::string old_note_dir = Gnote::old_note_dir();
    const bool migration_needed
                 = is_first_run
                   && sharp::directory_exists(old_note_dir);

    if (migration_needed) {
      migrate_notes(old_note_dir);
      is_first_run = false;
    }

    m_addin_mgr = create_addin_manager ();

    if (is_first_run) {
      std::list<ImportAddin*> l;
      m_addin_mgr->get_import_addins(l);
      bool has_imported = false;

      if(l.empty()) {
        DBG_OUT("no import plugins");
      }

      for(std::list<ImportAddin*>::iterator iter = l.begin();
          iter != l.end(); ++iter) {

        DBG_OUT("importing");
        (*iter)->initialize();
        if((*iter)->want_to_run(*this)) {
          has_imported |= (*iter)->first_run(*this);
        }
      }
      // we MUST call this after import
      post_load();

      // First run. Create "Start Here" notes.
      create_start_notes ();
    } 
    else {
      load_notes ();
    }
#endif

    // Gtk::Main::signal_quit().connect(sigc::mem_fun(*this, &NoteManager::on_exiting_event), 1);
  }

  NoteManager::~NoteManager()
  {
    delete m_addin_mgr;
  }

// Create a new note with the specified title, and a simple
// "Describe..." body or the body from the "New Note Template"
// note if it exists.  If the "New Note Template" body is found
// the text will not automatically be highlighted.
// BEGIN NoteManager::create_new_note()
Note::Ptr NoteManager::create_new_note (std::string title, const std::string & guid)
{
	Note::Ptr new_note; // FIXME: remove this stub
   	return new_note; 
}
#if 0
    Note::Ptr new_note = create_new_note (title, content, guid);
    std::string body;
    
    title = split_title_from_content (title, body);
    if (title.empty())
      return Note::Ptr();
      
    Note::Ptr note_template = find(m_note_template_title);
    if (note_template) {
      // Use the body from the "New Note Template" note
      std::string xml_content =
        sharp::string_replace_first(note_template->xml_content(), 
                                    m_note_template_title,
                                    utils::XmlEncoder::encode (title));
      return create_new_note (title, xml_content, guid);
    }
      
    // Use a simple "Describe..." body and highlight
    // it so it can be easily overwritten
    body = _("Describe your new note here.");
    
    std::string header = title + "\n\n";
    std::string content =
      boost::str(boost::format("<note-content>%1%%2%</note-content>") %
                 utils::XmlEncoder::encode (header) 
                 % utils::XmlEncoder::encode (body));
    
    Note::Ptr new_note = create_new_note (title, content, guid);
    
    // Select the inital
    // "Describe..." text so typing will overwrite the body text,
    //NoteBuffer 
    Glib::RefPtr<Gtk::TextBuffer> buffer = new_note->get_buffer();
    Gtk::TextIter iter = buffer->get_iter_at_offset(header.size());
    buffer->move_mark (buffer->get_selection_bound(), iter);
    buffer->move_mark (buffer->get_insert(), buffer->end());
    
    return new_note;
}
#endif
// END NoteManager::create_new_note()

} // namespace gnote
