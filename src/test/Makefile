UI=searchwindow newnotebookdialog notebooksdialog notesdialog searchbar 

test: ui_searchwindow.py ktomgirl_rc
	python searchwindow.py

ui_$(UI).py:
	pyuic4 ../ui/$(UI).ui > ui_$(UI).py

clean:
	rm -f ui_$(UI).py *.pyc ktomgirl_rc
	
ktomgirl_rc:
	pyrcc4 ../ui/ktomgirl.qrc > ktomgirl_rc.py
	#echo "class ktomgirl_rc():" >> ktomgirl_rc.py
	#echo "pass" >> ktomgirl_rc.py
