#include "output.h"
#include <ctype.h> // 


#include MY_INFO

static string safe_closure_name = {};

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
void output_closure_name(bool safe) {
	string s = {};
	if(safe) {
		if(safe_closure_name.base == NULL) {
			size_t i = 0;
			bstring lazy = {};
			for(;i<closure_name.len;++i) {
				if(isalnum(closure_name.base[i]) ||
				   closure_name.base[i] == '_') {
					// yay
				} else {
					if(lazy.base == NULL) {
						straddn(&lazy, closure_name.base, closure_name.len);
									}
					lazy.base[i] = '_';
					break;
				}
			}
			if(lazy.base == NULL) {
				safe_closure_name = closure_name;
			} else {
				safe_closure_name = STRING(lazy);
			}
		}
		s = safe_closure_name;			
	} else {
		s = closure_name;
	}
	fwrite(STRANDLEN(s), 1, stdout);
}

void output_string(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
