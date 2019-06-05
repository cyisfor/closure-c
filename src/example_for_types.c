#include "for_types.h"

#define INITSTR(a) { .base = a, .len = sizeof(a)-1 }

struct var vars[] = {
	{INITSTR("int"), INITSTR("foo")},
	{INITSTR("double"),INITSTR("bar") }
};

struct var init_vars[] = {
	{INITSTR("struct thing"), INITSTR("oof")},
	{INITSTR("void*"),INITSTR("arglebargle") }
};

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
