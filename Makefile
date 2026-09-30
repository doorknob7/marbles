CXX = g++
CXXFLAGS = -I. -Iinclude -Wall -Wextra -std=c++17
DEPS = Marbles.h Action.h Storage.h Controller.h View.h Utils.h
OBJ = Main.o Marbles.o Action.o Storage.o Controller.o View.o Utils.o
PREFIX ?= $(HOME)/.local

all: marbles

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

marbles: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

install: marbles
	mkdir -p $(PREFIX)/bin
	cp marbles $(PREFIX)/bin/marbles

uninstall:
	rm -f $(PREFIX)/bin/marbles

clean:
	rm -f *.o marbles

.PHONY: all clean install uninstall