#include "CLOSURE.h"

struct CLOSURE CLOSURE(
	CLOSURE_call call,
	struct CLOSURE closure) {
	if(call) // ehhh
		closure.call = call; 
	return closure;
}

struct RETURNS call_CLOSURE(
	const CLOSURE* self,
	FOR_TYPES
	type name,
	END_FOR_TYPES) {
	return self->call(
		self->arg,
		FOR_TYPES INIT
		self->name,
		END_FOR_TYPES
		FOR_TYPES
		name,
		END_FOR_TYPES);
}
