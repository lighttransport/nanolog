CXXFLAGS=-g -O2 -std=c++11 -Wno-c++98-compat -Wno-c++98-compat-pedantic

all: format.o os.o nanolog.o
	clang++ $(CXXFLAGS) -o run_test -Ideps/fmt/include -Iinclude test/main.cc nanolog.o format.o os.o -pthread

nanolog.o: src/nanolog.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include -Iinclude src/nanolog.cc

format.o: deps/fmt/src/format.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include $<

os.o: deps/fmt/src/os.cc
	clang++ $(CXXFLAGS) -c -Ideps/fmt/include $<


.PHONY: clean

clean:
	rm os.o format.o nanolog.o run_test
