#include "for_types.h"

#include MY_INFO

size_t nvars = sizeof(vars) / sizeof(*vars);
size_t ninit_vars = sizeof(init_vars) / sizeof(*init_vars);

const struct var* for_types(bool do_init, size_t* num) {
	if(do_init) {
		*num = ninit_vars;
		return init_vars;
	} else {
		*num = nvars;
		return vars;
	}
}
