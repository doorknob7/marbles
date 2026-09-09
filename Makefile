CXX= g++
CXXFLAGS = -I. -Iinclude -Wall -std=c++17
DEPS = marbles.h action.h storage.h controller.h view.h
OBJ = main.o marbles.o action.o storage.o controller.o view.o
PREFIX ?= $(HOME)/.local

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