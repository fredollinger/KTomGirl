#!/usr/bin/env python
 
import sys
from PyQt4 import Qt
from searchwindow import Ui_SearchWindow
from notebooksdialog import Ui_NotebooksDialog

class SearchWindow(Ui_SearchWindow, Qt.QMainWindow):
  def __init__(self,parent=None):
    Qt.QMainWindow.__init__(self,parent)
    self.ui = Ui_SearchWindow()
    self.ui.setupUi(self)

class NotebooksDialog(Ui_NotebooksDialog, Qt.QDockWidget):
  def __init__(self,parent=None):
    Qt.QDockWidget.__init__(self,parent)
    self.ui = Ui_NotebooksDialog()
    self.ui.setupUi(self)

class TestApp(Qt.QApplication):
 
    def __init__(self, args):
        """ In the constructor we're doing everything to get our application
            started, which is basically constructing a basic QApplication by 
            its __init__ method, then adding our widgets and finally starting 
            the exec_loop."""
        Qt.QApplication.__init__(self,args)
 
        # We pass None since it's the top-level widget, we could in fact leave 
        # that one out, but this way it's easier to add more dialogs or widgets.
        self.maindialog = SearchWindow()
        self.nbsdialog = NotebooksDialog()

        #self.setMainWidget(self.maindialog)
        self.nbsdialog.show()
        self.maindialog.show()

        #self.exec_loop()
 
## We instantiate a QApplication passing the arguments of the script to it:
#a = Qt.QApplication(sys.argv)
# 
## Add a basic widget to this application:
## The first argument is the text we want this QWidget to show, the second
## one is the parent widget. Since Our "hello" is the only thing we use (the 
## so-called "MainWidget", it does not have a parent.
#hello = Qt.QLabel("Hello, World")
# 
## ... and that it should be shown.
#hello.show()
# 
## Now we can start it.
#a.exec_()

if __name__ == "__main__":
    app = TestApp(sys.argv)
    sys.exit(app.exec_())
