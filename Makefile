LIBTINS=third_party/libtins
LIBTINS_LIB=$(LIBTINS)/build/lib

CFLAGS=-O3 -I$(LIBTINS)/include --std=c++14
LDFLAGS=-L$(LIBTINS_LIB) -Wl,-rpath=$(LIBTINS_LIB) -ltins

all:
	clang++ $(CFLAGS) src/*.cc $(LDFLAGS)
