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
#include <QDebug>

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
#include "files.hpp"
#include "note.hpp"
#include "tag.hpp"
#include "string.hpp"
#include "datetime.hpp"
#include "notemanager.hpp"
#include "xmlconvert.hpp"
#include "xmlreader.hpp"
#include "xmlwriter.hpp"

namespace utils{
    std::string encode(const std::string & source)
    {
      sharp::XmlWriter xml;
      xml.write_string(source);

      xml.close();
      return xml.to_string();
    }


    std::string decode(const std::string & source)
    {
      // TODO there is probably better than a std::string for that.
      // this will do for now.
      std::string builder;

      sharp::XmlReader xml;
      xml.load_buffer(source);

      while (xml.read ()) {
        switch (xml.get_node_type()) {
        case XML_READER_TYPE_TEXT:
        case XML_READER_TYPE_WHITESPACE:
          builder += xml.get_value();
          break;
        default:
          break;
        }
      }

      xml.close ();

      return builder;
    }
}

namespace gnote {
class NoteData
{
/* This holds the actual data, text, and on on of the note */
  public:
    NoteData(const std::string & _uri);

    const std::string & full_path() const
      {
        return m_full_path;
      }

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

    void set_full_path(const std::string & path)
      {
        m_full_path = path;
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
    std::string       m_full_path; // full path to the note
    sharp::DateTime             m_create_date;
    sharp::DateTime             m_change_date;
    sharp::DateTime             m_metadata_change_date;
    int               m_cursor_pos;
    int               m_width, m_height;
    int               m_x, m_y;
    bool              m_open_on_startup;
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
    , m_is_open(false)
{
}

Note::~Note()
{
}

void Note::changed(){
    m_save_needed = true;
}

// BEGIN NOTE::SAVE()
void Note::save(std::string text)
{
	// Prevent any other condition forcing a save on the note
	// if Delete has been called.
	if (m_is_deleting)
		return;

	qDebug() << __PRETTY_FUNCTION__ << "SAVING" << QString::fromStdString(text);

//    NoteArchiver::write(m_filepath, text);

}
// END NOTE::SAVE()

  const std::string & Note::uri() const
  {
    return m_data.data().uri();
  }


  const std::string & Note::get_title() const
  {
    //return m_title;
    return m_data.getData().title();
  }

/*  Parse guid from file path.
 *  We assume that the filepath + guid + ".note" is the m_fullpath.
 *  If this changes then obviously we break this... 
 */
std::string Note::uid() const {
	size_t len = m_filepath.length();
	size_t begin = m_filepath.find_last_of("/");
	return m_filepath.substr(begin+1, len - 6 - begin);
}

  Note::Ptr Note::create_existing_note(NoteData *data,
                                 std::string filepath,
                                 NoteManager & manager)
  {
    if (!data->change_date().is_valid()) {
      sharp::DateTime d(boost::filesystem::last_write_time(filepath));
      data->set_change_date(d);
    }
    if (!data->create_date().is_valid()) {
      if(data->change_date().is_valid()) {
        data->create_date() = data->change_date();
      }
      else {
        sharp::DateTime d(boost::filesystem::last_write_time(filepath));
        data->create_date() = d;
      }
    }
    return Note::Ptr(new Note(data, filepath, manager));
  }

  /// <summary>
  /// Returns a Tomboy URL from the given path.
  /// </summary>
  /// <param name="filepath">
  /// A <see cref="System.String"/>
  /// </param>
  /// <returns>
  /// A <see cref="System.String"/>
  /// </returns>
  std::string Note::url_from_path(const std::string & filepath)
  {
    return "note://gnote/" + sharp::file_basename(filepath);
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

  // BEGIN PARSE_TAGS FOR LOOP
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
  } // BEGIN PARSE_TAGS FOR LOOP
}
// END PARSE_TAGS

std::string Note::text_content()
{

	m_text_content=utils::decode(xml_content());
	return m_text_content;
    /*
    if(m_buffer) {
      return m_buffer->get_slice(m_buffer->begin(), m_buffer->end());
    }
    */
    // return utils::decode(xml_content());
}

