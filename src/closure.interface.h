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

struct CLOSURE;

struct CLOSURE CLOSURE(
	CLOSURE_call call,
	void* arg,
	FOR_TYPES INIT
	type name,
	END_FOR_TYPES);

struct RETURNS call_CLOSURE(
	const CLOSURE* self,
	FOR_TYPES
	type name,
	END_FOR_TYPES);
