CC=c++

SRC=../src
INCLUDE=../include
OBJ=../obj
BIN=../bin

CFLAGS=--std=c++14

main: $(SRC)/main.cpp $(DEPS)
	$(CC) $< -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)/* main