#ifndef N
#error call this in a namespace thingy
#endif

#include <stddef.h> // NULL
#include <assert.h>

/*
  Be careful these types are simple, anything refcounted or otherwise screwed up by
  naive struct copy should be pointers!
*/
typedef struct N(args) N(args);

struct N(closure);

typedef
#ifdef RETURNS
RETURNS
#undef RETURNS
#else
void
#endif
(*N(callback))(struct N(closure)*);
/* args are separate so we can override the signature of call with a new struct */
typedef struct N(closure) {
	N(callback) call;
	N(args);
} N(closure);

static
#ifdef RETURNS
RETURNS
#undef RETURNS
#else
void
#endif
N(call)(N(closure)* self) {
	return self->call(self);
}

#undef FOR_ARGS
