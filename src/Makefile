# sigh
all: test_grammar

lex.h lex.yy.c: lex.l
	flex --header-file=lex.h lex.l

grammar.tab.h grammar.tab.c: grammar.y
	bison grammar.y

test_grammar: test_grammar.c grammar.tab.c lex.yy.c output.c
	gcc -ggdb -I ~/code/cystuff/src -o $@ $^
