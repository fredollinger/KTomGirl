#include <QApplication>
#include <QDebug>

#include <string>
#include <iostream>

#include <libktomgirl/datetime.hpp>
#include <libktomgirl/ktglib.hpp>
#include "../../src/version.h"

using namespace sharp;

void dateTest(std::string &st, std::string success){
	qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(success);
	// test the strip delim separately
	std::string res = DateTime::strip_delimiters_from_iso8601(success);
	qDebug() << __PRETTY_FUNCTION__ << "strip delim test: "<< QString::fromStdString(res);

	#if 0
	// now test the converter
	DateTime dt = DateTime::from_iso8601(st);
	res = dt.to_iso8601();
	qDebug() << QString::fromStdString(res);

	if (success.compare(st)) 
		qDebug() << "success";
	else{
	  qDebug() << "date test failed: " <<  QString::fromStdString(res) << " != " << QString::fromStdString(success);
	  exit(0); 
	}
	#endif
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	std::string st;
	std::string success;

	st = "2002-02-01T07:59:59";

	success = "20020131T235959";
	#if 0
	// Test a valid string
	dateTest(st, success);

	st = "2002-0131T235959";
	dateTest(st, success);

	st = "this is really fucked up and should not pass";
	dateTest(st, success);
	#endif

	// Test a valid string
	#if 0
	std::string s_char = "-";
	std::string str = KTGlib::erase(success, s_char);
	qDebug() << "stripped string: " << QString::fromStdString(str);
	s_char = ":";
	str = KTGlib::erase(str, s_char);
	qDebug() << "stripped string: " << QString::fromStdString(str);
	qDebug() << "orig string: " << QString::fromStdString(success);
	#endif

	dateTest(success, st);

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
