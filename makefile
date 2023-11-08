CXX=g++
CXXFLAGS=--std=c++17

DEPS = global.h CPU.h MMU.h Register.h Cartridge.h
OBJS = main.o CPU.o MMU.o Register.o Cartridge.o

# Build objects
# $@ : Name of target being generated
# $< : Filename of first prerequisite

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

emu: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f emu $(OBJS)