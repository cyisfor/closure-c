#ifndef SAFE_CLOSURE_INTERFACE_H
#define SAFE_CLOSURE_INTERFACE_H

PREAMBLE;

#include "concatsym.h"

#include <stddef.h> // NULL
#include <assert.h>

typedef RETURNS (*CONCATSYM(CLOSURE,_callback))(
	FOR_TYPES ALL
	type name,
	END_FOR_TYPES);

/*
ignore leading space, FOR_TYPES, ignore whitespace, start here, end is at END_FOR_TYPES, comma is
everything between last `name` and END_FOR_TYPES
 */

struct CLOSURE {
	CONCATSYM(CLOSURE,_callback) call;
	FOR_TYPES ALL TAIL
	type name;
	END_FOR_TYPES;
};

static
struct CLOSURE CLOSURE(
	CONCATSYM(CLOSURE,_callback) call,
	struct CLOSURE closure) {
	assert(call);
	assert(closure.call == NULL); // ehhhh
	closure.call = call;
	return closure;
}

static
RETURNS CONCATSYM(CLOSURE,_call)(
	const struct CLOSURE self
	FOR_TYPES HEAD
	type name,
	END_FOR_TYPES) {
	return self.call(
		FOR_TYPES AUX TAIL
		self.name,
		END_FOR_TYPES
		FOR_TYPES
		name,
		END_FOR_TYPES);
}

#endif /* SAFE_CLOSURE_INTERFACE_H */
