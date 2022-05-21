
OBJ = ./obj
SRC = ./src
LIB = ./lib
APP = ./app
HEADER = ./header

all: libed mytest

libed: \
	$(OBJ)/mfcc.o

mytest:
	gcc -lm -g $(SRC)/main.c $(OBJ)/*.o -I $(HEADER) -o $(APP)/main

mytestWav:
	gcc -lm -g $(SRC)/teste.c -o $(APP)/teste

$(OBJ)/%.o: $(SRC)/%.c $(HEADER)/%.h
	gcc -c -g $< -I $(HEADER) -o $@

clean:
	rm -rf $(OBJ)/* $(APP)/* $(LIB)/*

cleanApp:
	rm -rf $(APP)/*
