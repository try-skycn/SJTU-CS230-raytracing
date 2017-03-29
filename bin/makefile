CC=c++

SRC=../src
INCLUDE=../include
OBJ=../obj
BIN=../bin

CFLAGS=--std=c++14

.PHONY: main
main: $(SRC)/main.cpp $(DEPS)
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm -f main