  void Note::set_text_content(const std::string & text)
  {
    m_text_content = text;
    return;
  }


/*
void Note::slotNoteChanged(const QString &qs){
	qDebug() << __PRETTY_FUNCTION__ << "FIXME: STUB";
}
*/

// BEGIN Note:set_title()
void Note::set_title(const std::string & new_title)
{
    //qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(new_title);
    m_data.data().title() = new_title;

    qDebug() << __PRETTY_FUNCTION__ << "title: "<< QString::fromStdString(m_data.data().title());
    //m_title = new_title;
    /*
    if (m_data.data().title() != new_title) {

      std::string old_title = m_data.data().title();
      m_data.data().title() = new_title;

      // FIXME: Mark note as needing to save
      //queue_save (CONTENT_CHANGED); // TODO: Right place for this?
    }
    */
}
// END Note:set_title()


// END NOTE


// BEGIN NOTE ARCHIVER
std::string NoteArchiver::write_string(const NoteData & note)
{
    qDebug() << __PRETTY_FUNCTION__;
    std::string str;
    sharp::XmlWriter xml;
    obj().write(xml, note);
    xml.close();
    str = xml.to_string();
    return str;
}


// BEGIN Note::write_file()
void Note::write(const std::string & _write_file, const NoteData & note)
{
    qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(_write_file);
    return;
    #if 0
    std::string tmp_file = _write_file + ".tmp";
    // TODO Xml doc settings
    sharp::XmlWriter xml(tmp_file); //, XmlEncoder::DocumentSettings);
    write(xml, note);
    xml.close ();

    try {
      if (boost::filesystem::exists(_write_file)) {
        std::string backup_path = _write_file + "~";
        if (boost::filesystem::exists(backup_path)) {
          boost::filesystem::remove(backup_path);
        }
      
        // Backup the to a ~ file, just in case
        boost::filesystem::rename(_write_file, backup_path);
      
        // Move the temp file to write_file
        boost::filesystem::rename(tmp_file, _write_file);

        // Delete the ~ file
        boost::filesystem::remove(backup_path);
      } 
      else {
        // Move the temp file to write_file
	qDebug() << "moving temp file to write file";
        boost::filesystem::rename(tmp_file, _write_file);
      }
    }
    catch(const std::exception & e)
    {
      ERR_OUT("filesystem error: '%s'", e.what());
	qDebug() << "save fail";
    }
#endif
}
// END Note::write_file()

  void NoteArchiver::write(const std::string & write_file, const NoteData & data)
  {
    obj().write_file(write_file, data);
  }

// STARTHERE
// BEGIN NoteArchiver::write_file()
void NoteArchiver::write_file(const std::string & _write_file, const NoteData & note)
{
    qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(_write_file);
    std::string tmp_file = _write_file + ".tmp";
    // TODO Xml doc settings
    sharp::XmlWriter xml(tmp_file); //, XmlEncoder::DocumentSettings);
    write(xml, note);
    xml.close ();

    try {
      if (boost::filesystem::exists(_write_file)) {
        std::string backup_path = _write_file + "~";
        if (boost::filesystem::exists(backup_path)) {
          boost::filesystem::remove(backup_path);
        }
      
        // Backup the to a ~ file, just in case
        boost::filesystem::rename(_write_file, backup_path);
      
        // Move the temp file to write_file
        boost::filesystem::rename(tmp_file, _write_file);

        // Delete the ~ file
        boost::filesystem::remove(backup_path);
      } 
      else {
        // Move the temp file to write_file
	qDebug() << "moving temp file to write file";
        boost::filesystem::rename(tmp_file, _write_file);
      }
    }
    catch(const std::exception & e)
    {
      //ERR_OUT("filesystem error: '%s'", e.what());
	qDebug() << "save fail";
    }
  }

