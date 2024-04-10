INPUT ?= ./data/program.txt

CC = clang
CFLAGS = -Wno-everything -pthread -lm
PROGRAM_NAME = parser

build:
	$(CC) $(CFLAGS) -o ./$(PROGRAM_NAME) ./main.c

run:
	clear
	./$(PROGRAM_NAME) $(INPUT)

clean:
	rm -rf ./$(PROGRAM_NAME)
