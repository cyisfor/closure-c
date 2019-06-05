# sigh
all: test_grammar

test_grammar: test_grammar.c grammar.c output.c for_types.c ~/code/cystuff/src/mmapfile.c
	gcc -ggdb -I ~/code/cystuff/src -o $@ $^
