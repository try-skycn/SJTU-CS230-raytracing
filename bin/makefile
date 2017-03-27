CC=c++

SRC=../src
INCLUDE=../include
OBJ=../obj
BIN=../bin

_DEPS=printer.hpp scene.hpp tracer.hpp vec.hpp
DEPS=$(patsubst %, $(INCLUDE)/%, $(_DEPS))

CFLAGS=-I$(INCLUDE) --std=c++14

main: $(SRC)/main.cpp $(DEPS)
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)/* main