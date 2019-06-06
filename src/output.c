#include "output.h"

#include MY_INFO

void output_preamble(void) {
	fwrite(STRANDLEN(preamble), 1, stdout);
}

void output_char(char c) {
	fputc(c, stdout);
}
void output_space(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
void output_return_type() {
	fwrite(STRANDLEN(return_type), 1, stdout);
}
void output_closure_name() {
	fwrite(STRANDLEN(closure_name), 1, stdout);
}

void output_string(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
