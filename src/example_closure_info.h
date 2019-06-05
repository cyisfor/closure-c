#include "mystring.h"
#include "for_types.h"
#define I(a) { .base = #a, .len = sizeof(#a)-1 }

static
const struct var init_vars[] = {
	{I(struct thing), I(oof)},
	{I(void*),I(arglebargle)}
};


static
const struct var vars[] = {
	{I(char), I(foo)},
	{I(string),I(bar)}
};

static
const string closure_name = I(example_derp);
static
const string return_type = I(struct thing);
#undef I
