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

#include <QSharedPointer>

#include "config.h"

#include <tr1/functional>

#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/find.hpp>

#include <libxml/parser.h>

#include <map>

#include "debug.hpp"
#include "note.hpp"
#include "tag.hpp"
#include "string.hpp"
#include "datetime.hpp"
#include "notemanager.hpp"
#include "xmlconvert.hpp"
#include "xmlreader.hpp"

namespace gnote {

  class NoteData
  {
  public:
    // typedef std::map<std::string, Tag::Ptr> TagMap;
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
/*
    const TagMap & tags() const
      {
        return m_tags;
      }
*/
/*
    TagMap & tags()
      {
        return m_tags;
      }
*/
    
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

    // TagMap m_tags;
  
    static const int  s_noPosition;
  }; // class NoteData

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
// END NOTEDATA

// BEGIN NOTE
  Note::Note(NoteData * _data, const std::string & filepath, NoteManager & _manager)
    : m_data(_data)
    , m_filepath(filepath)
    , m_save_needed(false)
    , m_is_deleting(false)
    , m_manager(_manager)
  {
    /*
    for(NoteData::TagMap::const_iterator iter = _data->tags().begin();
        iter != _data->tags().end(); ++iter) {
      add_tag(iter->second);
    }
    m_save_timeout = new utils::InterruptableTimeout();
    m_save_timeout->signal_timeout.connect(sigc::mem_fun(*this, &Note::on_save_timeout));
   */
  }

  Note::~Note()
  {
  }

  Note::Ptr Note::load(const std::string & read_file, NoteManager & manager)
  {
    NoteData *data = NoteArchiver::read(read_file, url_from_path(read_file));
    return create_existing_note (data, read_file, manager);
  }

// BEGIN PARSE_TAGS
void Note::parse_tags(const xmlNodePtr tagnodes, std::list<std::string> & tags)
{
  sharp::XmlNodeSet nodes = sharp::xml_node_xpath_find(tagnodes, "//*");
    
  if(nodes.empty()) {
    return;
  }

  // BEGIN FOR
  for(sharp::XmlNodeSet::const_iterator iter = nodes.begin();
      iter != nodes.end(); ++iter) {

        const xmlNodePtr node = *iter;
     if(xmlStrEqual(node->name, (const xmlChar*)"tag") && (node->type == XML_ELEMENT_NODE)) 
     {
        xmlChar * content = xmlNodeGetContent(node);
        if(content) 
        {
          DBG_OUT("found tag %s", content);
          tags.push_back((const char*)content);
          xmlFree(content);
        }
    }
  } // END FOR
}
// END PARSE_TAGS

// END NOTE

// BEGIN NOTE ARCHIVER
  const char *NoteArchiver::CURRENT_VERSION = "0.3";
//  const char *NoteArchiver::DATE_TIME_FORMAT = "%Y-%m-%dT%T.@7f@%z"; //"yyyy-MM-ddTHH:mm:ss.fffffffzzz";

  NoteData *NoteArchiver::read(const std::string & read_file, const std::string & uri)
  {
    return obj()._read(read_file, uri);
  }


  NoteData *NoteArchiver::_read(const std::string & read_file, const std::string & uri)
  {
    NoteData *note = new NoteData(uri);
    std::string version;

    sharp::XmlReader xml(read_file);

    std::string name;

    while (xml.read ()) {
      switch (xml.get_node_type()) {
      case XML_READER_TYPE_ELEMENT:
        name = xml.get_name();
        
        if(name == "note") {
          version = xml.get_attribute("version");
        }
        else if(name == "title") {
          note->title() = xml.read_string();
        } 
        else if(name == "text") {
          // <text> is just a wrapper around <note-content>
          // NOTE: Use .text here to avoid triggering a save.
          note->text() = xml.read_inner_xml();
        }
        else if(name == "last-change-date") {
          note->set_change_date(
            sharp::XmlConvert::to_date_time (xml.read_string()));
        }
        else if(name == "last-metadata-change-date") {
          note->metadata_change_date() =
            sharp::XmlConvert::to_date_time(xml.read_string());
        }
        else if(name == "create-date") {
          note->create_date() =
            sharp::XmlConvert::to_date_time (xml.read_string());
        }
        else if(name == "cursor-position") {
          note->set_cursor_position(boost::lexical_cast<int>(xml.read_string()));
        }
        else if(name == "width") {
          note->width() = boost::lexical_cast<int>(xml.read_string());
        }
        else if(name == "height") {
          note->height() = boost::lexical_cast<int>(xml.read_string());
        }
        else if(name == "x") {
          note->x() = boost::lexical_cast<int>(xml.read_string());
        }
        else if(name == "y") {
          note->y() = boost::lexical_cast<int>(xml.read_string());
        }
        else if(name == "tags") {
          xmlDocPtr doc2 = xmlParseDoc((const xmlChar*)xml.read_outer_xml().c_str());

          if(doc2) {
            std::list<std::string> tag_strings;
            Note::parse_tags(doc2->children, tag_strings);
            for(std::list<std::string>::const_iterator iter = tag_strings.begin();
                iter != tag_strings.end(); ++iter) {
              // Tag::Ptr tag = TagManager::obj().get_or_create_tag(*iter);
              // note->tags()[tag->normalized_name()] = tag;
            }
            xmlFreeDoc(doc2);
          }
          else {
            DBG_OUT("loading tag subtree failed");
          }
        }
        else if(name == "open-on-startup") {
          note->set_is_open_on_startup(xml.read_string() == "True");
        }
        break;

      default:
        break;
      }
    }
    xml.close ();

    if (version != NoteArchiver::CURRENT_VERSION) {
      // Note has old format, so rewrite it.  No need
      // to reread, since we are not adding anything.
      DBG_OUT("Updating note XML from %s to newest format...", version.c_str());
      NoteArchiver::write (read_file, *note);
    }
    return note;
  }
// END NOTE ARCHIVER
  
} // namespace gnote
// Sat Mar 31 14:06:31 PDT 2012
