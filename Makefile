# Nanolog with internal backend.
#
CXXFLAGS=-g -O2 -Wall -Werror -std=c++11 -Wno-c++98-compat -Wno-c++98-compat-pedantic

all: nanolog.o
	g++ $(CXXFLAGS) -o run_test -Iinclude test/main.cc nanolog.o -pthread

nanolog.o: src/nanolog.cc
	g++ $(CXXFLAGS) -c -Iinclude src/nanolog.cc

.PHONY: clean

clean:
	rm nanolog.o run_test
