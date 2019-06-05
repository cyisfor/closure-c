#include "for_types.h"

#include MY_INFO

size_t nvars = sizeof(vars) / sizeof(*vars);
size_t ninit_vars = sizeof(init_vars) / sizeof(*init_vars);

struct var* for_types(bool do_init, size_t* num) {
	if(do_init) {
		*num = ninit_vars;
		return init_vars;
	} else {
		*num = nvars;
		return vars;
	}
}

void output_return_type() {
	fwrite(LITLEN("herpderp"), 1, stdout);
}
void output_closure_name() {
	fwrite(LITLEN("uv_mainderp_thing"), 1, stdout);
}
