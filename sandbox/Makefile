CXX=clang++
CXXFLAGS=-g -O2 -std=c++11 -I../deps/fmt/include

NANOLOG_CXXFLAGS=$(CXXFLAGS)

all: os.o format.o
	$(CXX) $(NANOLOG_CXXFLAGS) -o run_test main.cc $? -pthread

os.o: ../deps/fmt/src/os.cc
	$(CXX) $(CXXFLAGS) -c  -Iinclude ../deps/fmt/src/os.cc

format.o: ../deps/fmt/src/format.cc
	$(CXX) $(CXXFLAGS) -c  -Iinclude ../deps/fmt/src/format.cc

.PHONY: clean

clean:
	rm -f os.o format.o run_test
