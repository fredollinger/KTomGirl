#ifndef KNOTE_H
#define KNOTE_H

#include <KXmlGuiWindow>

class KTextEdit;

class KNote : public KXmlGuiWindow
{
  Q_OBJECT

  public:
    KNote(QWidget *parent=0);

  private:
    void setupActions();

};// class KNote
#endif // KNOTE_H
