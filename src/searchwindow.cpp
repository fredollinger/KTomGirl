#include <qdebug.h>

#include <QMainWindow>

#include "searchwindow.h"
#include "gnote.hpp"

namespace ktomgirl{
SearchWindow::SearchWindow(QWidget* pParent, const char* szName) 
{
 	m_model = new QStringListModel();

	m_list << gnote::Gnote::get_note_list();
     	// m_list << "a" << "b" << "c";
     	m_model->setStringList(m_list);

	setupUi(this);

	tableNotes->setModel(m_model);
}

SearchWindow::~SearchWindow()
{

}
} // namespace ktomgirl
// Wed Mar 28 14:37:50 PDT 2012
