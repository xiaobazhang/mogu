all:
	mkdir -p build;cd build;cmake -DCMAKE_INSTALL_PREFIX=${MGJ_REL} ..;make

install:
	cd build;make install

clean:
	rm -rf build;rm -rf core.*; rm -rf *.log
