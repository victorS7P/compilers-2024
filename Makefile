NeanderPath = 02_23_Neander
AssemblerPath = 03_09_Assembler
ParserPath = 03_22_Parser

Build = && make build -s
Run 	= && make run		-s
Clean = && make clean -s

FinishMessage = "Output file saved at program's folder. Press any key to continue"

CC = clang
CFLAGS = -Wno-everything -pthread -lm
PROGRAM_NAME = parser

SRC = ./src

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

teste:
	$(CC) $(CFLAGS) -o ./vm_test ./tests/vm.test.c

	clear

	@printf "VM TESTS\n"
	@./vm_test
	@printf "\n"

	@rm -rf ./*_test
