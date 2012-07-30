#ifndef _KTG_STUB_
#define _KTG_STUB_

#include <QObject>
#include <QWidget>

namespace ktomgirl{
class Stub : QWidget
{
	Q_OBJECT
public:
	Stub(QWidget* pParent = NULL, const char* szName = NULL);
	~Stub();

}; // class Stub
} // namespace ktomgirl
#endif // _KTG_STUB_
// Mon Jul 30 10:44:25 PDT 2012
