#ifndef CLOSURE_INTERFACE_H
#define CLOSURE_INTERFACE_H

#include "CLOSURE_deps.h"

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
	FOR_TYPES INIT
	type name;
	END_FOR_TYPES
	FOR_TYPES
	type name;
	END_FOR_TYPES
};

struct CLOSURE CLOSURE(
	CLOSURE_call call,
	struct CLOSURE args);

struct RETURNS call_CLOSURE(
	const CLOSURE* self,
	FOR_TYPES
	type name,
	END_FOR_TYPES);

#endif /* CLOSURE_INTERFACE_H */
