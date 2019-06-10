#include "mystring.h"

#define NAMESPACE example
#include "../namespace.h"

struct N(thing) {
	int foo;
};

#define FOR_TYPES X(struct N(thing), foo) X(double,bar) X(void*,baz) X(string,message) X(int*, updater)
#include "../closure.h"
#include "../end_namespace.h"