  void NoteArchiver::write(sharp::XmlWriter & xml, const NoteData & note)
  {

    qDebug() << __PRETTY_FUNCTION__ << "title: "<< QString::fromStdString(note.title());
    xml.write_start_document();
    xml.write_start_element("", "note", "http://beatniksoftware.com/tomboy");
    xml.write_attribute_string("",
                             "version",
                             "",
                             CURRENT_VERSION);
    xml.write_attribute_string("xmlns",
                             "link",
                             "",
                             "http://beatniksoftware.com/tomboy/link");
    xml.write_attribute_string("xmlns",
                             "size",
                             "",
                             "http://beatniksoftware.com/tomboy/size");

    xml.write_start_element ("", "title", "");
    xml.write_string (note.title());
    xml.write_end_element ();

    xml.write_start_element ("", "text", "");
    xml.write_attribute_string ("xml", "space", "", "preserve");
    // Insert <note-content> blob...
    xml.write_raw (note.text());
    xml.write_end_element ();

    xml.write_start_element ("", "last-change-date", "");
    xml.write_string (
      sharp::XmlConvert::to_string (note.change_date()));
    xml.write_end_element ();

    xml.write_start_element ("", "last-metadata-change-date", "");
    xml.write_string (
      sharp::XmlConvert::to_string (note.metadata_change_date()));
    xml.write_end_element ();

    if (note.create_date().is_valid()) {
      xml.write_start_element ("", "create-date", "");
      xml.write_string (
        sharp::XmlConvert::to_string (note.create_date()));
      xml.write_end_element ();
    }

    xml.write_start_element ("", "cursor-position", "");
    xml.write_string (boost::lexical_cast<std::string>(note.cursor_position()));
    xml.write_end_element ();

    xml.write_start_element ("", "width", "");
    xml.write_string (boost::lexical_cast<std::string>(note.width()));
    xml.write_end_element ();

    xml.write_start_element("", "height", "");
    xml.write_string(boost::lexical_cast<std::string>(note.height()));
    xml.write_end_element();

    xml.write_start_element("", "x", "");
    xml.write_string (boost::lexical_cast<std::string>(note.x()));
    xml.write_end_element();

    xml.write_start_element ("", "y", "");
    xml.write_string (boost::lexical_cast<std::string>(note.y()));
    xml.write_end_element();

    /*
    if (note.tags().size() > 0) {
      xml.write_start_element ("", "tags", "");
      for(NoteData::TagMap::const_iterator iter = note.tags().begin();
          iter != note.tags().end(); ++iter) {
        xml.write_start_element("", "tag", "");
        xml.write_string(iter->second->name());
        xml.write_end_element();
      }
      xml.write_end_element();
    }
    */

    xml.write_start_element("", "open-on-startup", "");
    xml.write_string(note.is_open_on_startup() ? "True" : "False");
    xml.write_end_element();

    xml.write_end_element(); // Note
    xml.write_end_document();

}
// END NoteArchiver::write_file()
 
  const char *NoteArchiver::CURRENT_VERSION = "0.3";
//  const char *NoteArchiver::DATE_TIME_FORMAT = "%Y-%m-%dT%T.@7f@%z"; //"yyyy-MM-ddTHH:mm:ss.fffffffzzz";

  NoteData *NoteArchiver::read(const std::string & read_file, const std::string & uri)
  {
    return obj()._read(read_file, uri);
  }


// BEGIN NoteArchiver::_read()
NoteData *NoteArchiver::_read(const std::string & read_file, const std::string & uri)
{
    //qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(read_file);
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
// END NoteArchiver::_read()
// END NOTE ARCHIVER

// BEGIN NoteDataBufferSynchronizer
  NoteDataBufferSynchronizer::~NoteDataBufferSynchronizer()
  {
    delete m_data;
  }

  const std::string & NoteDataBufferSynchronizer::text()
  {
    // synchronize_text();
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

/*
  void NoteDataBufferSynchronizer::synchronize_text(std::string &text) const
  {
	m_data->text() = text;
  }
*/

void NoteDataBufferSynchronizer::synchronize_buffer()
{
}

  void NoteDataBufferSynchronizer::buffer_changed()
  {
    invalidate_text();
  }
// END NoteDataBufferSynchronizer

/*
const NoteData & synchronized_data() const
{
      synchronize_text();
      return *m_data;
}
*/

  
 void Note::save()
 {
//    try {
      NoteArchiver::write(m_filepath, m_data.synchronized_data());
 //   } 

  //  catch (const sharp::Exception & e) {
      // Probably IOException or UnauthorizedAccessException?
      //ERR_OUT("Exception while saving note: %s", e.what());
      //show_io_error_dialog(m_window);
   // }
}

  
} // namespace gnote
// Sat Apr 14 10:42:16 PDT 2012
