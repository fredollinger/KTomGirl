#include <QApplication>
#include <QDebug>

#include <string>
#include <iostream>

#include <libktomgirl/datetime.hpp>
#include "../../src/version.h"

using namespace sharp;

void dateTest(std::string &st, std::string success){
	qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(st);
	// test the strip delim separately
	std::string res = DateTime::strip_delimiters_from_iso8601(st);
	qDebug() << __PRETTY_FUNCTION__ << QString::fromStdString(res);

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
}

int main( int argc, char *argv[] )
{
	QApplication app(argc, argv);

	std::string st;
	std::string success;

	success = "2002-02-01T07:59:59";

	#if 0
	// Test a valid string
	st = "20020131T235959";
	dateTest(st, success);

	st = "2002-0131T235959";
	dateTest(st, success);

	st = "this is really fucked up and should not pass";
	dateTest(st, success);
	#endif

	// Test a valid string
	success = "2012-02-01T07:59:59";
	st = "20120131T235959";
	dateTest(st, success);

	return app.exec();
}
// Sat May  5 10:26:49 PDT 2012
