TARGET = libktomgirl

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

install: build/$(TARGET).so
	cp build/$(TARGET).so /usr/lib
	mkdir -p /usr/include/$(TARGET)
	cp -r *.hpp base /usr/include/$(TARGET)
	ldconfig

deps:
	sudo apt-get install g++ cmake make git libxml2-dev pkg-config libsigc++-2.0-dev libboost-date-time1.42-dev libpcre++-dev libboost-filesystem1.42-dev

