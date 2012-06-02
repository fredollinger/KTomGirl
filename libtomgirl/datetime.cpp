/*
 * gnote
 *
 * Copyright (C) 2009 Hubert Figuiere
 *
 * 2012 Modified by Fred Ollinger <follinge@gmail.com> for KTomGirl
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



#include <time.h>
#include <sys/time.h>

#include <QDateTime>

#include "datetime.hpp"


namespace sharp {

#define SEC_PER_MINUTE 60
#define SEC_PER_HOUR (SEC_PER_MINUTE * 60)
#define SEC_PER_DAY (SEC_PER_HOUR * 24)

  DateTime::DateTime()
  {
    m_date.tv_sec = -1;
    m_date.tv_usec = -1;
  }

  DateTime::DateTime(time_t t, glong _usec)
  {
    m_date.tv_sec = t;
    m_date.tv_usec = _usec;
  }

  DateTime::DateTime(const GTimeVal & v)
    : m_date(v)
  {
  }

  DateTime & DateTime::add_days(int days)
  {
    m_date.tv_sec += (days * SEC_PER_DAY);
    return *this;
  }

  DateTime & DateTime::add_hours(int hours)
  {
    m_date.tv_sec += (hours * SEC_PER_HOUR);
    return *this;
  }

  int DateTime::day() const
  {
    struct tm result;
    localtime_r(&m_date.tv_sec, &result);
    return result.tm_mday;
  }

  int DateTime::month() const
  {
    struct tm result;
    localtime_r(&m_date.tv_sec, &result);
    return result.tm_mon + 1;
  }

  int DateTime::year() const
  {
    struct tm result;
    localtime_r(&m_date.tv_sec, &result);
    return result.tm_year + 1900;
  }

  int DateTime::day_of_year() const
  {
    struct tm result;
    localtime_r(&m_date.tv_sec, &result);
    return result.tm_yday;
  }

  bool DateTime::is_valid() const
  {
    return ((m_date.tv_sec != -1) && (m_date.tv_usec != -1));
  }

  std::string DateTime::_to_string(const char * format, struct tm * t) const
  {
    char output[256];
    strftime(output, sizeof(output), format, t);
    return QString(output).toStdString();
    // return Glib::locale_to_utf8(output);
    //return QString::toS(output);
  }

  QString DateTime::toString() const
  {
    char output[256];
    struct tm result; 
    strftime(output, sizeof(output), "%R", localtime_r(&m_date.tv_sec, &result));
    return QString(output);
  }

  std::string DateTime::to_string(const char * format) const
  {
    struct tm result; 
    return _to_string(format, localtime_r(&m_date.tv_sec, &result));
  }


  std::string DateTime::to_short_time_string() const
  {
    struct tm result;
    return _to_string("%R", localtime_r(&m_date.tv_sec, &result));
  }

  std::string DateTime::to_iso8601() const
  {
    std::string retval;
    if(!is_valid()) {
      return retval;
    }
    QTime t(0, m_date.tv_sec, m_date.tv_usec);
    retval = t.toString().toStdString();

    return retval;
  }

DateTime DateTime::now()
  {
	GTimeVal n;
	//g_get_current_time(&n); 
	struct timeval t;
	gettimeofday(&t, NULL);
	n.tv_sec = t.tv_sec;
	n.tv_usec = t.tv_usec;

	return DateTime(n);
}

DateTime DateTime::from_iso8601(const std::string &iso8601)
{
	GTimeVal retval;
	QDateTime qdt = QDateTime::fromString(QString::fromStdString(iso8601), Qt::ISODate);
	retval.tv_sec = qdt.toTime_t();
	retval.tv_usec = 0;
	return DateTime(retval);
}


  int DateTime::compare(const DateTime &a, const DateTime &b)
  {
    if(a > b)
      return +1;
    if(b > a)
      return -1;
    return 0;
  }

  bool DateTime::operator==(const DateTime & dt) const
  {
    return (m_date.tv_sec == dt.m_date.tv_sec) 
      && (m_date.tv_usec == dt.m_date.tv_usec);
  }

  bool DateTime::operator>(const DateTime & dt) const
  {
    if(m_date.tv_sec == dt.m_date.tv_sec) {
      return (m_date.tv_usec > dt.m_date.tv_usec);
    }
    return (m_date.tv_sec > dt.m_date.tv_sec);
  }


}
