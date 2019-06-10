#include "mystring.h"

#define NAMESPACE example
#include "../namespace.h"

struct N(thing) {
	int foo;
};

struct N(args) {
	struct N(thing) foo;
	double bar;
	void* baz;
	string message;
	int* answer;
};
#include "../closure.h"
#include "../end_namespace.h"
