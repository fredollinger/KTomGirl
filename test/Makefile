UIC=uic-qt4

.PHONY: ktgconfig.h

all: 
	mkdir -p build
	cd build && make 2>err
setup:
	mkdir build
	cd build && cmake ..
err:
	vim build/err
clean:
	cd build && make clean
dist-clean:
	rm $(UI_HEADERS)

build/ktomgirl:
	make all
install: build/ktomgirl
	cd build && su -c "make install"
	#cd build && sudo make install
test:
	build/ktomgirl
	#su -c "make install"
	#/usr/local/bin/ktomgirl ktomgirl.cpp

newprojectlayout4.h:
	uic-qt4 ui/newprojectlayout4.ui > newprojectlayout4.h 


kgtconfig.h:
	rm -f ktgconfig.h
	kconfig_compiler ktgconfig.kcfg ktgconfig.kcfgc
deps:
	sudo apt-get install kdelibs5-dev g++ cmake
# Sat Mar 17 17:29:13 PDT 2012
