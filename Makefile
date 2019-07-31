CXXFLAGS=-O3 -std=c++11 -Wno-c++98-compat -Wno-c++98-compat-pedantic

all: format.o posix.o nanolog.o
	clang++ $(CXXFLAGS) -o run_test -Ideps/fmt/include -Iinclude test/main.cc nanolog.o format.o posix.o -pthread

nanolog.o: src/nanolog.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include -Iinclude src/nanolog.cc

format.o: deps/fmt/src/format.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include $<

posix.o: deps/fmt/src/posix.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include $<


.PHONY: clean

clean:
	rm posix.o format.o nanolog.o run_test
