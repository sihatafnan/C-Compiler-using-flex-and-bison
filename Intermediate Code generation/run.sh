#!/bin/bash

bison -d -y parser.y
echo 'Generated the parser C file as well the header file'
g++ -w -c -o y.o y.tab.c
echo 'Generated the parser object file'
flex scanner.l
echo 'Generated the scanner C file'
g++ -w -c -o l.o lex.yy.c
# if the above command doesn't work try g++ -fpermissive -w -c -o l.o lex.yy.c
echo 'Generated the scanner object file'
g++ y.o l.o -ll
echo 'All ready, running'
./a.out input.c
