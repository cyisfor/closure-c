#ifndef CLOSURE_INTERFACE_H
#define CLOSURE_INTERFACE_H

#include "CLOSURE_deps.h"
#include <stddef.h> // NULL
#include <assert.h>

typedef RETURNS (*CLOSURE_callback)(
	FOR_TYPES AUX
	type name,
	END_FOR_TYPES,
	FOR_TYPES
	type name,
	END_FOR_TYPES);

/*
ignore leading space, FOR_TYPES, ignore whitespace, start here, end is at END_FOR_TYPES, comma is
everything between last `name` and END_FOR_TYPES
 */

struct CLOSURE {
	CLOSURE_callback call;
	FOR_TYPES AUX
	type name;
	END_FOR_TYPES;
	FOR_TYPES
	type name;
	END_FOR_TYPES;
};

static
struct CLOSURE CLOSURE(
	CLOSURE_callback call,
	struct CLOSURE closure) {
	assert(call);
	assert(closure.call == NULL); // ehhhh
	closure.call = call;
	return closure;
}

static
RETURNS CLOSURE_call(
	const struct CLOSURE self,
	FOR_TYPES
	type name,
	END_FOR_TYPES) {
	return self.call(
		FOR_TYPES AUX
		self.name,
		END_FOR_TYPES,
		FOR_TYPES
		name,
		END_FOR_TYPES);
}

#endif /* CLOSURE_INTERFACE_H */
