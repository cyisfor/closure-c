#ifndef N
#error call this in a namespace thingy
#endif
#ifndef CLOSURE_ARGS
#error your closure should have some arguments! like X(type,name) X(type2,name2)
#endif

#include <stddef.h> // NULL
#include <assert.h>

/*
  Be careful these types are simple, anything refcounted or otherwise screwed up by
  naive struct copy should be pointers!
*/
typedef struct N(args) {
#define X(type, name) type name;
	CLOSURE_ARGS
#undef X		
} N(args);

struct N(closure);

typedef void (*N(callback))(struct N(closure)*);
/* args are separate so we can override the signature of call with a new struct */
typedef struct N(closure) {
	N(callback) call;
	N(args);
} N(closure);

static
void N(call)(N(closure)* self) {
	return self->call(self);
}

#undef FOR_ARGS
