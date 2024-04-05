NeanderPath = 02_23_Neander
AssemblerPath = 03_09_Assembler
LexerPath = 03_15_Lexer

Build = && make build -s
Run 	= && make run		-s
Clean = && make clean -s

FinishMessage = "Output file saved at program's folder.\nPress any key to continue"

build:
	@echo "Building Neander Simulator" \
		&& cd $(NeanderPath) $(Build)
		

	@echo "Building Neander Assembler" \
		&& cd $(AssemblerPath) $(Build)

	@echo "Building Language Lexer" \
		&& cd $(LexerPath) $(Build)

run:
	@echo "Running Language Lexer" \
		&& cd $(LexerPath) $(Run) \
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
	@cd $(LexerPath) $(Clean)
	@echo "All Clean!"
