#include "ktgconfig.h"

namespace ktomgirl{

KTGConfig::KTGConfig() : KTGConfigSkel()
{
	readConfig();
}

KTGConfig::~KTGConfig()
{
}

int KTGConfig::noteNumber(){ 
	int n;

	if (mNoteNumber < 1) mNoteNumber = 1;

	n = mNoteNumber;
	mNoteNumber++;
	return n;
}

int KTGConfig::addNotebook(const QString &nb){ 
  	mNoteBooks << nb;
	qDebug() << mNoteBooks; 
}

} // namespace ktomgirl
// Sun May 13 10:34:29 PDT 2012
