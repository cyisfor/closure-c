#include "CLOSURE.h"

struct CLOSURE {
	void* arg;
	FOR_TYPES INIT
	type name;
	END_FOR_TYPES
	FOR_TYPES
	type name;
	END_FOR_TYPES;
	CLOSURE_call call;
};

struct CLOSURE CLOSURE(
	CLOSURE_call call,
	void* arg,
	FOR_TYPES INIT
	type name,
	END_FOR_TYPES) {
	return (struct CLOSURE) {
		.call = call,
		.arg = arg,
		FOR_TYPES INIT
		.name = name,
		END_FOR_TYPES
	};
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
