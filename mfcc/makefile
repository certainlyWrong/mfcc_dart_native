
OBJ = ./obj
SRC = ./src
LIB = ./lib
APP = ./app
HEADER = ./header

all: libed mytest

libed: \
	$(OBJ)/mfcc.o

mytest:
	gcc -g $(SRC)/main.c $(OBJ)/*.o -I $(HEADER) -o $(APP)/main -lm

mytestWav:
	gcc -g $(SRC)/teste.c -o $(APP)/teste -lm

$(OBJ)/%.o: $(SRC)/%.c $(HEADER)/%.h
	gcc -c -g $< -I $(HEADER) -o $@ -lm

clean:
	rm -rf $(OBJ)/* $(APP)/* $(LIB)/*

cleanApp:
	rm -rf $(APP)/*
