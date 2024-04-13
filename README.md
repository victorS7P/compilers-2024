O que foi feito
  - Emulador Neander
  - Assembler
  - Lexer + Parser

O que faltou fazer
  - O Assembler e o Emulador Neander estão funcionando por um .txt representando a memória do Neander, faltou fazer o Assembler gerar um arquivo .mem como o do Neander, bem como o Emulador ler e rodar a partir desse aqruivo, pra debugar precisamos setar a memória do Neander com base no .txt gerado.
  - O Lexer e o Parser só estão fazendo operações de adição e subtração.
  - Desacoplar o código do Repl.it
  - Refatorar o código e criar testes

Como testar seu código
  - Editar o arquivo 03_22_Parser/data/program.txt com a espressão desejada
  - Rodar os comandos `make build` e então `make run` da raíz do projeto
  - Cada módulo vai rodar um depois do outro (Lexer > Parser > Assembler > Emalador)
    - Teclar Enter para fazer a execução do próximo módulo
  - Ao final, dentro da pasta de cada módulo, vamos ter o arquivo de entrada em `/data` e o gerado em `/out`
    - O arquivo em `/03_09_Assembler/out/program.txt` representa a memória que precisamos setar no Nenader para debugar o processamento do Emulador

Quais as limitações conhecidas
  - As já citadas na sessão "O que faltou fazer"
