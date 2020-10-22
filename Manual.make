CXX = g++
CXXFLAGS = -O3 -std=c++11 -shared -fPIC -fopenmp
#CXX = clang++-8
#CXXFLAGS = -Wno-padded -Wno-disabled-macro-expansion -Wno-gnu-statement-expression -Wno-bad-function-cast -fopenmp -O1 -fsanitize=address -fsanitize=undefined -fdenormal-fp-math=ieee -msse2 -march=native
#static analysis in clang using
#scan-build-8 --use-c++=/usr/bin/clang++-8 make
LIB = -lbz2 -lpthread -ldivsufsort
INC = -Inist_impl/cpp $(shell python -m pybind11 --includes)

LIBRARY = sp800_90b$(shell python3-config --extension-suffix)

######
# Main operations
######

all: $(LIBRARY)

$(LIBRARY): iid_lib.o
	$(CXX) $(CXXFLAGS) iid_lib.o $(LIB) -o $(LIBRARY)

clean:
	rm -f iid_lib.o $(LIBRARY)

iid_lib.o: src/*
	$(CXX) $(CXXFLAGS) $(INC) src/iid_lib.cpp -c -o iid_lib.o
