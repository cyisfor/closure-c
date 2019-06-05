#include "for_types.h"

#include MY_INFO

const struct var* for_types(bool do_init, size_t* num) {
	if(do_init) {
		*num = ninit_vars;
		return init_vars;
	} else {
		*num = nvars;
		return vars;
	}
}
