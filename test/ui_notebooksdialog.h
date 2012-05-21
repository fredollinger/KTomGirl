/********************************************************************************
** Form generated from reading UI file 'notebooksdialog.ui'
**
** Created: Mon May 14 18:56:27 2012
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEBOOKSDIALOG_H
#define UI_NOTEBOOKSDIALOG_H

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

class Ui_NotebooksDialog
{
public:
    QWidget *dockWidgetContents;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTableWidget *tableNotebooks;

    void setupUi(QDockWidget *NotebooksDialog)
    {
        if (NotebooksDialog->objectName().isEmpty())
            NotebooksDialog->setObjectName(QString::fromUtf8("NotebooksDialog"));
        NotebooksDialog->resize(584, 394);
        NotebooksDialog->setFeatures(QDockWidget::NoDockWidgetFeatures);
        NotebooksDialog->setAllowedAreas(Qt::LeftDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayoutWidget = new QWidget(dockWidgetContents);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, -10, 581, 381));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableNotebooks = new QTableWidget(verticalLayoutWidget);
        tableNotebooks->setObjectName(QString::fromUtf8("tableNotebooks"));

        verticalLayout->addWidget(tableNotebooks);

        NotebooksDialog->setWidget(dockWidgetContents);

        retranslateUi(NotebooksDialog);

        QMetaObject::connectSlotsByName(NotebooksDialog);
    } // setupUi

    void retranslateUi(QDockWidget *NotebooksDialog)
    {
        NotebooksDialog->setWindowTitle(QApplication::translate("NotebooksDialog", "Notebooks", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NotebooksDialog: public Ui_NotebooksDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTEBOOKSDIALOG_H
