#include "mytring.h"
#define I(a) { .base = #a, .len = sizeof(#a)-1 }

const struct var vars[] = {
	{I(int), I(foo)},
	{I(double),I(bar)}
};

const struct var init_vars[] = {
	{I(struct thing), I(oof)},
	{I(void*),I(arglebargle)}
};

const string closure_name = I(example_derp);
const string return_type = I(void);
#undef I
