#SOURCES:=$(patsubst ../ui/%.ui,%,$(wildcard ../ui/*.ui))
SOURCES:=notebooksdialog  notesdialog  searchbar  searchwindow
OBJECTS=$(addsuffix .py,$(SOURCES)) ktomgirl_rc.py

test: $(OBJECTS) 
	./test.py

%.py : %.ui
	pyuic4 $<  > $@
	
clean:
	rm -f ui_*.py *.pyc ktomgirl_rc $(OBJECTS)

dist-clean:
	make clean
	rm -f *.ui
	
ktomgirl_rc.py:
	pyrcc4 ../ui/ktomgirl.qrc > ktomgirl_rc.py

setup:
	cp ../ui/*.ui .
