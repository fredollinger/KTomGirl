#ifndef __KTOMGIRL_KTGCONFIG__
#define __KTOMGIRL_KTGCONFIG__

#include <kglobal.h>
#include <kconfigskeleton.h>
#include <kdebug.h>

#include <kglobalsettings.h>

#include "ktgconfigskel.h"

namespace ktomgirl {
class KTGConfig 
     : public KTGConfigSkel {
public:
   static KTGConfig & obj(){ 
        static KTGConfig * instance = new KTGConfig();
        return *instance;
    }

	~KTGConfig(void);
	int addNotebook(const QString &nb);
	int noteNumber(void);
	void setNoteNumber(int n){ mNoteNumber = n; }
	void store(void){ writeConfig(); }
private:
	KTGConfig(void);
	
}; // class KTGConfig
}  // namespace ktomgirl
#endif // __KTOMGIRL_KTGCONFIG__
// Sun May 13 10:31:08 PDT 2012
