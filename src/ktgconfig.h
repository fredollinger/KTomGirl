#ifndef __KTOMGIRL_KTGCONFIG__
#define __KTOMGIRL_KTGCONFIG__

#include <kglobal.h>
#include <kconfigskeleton.h>
#include <kdebug.h>

#include <kglobalsettings.h>

#include "ktgconfigskel.h"

namespace ktomgirl {
class KTGConfig: public KTGConfigSkel
{
  public:
    KTGConfig();
    ~KTGConfig();
}; // class KTGConfig
}  // namespace ktomgirl
#endif // __KTOMGIRL_KTGCONFIG__
// Sun May 13 10:31:08 PDT 2012
