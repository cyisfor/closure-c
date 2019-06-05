#include "output.h"

void output_char(char c) {
	fputc(c, stdout);
}
void output_space(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
void output_return_type() {
	fwrite(LITLEN("herpderp"), 1, stdout);
}
void output_closure_name() {
	fwrite(LITLEN("uv_mainderp_thing"), 1, stdout);
}

void output_string(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
