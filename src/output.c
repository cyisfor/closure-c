#include "output.h"

void output_for_types(bool for_init, bool type, bool name, string delim) {
	// what about self->name = name ? reparsing? unfolding?
	size_t i;
	size_t n = for_init ? ninit_vars : nvars;
	for(i=0;i<n;++i) {
		if(i != 0) {
			fwrite(delim.base, delim.len, 1, stdout);
		}

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
	}
}

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
