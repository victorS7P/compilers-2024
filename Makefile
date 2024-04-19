CC = clang
CFLAGS = -Wno-everything -pthread -lm
PROGRAM = ./build/program

MODULE_TO_TEST = utils
RUN_TESTS = @ \
	$(CC) $(CFLAGS) -o ./$(MODULE_TO_TEST)_test ./tests/$(MODULE_TO_TEST).test.c && \
	echo "$(MODULE_TO_TEST) TESTS\n\n" && ./$(MODULE_TO_TEST)_test && \
	echo "\n" && echo "Tests done! Press any key to continue ..." && read && clear

run:
	@clear
	$(CC) $(CFLAGS) -o $(PROGRAM) ./src/main.c
	@echo "Program built! Press any key to run ..." && read && clear

	@$(PROGRAM)

clean:
	@rm -rf $(PROGRAM)
	@rm -rf ./debug
	@rm -rf ./out
	@echo "All Clean!"

test:
	clear

	$(eval MODULE_TO_TEST = utils)
	$(RUN_TESTS)

	$(eval MODULE_TO_TEST = vm)
	$(RUN_TESTS)
	
	$(eval MODULE_TO_TEST = assembler)
	$(RUN_TESTS)

	$(eval MODULE_TO_TEST = lexer)
	$(RUN_TESTS)

	$(eval MODULE_TO_TEST = parser)
	$(RUN_TESTS)

	@rm -rf ./*_test
