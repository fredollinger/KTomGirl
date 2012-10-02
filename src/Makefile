UIC=uic-qt4
UI_HEADERS= ui_searchwindow.h ui_notesdialog.h ui_notebooksdialog.h ktgconfigskel.h ui_searchbar.h ui_newnotebookdialog.h

# .PHONY: ktgconfigskel.h

all: $(UI_HEADERS)
	mkdir -p build
	cd build && make 2>err
setup:
	mkdir build
	cd build && cmake ..
err:
	vim build/err
clean:
	cd build && make clean
	mv ktgconfigskel.cpp ktgconfigskel.h /tmp

dist-clean:
	make clean
	rm $(UI_HEADERS)

build/ktomgirl:
	make all
install: build/ktomgirl
	cd build && su -c "make install"
	#mkdir /usr/share/kde4/apps/ktomgirl
	cp ui/*.rc /usr/share/kde4/apps/ktomgirl
test:
	build/ktomgirl
	#su -c "make install"
	#/usr/local/bin/ktomgirl ktomgirl.cpp

ui_searchwindow.h:
	$(UIC) ui/searchwindow.ui > ui_searchwindow.h

ui_searchbar.h:
	$(UIC) ui/searchbar.ui > ui_searchbar.h

newprojectlayout4.h:
	uic-qt4 ui/newprojectlayout4.ui > newprojectlayout4.h 


ktgconfigskel.h:
	rm -f ktgconfigskel.h
	kconfig_compiler ktgconfigskel.kcfg ktgconfigskel.kcfgc
deps:
	sudo apt-get install kdelibs5-dev g++ cmake

ui_notesdialog.h:
	$(UIC) ui/notesdialog.ui > ui_notesdialog.h

ui_notebooksdialog.h:
	$(UIC) ui/notebooksdialog.ui > ui_notebooksdialog.h

ui_newnotebookdialog.h:
	$(UIC) ui/newnotebookdialog.ui > ui_newnotebookdialog.h

# Sat May 19 09:33:42 PDT 2012
