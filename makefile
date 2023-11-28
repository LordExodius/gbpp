CXX=g++
CXXFLAGS=--std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.1/include
SFML_LIBS=-lsfml-graphics -lsfml-window -lsfml-system -L/opt/homebrew/Cellar/sfml/2.6.1/lib

DEPS = global.h MMU.h Register.h Cartridge.h Graphics.h
OBJS = test.o MMU.o Register.o Cartridge.o Graphics.o

# Build objects
# $@ : Name of target being generated
# $< : Filename of first prerequisite

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

emu: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

clean:
	rm -f emu $(OBJS)