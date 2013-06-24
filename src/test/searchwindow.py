# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'searchwindow.ui'
#
# Created: Sun Jun 23 18:58:54 2013
#      by: PyQt4 UI code generator 4.8.5
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_SearchWindow(object):
    def setupUi(self, SearchWindow):
        SearchWindow.setObjectName(_fromUtf8("SearchWindow"))
        SearchWindow.resize(649, 415)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(48)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(SearchWindow.sizePolicy().hasHeightForWidth())
        SearchWindow.setSizePolicy(sizePolicy)
        SearchWindow.setMinimumSize(QtCore.QSize(100, 100))
        SearchWindow.setWindowTitle(QtGui.QApplication.translate("SearchWindow", "KTomGirl", None, QtGui.QApplication.UnicodeUTF8))
        self.widget = QtGui.QWidget(SearchWindow)
        self.widget.setEnabled(False)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.widget.sizePolicy().hasHeightForWidth())
        self.widget.setSizePolicy(sizePolicy)
        self.widget.setMinimumSize(QtCore.QSize(0, 0))
        self.widget.setMaximumSize(QtCore.QSize(0, 0))
        self.widget.setObjectName(_fromUtf8("widget"))
        SearchWindow.setCentralWidget(self.widget)
        self.menubar = QtGui.QMenuBar(SearchWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 649, 27))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuSearch = QtGui.QMenu(self.menubar)
        self.menuSearch.setTitle(QtGui.QApplication.translate("SearchWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuSearch.setObjectName(_fromUtf8("menuSearch"))
        self.menuDebug = QtGui.QMenu(self.menubar)
        self.menuDebug.setTitle(QtGui.QApplication.translate("SearchWindow", "Debug", None, QtGui.QApplication.UnicodeUTF8))
        self.menuDebug.setObjectName(_fromUtf8("menuDebug"))
        SearchWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(SearchWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        SearchWindow.setStatusBar(self.statusbar)
        self.actionNew_Note = QtGui.QAction(SearchWindow)
        self.actionNew_Note.setText(QtGui.QApplication.translate("SearchWindow", "New Note", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNew_Note.setObjectName(_fromUtf8("actionNew_Note"))
        self.actionQuit = QtGui.QAction(SearchWindow)
        self.actionQuit.setText(QtGui.QApplication.translate("SearchWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setObjectName(_fromUtf8("actionQuit"))
        self.actionShow_Open_Notes = QtGui.QAction(SearchWindow)
        self.actionShow_Open_Notes.setText(QtGui.QApplication.translate("SearchWindow", "Show Open Notes", None, QtGui.QApplication.UnicodeUTF8))
        self.actionShow_Open_Notes.setObjectName(_fromUtf8("actionShow_Open_Notes"))
        self.menuSearch.addAction(self.actionNew_Note)
        self.menuSearch.addAction(self.actionQuit)
        self.menuDebug.addAction(self.actionShow_Open_Notes)
        self.menubar.addAction(self.menuSearch.menuAction())
        self.menubar.addAction(self.menuDebug.menuAction())

        self.retranslateUi(SearchWindow)
        QtCore.QMetaObject.connectSlotsByName(SearchWindow)

    def retranslateUi(self, SearchWindow):
        pass

import ktomgirl_rc
