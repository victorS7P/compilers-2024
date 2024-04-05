programa ?= ./data/program.txt

CC = clang
CFLAGS = -Wno-everything -pthread -lm
PROGRAM_NAME = assembler

build:
	$(CC) $(CFLAGS) -o ./$(PROGRAM_NAME) ./main.c

run:
	clear
	./$(PROGRAM_NAME) $(programa)

clean:
	rm -rf ./$(PROGRAM_NAME)
