
OBJ = ./obj
SRC = ./src
LIB = ./lib
APP = ./app
HEADER = ./header

all: libed mytest

libed: \
	$(OBJ)/cwalk.o\
	$(OBJ)/wav.o\
	$(OBJ)/mfcc.o

mytest:
	gcc -lm -g $(SRC)/main.c $(OBJ)/*.o -I $(HEADER) -o $(APP)/main

$(OBJ)/%.o: $(SRC)/%.c $(HEADER)/%.h
	gcc -c -g $< -I $(HEADER) -o $@

clean:
	rm -rf $(OBJ)/* $(APP)/* $(LIB)/*

