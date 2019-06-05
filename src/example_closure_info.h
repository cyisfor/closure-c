#include "mystring.h"
#include "for_types.h"
#define I(a) { .base = #a, .len = sizeof(#a)-1 }

static
const struct var vars[] = {
	{I(int), I(foo)},
	{I(double),I(bar)}
};

static
const struct var init_vars[] = {
	{I(struct thing), I(oof)},
	{I(void*),I(arglebargle)}
};

static
const string closure_name = I(example_derp);
static
const string return_type = I(void);
#undef I
