CXX=g++
CXXFLAGS=--std=c++17
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system

DEPS = global.h CPU.h MMU.h Register.h Cartridge.h Emulator.h Graphics.h
OBJS = test.o CPU.o MMU.o Register.o Cartridge.o Emulator.o Graphics.o

# Build objects
# $@ : Name of target being generated
# $< : Filename of first prerequisite

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

emu: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

clean:
	rm -f emu $(OBJS)