/********************************************************************************
** Form generated from reading UI file 'searchwindow.ui'
**
** Created: Mon May 14 18:56:27 2012
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHWINDOW_H
#define UI_SEARCHWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchWindow
{
public:
    QAction *actionNew_Note;
    QWidget *widget;
    QMenuBar *menubar;
    QMenu *menuSearch;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *SearchWindow)
    {
        if (SearchWindow->objectName().isEmpty())
            SearchWindow->setObjectName(QString::fromUtf8("SearchWindow"));
        SearchWindow->resize(649, 415);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(48);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SearchWindow->sizePolicy().hasHeightForWidth());
        SearchWindow->setSizePolicy(sizePolicy);
        SearchWindow->setMinimumSize(QSize(100, 100));
        actionNew_Note = new QAction(SearchWindow);
        actionNew_Note->setObjectName(QString::fromUtf8("actionNew_Note"));
        widget = new QWidget(SearchWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(0, 0));
        SearchWindow->setCentralWidget(widget);
        menubar = new QMenuBar(SearchWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 649, 27));
        menuSearch = new QMenu(menubar);
        menuSearch->setObjectName(QString::fromUtf8("menuSearch"));
        SearchWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(SearchWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        SearchWindow->setStatusBar(statusbar);

        menubar->addAction(menuSearch->menuAction());
        menuSearch->addAction(actionNew_Note);

        retranslateUi(SearchWindow);

        QMetaObject::connectSlotsByName(SearchWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SearchWindow)
    {
        SearchWindow->setWindowTitle(QApplication::translate("SearchWindow", "KTomGirl", 0, QApplication::UnicodeUTF8));
        actionNew_Note->setText(QApplication::translate("SearchWindow", "New Note", 0, QApplication::UnicodeUTF8));
        menuSearch->setTitle(QApplication::translate("SearchWindow", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SearchWindow: public Ui_SearchWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHWINDOW_H
