# C Compiler using Flex and Bison

A compiler for a subset of the C language, built incrementally as the four assignments of
the **CSE-310 Compiler Sessional** course at BUET. Each stage of the classic compiler
pipeline is implemented in its own directory: a hand-written symbol table, a Flex-based
lexical analyzer, a Bison/YACC syntax & semantic analyzer, and finally intermediate code
generation with a simple peephole optimizer that produces 8086 assembly.

## Pipeline / Project Structure

```
.
├── Symbol table/                 # Assignment 1 – symbol table (C++)
│   └── 1705098.cpp               # ScopeTable + SymbolTable with hashing & scoping
├── Lexical Analyzer/             # Assignment 2 – lexer (Flex)
│   ├── 1705098.l                 # Flex specification (tokens, keywords, constants, comments)
│   ├── 1705098_symboltable.h     # Symbol table used by the lexer
│   └── Test Cases/ , input.txt   # Sample inputs and expected logs/tokens
├── Syntax_and_Semantic_Analyzer/ # Assignment 3 – parser + semantic analysis (Flex + Bison)
│   ├── 1705098.l                 # Lexer feeding the parser
│   ├── 1705098.y                 # Bison grammar with semantic actions & type checking
│   ├── symboltable.h             # Shared symbol table
│   └── script.sh                 # Build/run script
└── Intermediate Code generation/ # Assignment 4 – codegen + optimization (Flex + Bison)
    ├── 1705098.l                 # Lexer
    ├── 1705098.y                 # Bison grammar emitting 8086 assembly
    ├── symboltable.h
    ├── code.asm / optimized_code.asm   # Generated and peephole-optimized assembly
    └── run.sh / script.sh        # Build/run scripts
```

Reference material (`flex__bison.pdf`) and a course quiz PDF are also included.

## Topics Covered

Lexical analysis (tokens, patterns, lexemes), syntax analysis (LL/LR parsing with parser
generators), syntax-directed translation (synthesized/inherited attributes), semantic
analysis (type expressions, type equivalence, type checking), run-time environments
(activation records), intermediate code generation (three-address code), code generation,
and basic code optimization.

## Tech Stack

- **C / C++** – semantic actions, symbol table, and the generated scanner/parser code
- **Flex (lex)** – `.l` lexical specifications
- **Bison / YACC** – `.y` grammar specifications
- Target output: **8086 assembly** (Intermediate Code generation stage)

## Build & Run

You need `flex`, `bison` (or `yacc`), and `gcc`/`g++` installed.

### Symbol table (Assignment 1)

```bash
cd "Symbol table"
g++ 1705098.cpp -o symboltable
./symboltable < input.txt
```

### Lexical Analyzer (Assignment 2)

```bash
cd "Lexical Analyzer"
flex 1705098.l
gcc lex.yy.c -lfl
./a.out input.txt        # see command.sh for the exact commands used
```

### Syntax & Semantic Analyzer (Assignment 3) and Intermediate Code Generation (Assignment 4)

Each of these directories ships a build script that runs flex, bison, and the compiler in
order. From inside the relevant directory:

```bash
./script.sh              # (Assignment 4 also provides run.sh)
```

Or run the steps manually:

```bash
bison -d -y 1705098.y
flex 1705098.l
g++ -w -c -o y.o y.tab.c
g++ -w -c -o l.o lex.yy.c
g++ y.o l.o -lfl -o a.out
./a.out input.txt        # input.c for the codegen stage
```

Output (token logs, parse logs, errors, and generated/optimized assembly) is written to the
`*.txt` / `*.asm` files in each directory.
