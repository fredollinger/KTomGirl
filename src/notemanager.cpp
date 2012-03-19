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
}
