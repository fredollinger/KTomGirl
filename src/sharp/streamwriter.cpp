/*
 * gnote
 *
 * Copyright (C) 2009 Hubert Figuiere
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */





#include "sharp/streamwriter.hpp"

#include "debug.hpp"

namespace sharp {


  bool StreamWriterLink_ = true;

StreamWriter::StreamWriter()
  : m_file(NULL)
{
}

StreamWriter::~StreamWriter()
{
  if(m_file) {
    fclose(m_file);
  }
}

void StreamWriter::init(const std::string & filename)
{
  m_file = fopen(filename.c_str(), "wb");
}


void StreamWriter::write(const std::string & text)
{
  fprintf(m_file, "%s", text.c_str());
}

  void StreamWriter::write(const xmlBufferPtr buffer)
{
  xmlBufferDump(m_file, buffer);
}


void StreamWriter::close()
{
  fclose(m_file);
  m_file = NULL;
}


}
