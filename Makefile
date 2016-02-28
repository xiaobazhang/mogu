ifeq ($(MGJ_REL), )
	export MGJ_REL=/usr/local
endif

all:
	mkdir -p build;
	cd build;cmake -DCMAKE_INSTALL_PREFIX=${MGJ_REL} ..;make

clean:
	rm -rf build;rm -rf core.*;

install:
	cd build;make install;
