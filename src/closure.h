#pragma once
PREAMBLE;
#include "concatsym.h"

#include <stddef.h> // NULL
#include <assert.h>

#define CALLBACK CONCATSYM(NAME,_callback)

/*
  Be careful these types are simple, anything refcounted or otherwise screwed up by
  naive struct copy should be pointers!
*/
struct NAME {
	FOR_TYPES TAIL ALL
	type name;
	END_FOR_TYPES
};

typedef void (*CALLBACK)(struct NAME);

struct CONCATSYM(NAME,_closure) {
	struct NAME;
	CALLBACK call;
};

static
void CONCATSYM(NAME,_call)(struct CONCATSYM(NAME,_closure) self) {
	return self.call(self.NAME);
}

/* wow, this would work with just cpp! I'm such a fool. */
#undef CALLBACK
