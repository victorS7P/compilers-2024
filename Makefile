Build = && make build -s
Run 	= && make run		-s
Clean = && make clean -s

FinishMessage = "Output file saved at program's folder. Press any key to continue"

CC = clang
CFLAGS = -Wno-everything -pthread -lm
PROGRAM_NAME = parser

SRC = ./src
TEST_DONE_MESSAGE = "Tests done! Press any key to continue ..."

build:
	@echo "Building Neander Simulator" \
		&& cd $(NeanderPath) $(Build)

	@echo "Building Neander Assembler" \
		&& cd $(AssemblerPath) $(Build)

	@echo "Building Language Parser" \
		&& cd $(ParserPath) $(Build)

run:
	@echo "Running Language Parser" \
		&& cd $(ParserPath) $(Run) \
		&& echo $(FinishMessage) && read

	@echo "Running Neander Assembler" \
		&& cd $(AssemblerPath) $(Run) \
		&& echo $(FinishMessage) && read

	@echo "Running Neander Simulator" \
		&& cd $(NeanderPath) $(Run) \
		&& echo $(FinishMessage) && read

clean:
	@cd $(NeanderPath) $(Clean)
	@cd $(AssemblerPath) $(Clean)
	@cd $(ParserPath) $(Clean)
	@echo "All Clean!"

test:
	clear

	@$(CC) $(CFLAGS) -o ./utils_test ./tests/utils.test.c
	@printf "UTILS TESTS\n\n" && ./utils_test
	@printf "\n" && echo $(TEST_DONE_MESSAGE) && read && clear

	@$(CC) $(CFLAGS) -o ./vm_test ./tests/vm.test.c
	@printf "VM TESTS\n\n" && ./vm_test
	@printf "\n" && echo $(TEST_DONE_MESSAGE) && read && clear
	
	@$(CC) $(CFLAGS) -o ./assembler_test ./tests/assembler.test.c
	@printf "ASSEMBLER TESTS\n\n" && ./assembler_test
	@printf "\n" && echo $(TEST_DONE_MESSAGE) && read && clear

	@$(CC) $(CFLAGS) -o ./lexer_test ./tests/lexer.test.c
	@printf "lexer TESTS\n\n" && ./lexer_test
	@printf "\n" && echo $(TEST_DONE_MESSAGE) && read && clear

	@$(CC) $(CFLAGS) -o ./parser_test ./tests/parser.test.c
	@printf "parser TESTS\n\n" && ./parser_test
	@printf "\n" && echo $(TEST_DONE_MESSAGE) && read && clear

	@rm -rf ./*_test
