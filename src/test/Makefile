UI:=$(patsubst ../ui/%.ui,%,$(wildcard ../ui/*.ui))

this:  
	echo $(UI)

test: ui_searchwindow.py ktomgirl_rc  ui_newnotebookdialog.py
	echo $(UI)
	python searchwindow.py


.ui.py:
	pyuic4 ../ui/$(UI).ui > $(UI).ui.py

clean:
	rm -f ui_*.py *.pyc ktomgirl_rc
	
ktomgirl_rc:
	pyrcc4 ../ui/ktomgirl.qrc > ktomgirl_rc.py
	#echo "class ktomgirl_rc():" >> ktomgirl_rc.py
	#echo "pass" >> ktomgirl_rc.py
