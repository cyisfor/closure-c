#include "output.h"

struct var {
	string type;
	string name;
};

#define INITSTR(a) { .base = a, .len = sizeof(a)-1 }

struct var vars[] = {
	{INITSTR("int"), INITSTR("foo")},
	{INITSTR("double"),INITSTR("bar") }
};

struct var init_vars[0] = {};

size_t nvars = sizeof(vars) / sizeof(*vars);
size_t ninit_vars = sizeof(init_vars) / sizeof(*init_vars);

void output_for_types(bool for_init, bool type, bool name, string delim) {
	size_t i;
	size_t n = for_init ? ninit_vars : nvars;
	for(i=0;i<n;++i) {
		struct var* v;
		if(for_init) {
			v = &init_vars[i];
		} else {
			v = &vars[i];
		}
		if(type) {
			fwrite(v->type.base, v->type.len, 1, stdout);
			if(name) {
				fputc(' ', stdout);
			}
		}
		if(name) {
			fwrite(v->name.base, v->name.len, 1, stdout);
		}
		if(i != 0) {
			fwrite(delim.base, delim.len, 1, stdout);
		}
	}
}

void output_char(char c) {
//	fputc(c, stdout);
	fprintf(stdout, "C %c\n", c);
}
void output_space(string s) {
	fwrite(s.base, s.len, 1, stdout);
}
void output_return_type() {
	fwrite(LITLEN("derpreturntype"), 1, stdout);
}
void output_closure_name() {
	fwrite(LITLEN("uv_mainderp_thing"), 1, stdout);
}
