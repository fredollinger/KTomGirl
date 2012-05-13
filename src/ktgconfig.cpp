#include "ktgconfig.h"

namespace ktomgirl{

KTGConfig::KTGConfig() : KTGConfigSkel()
{
	readConfig();
}

KTGConfig::~KTGConfig()
{
}

int
KTGConfig::noteNumber(){ 
	int n;

	if (mNoteNumber < 1) mNoteNumber = 1;

	n = mNoteNumber;
	mNoteNumber++;
	return n;
}

} // namespace ktomgirl
// Sun May 13 10:34:29 PDT 2012
