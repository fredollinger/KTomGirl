/********************************************************************************
** Form generated from reading UI file 'notesdialog.ui'
**
** Created: Mon May 14 18:56:27 2012
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTESDIALOG_H
#define UI_NOTESDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NotesDialog
{
public:
    QWidget *dockWidgetContents;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableNotes;

    void setupUi(QDockWidget *NotesDialog)
    {
        if (NotesDialog->objectName().isEmpty())
            NotesDialog->setObjectName(QString::fromUtf8("NotesDialog"));
        NotesDialog->resize(511, 376);
        NotesDialog->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayoutWidget = new QWidget(dockWidgetContents);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 511, 351));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableNotes = new QTableWidget(verticalLayoutWidget);
        tableNotes->setObjectName(QString::fromUtf8("tableNotes"));

        verticalLayout->addWidget(tableNotes);

        NotesDialog->setWidget(dockWidgetContents);

        retranslateUi(NotesDialog);

        QMetaObject::connectSlotsByName(NotesDialog);
    } // setupUi

    void retranslateUi(QDockWidget *NotesDialog)
    {
        NotesDialog->setWindowTitle(QApplication::translate("NotesDialog", "Notes", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NotesDialog: public Ui_NotesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTESDIALOG_H
