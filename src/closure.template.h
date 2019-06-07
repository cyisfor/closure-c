#pragma once

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
	FOR_TYPES TAIL ALL
	type name;
	END_FOR_TYPES
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
	const struct CLOSURE selfFOR_TYPES HEAD VAR
	type name,
	END_FOR_TYPES) {
	return self.call(
		FOR_TYPES AUX
		self.name,
		VAR
		name,
		END_FOR_TYPES);
}
