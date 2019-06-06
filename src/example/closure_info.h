#include "mystring.h"
#include "../for_types.h"
#define I(a) { .base = #a, .len = sizeof(#a)-1 }

static
const struct var aux_vars[] = {
	{I(struct thing), I(oof)},
	{I(void*),I(arglebargle)}
};

// note: aux vars could be auxialized first, set to null, set later, etc
// but these vars have to be set at the time of the closure call:
static
const struct var vars[] = {
	{I(char), I(foo)},
	{I(string),I(bar)}
};

static
const string closure_name = I(example_derp);
static
const string return_type = I(struct thing);

static
const size_t nvars = sizeof(vars) / sizeof(*vars);
static
const size_t naux_vars = sizeof(aux_vars) / sizeof(*aux_vars);

static const
string preamble = "#include \"mystring.h\"";
