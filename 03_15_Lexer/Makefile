INPUT ?= ./data/program.txt

CC = clang
CFLAGS = -Wno-everything -pthread -lm -nostartfiles
PROGRAM_NAME = lexer

build:
	$(CC) $(CFLAGS) -o ./$(PROGRAM_NAME) ./main.c -e $(PROGRAM_NAME)_main

run:
	clear
	./$(PROGRAM_NAME) $(INPUT)

clean:
	rm -rf ./$(PROGRAM_NAME)
