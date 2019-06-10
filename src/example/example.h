#include "mystring.h"

#define NAMESPACE example
#include "../namespace.h"

struct N(thing) {
	int foo;
};

#define CLOSURE_ARGS X(struct N(thing), foo) X(double,bar) X(void*,baz) X(string,message) X(int*, answer)
#include "../closure.h"
#include "../end_namespace.h"
