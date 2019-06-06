#include "for_types.h"

#include MY_INFO

const struct var* for_types(bool do_aux, size_t* num) {
	if(do_aux) {
		*num = naux_vars;
		return aux_vars;
	} else {
		*num = nvars;
		return vars;
	}
}
