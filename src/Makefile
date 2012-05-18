UIC=uic-qt4
UI_HEADERS= ui_searchwindow.h

# .PHONY: ktgconfigskel.h

all: ui_searchwindow.h ktgconfigskel.h
	mkdir -p build
	cd build && make 2>err
setup:
	mkdir build
	cd build && cmake ..
err:
	vim build/err
clean:
	rm -f ui_searchwindow.h && cd build && make clean
	mv ktgconfigskel.cpp ktgconfigskel.h /tmp
dist-clean:
	rm $(UI_HEADERS)

build/ktomgirl:
	make all
install: build/ktomgirl
	cd build && su -c "make install"
	mkdir /usr/share/kde4/apps/ktomgirl
	cp ui/*.rc /usr/share/kde4/apps/ktomgirl
test:
	build/ktomgirl
	#su -c "make install"
	#/usr/local/bin/ktomgirl ktomgirl.cpp

ui_searchwindow.h:
	$(UIC) searchwindow.ui > ui_searchwindow.h

newprojectlayout4.h:
	uic-qt4 ui/newprojectlayout4.ui > newprojectlayout4.h 


ktgconfigskel.h:
	rm -f ktgconfigskel.h
	kconfig_compiler ktgconfigskel.kcfg ktgconfigskel.kcfgc
deps:
	sudo apt-get install kdelibs5-dev g++ cmake
# Sat Mar 17 17:29:13 PDT 2012
