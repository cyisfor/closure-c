#ifndef CLOSURE_INTERFACE_H
#define CLOSURE_INTERFACE_H

#include "CLOSURE_deps.h"
#include <assert.h>

typedef RETURNS (*CLOSURE_call)(
	void* arg,
	FOR_TYPES INIT
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
	CLOSURE_call call;
	void* arg;
	FOR_TYPES INIT
	type name;
	END_FOR_TYPES;
	FOR_TYPES
	type name;
	END_FOR_TYPES;
};

static
struct CLOSURE CLOSURE(
	CLOSURE_call call,
	void* arg,
	struct CLOSURE closure) {
	assert(call);
	assert(closure.call == NULL); // ehhhh
	closure.call = call;
	closure.arg = arg;
	return closure;
}

static
RETURNS call_CLOSURE(
	const struct CLOSURE* self,
	FOR_TYPES
	type name,
	END_FOR_TYPES) {
	return self->call(
		self->arg,
		FOR_TYPES INIT
		self->name,
		END_FOR_TYPES,
		FOR_TYPES
		name,
		END_FOR_TYPES);
}

#endif /* CLOSURE_INTERFACE_H */
