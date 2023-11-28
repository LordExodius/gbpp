CXX=g++
CXXFLAGS=--std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.1/include
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system -L/opt/homebrew/Cellar/sfml/2.6.1/lib

DEPS = global.h CPU.h MMU.h Register.h Cartridge.h Emulator.h Graphics.h catch_amalgamated.hpp
OBJS = test.o CPU.o MMU.o Register.o Cartridge.o Emulator.o Graphics.o catch_amalgamated.o

# Build objects
# $@ : Name of target being generated
# $< : Filename of first prerequisite

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

emu: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

clean:
	rm -f emu $(OBJS)