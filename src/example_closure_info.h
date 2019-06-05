#include "mytring.h"
#define INITSTR(a) { .base = a, .len = sizeof(a)-1 }

struct var vars[] = {
	{INITSTR("int"), INITSTR("foo")},
	{INITSTR("double"),INITSTR("bar") }
};

struct var init_vars[] = {
	{INITSTR("struct thing"), INITSTR("oof")},
	{INITSTR("void*"),INITSTR("arglebargle") }
};

string closure_name = INITSTR("example_derp");
string return_type = INITSTR("void");
