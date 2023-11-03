CXX=g++
CXXFLAGS=--std=c++17

DEPS = global.h CPU.h MMU.h Register.h
OBJS = 

# Build objects
# $@ : Name of target being generated
# $< : Filename of first prerequisite

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
