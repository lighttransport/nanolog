#
# TODO(LTE): Provide Makefile for fmtlib backend
#
CXXFLAGS=-g -O2 -std=c++11 -Wno-c++98-compat -Wno-c++98-compat-pedantic

all: nanolog.o
	clang++ $(CXXFLAGS) -o run_test -Ideps/pprintpp/include -Iinclude test/main.cc nanolog.o -pthread

nanolog.o: src/nanolog.cc
	clang++ $(CXXFLAGS) -c -Ideps/pprintpp/include -Iinclude src/nanolog.cc

.PHONY: clean

clean:
	rm nanolog.o run_test
