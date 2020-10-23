CXX = g++
CXXFLAGS = -O0 -g -std=c++11 -shared -fPIC -fopenmp
#CXX = clang++-8
#CXXFLAGS = -Wno-padded -Wno-disabled-macro-expansion -Wno-gnu-statement-expression -Wno-bad-function-cast -fopenmp -O1 -fsanitize=address -fsanitize=undefined -fdenormal-fp-math=ieee -msse2 -march=native
#static analysis in clang using
#scan-build-8 --use-c++=/usr/bin/clang++-8 make
LIB = -lbz2 -lpthread -ldivsufsort
INC = -Inist_impl/cpp $(shell python -m pybind11 --includes)

LIBRARY = sp800_90b$(shell python3-config --extension-suffix)
OBJECTS = data.o bindings.o nist.o

######
# Main operations
######

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LIB) -o $@

%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) $< -c -o $@

clean:
	rm -f $(OBJECTS) $(LIBRARY)
