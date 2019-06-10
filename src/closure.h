#ifndef N
#error call this in a namespace thingy
#endif

#include <stddef.h> // NULL
#include <assert.h>

/*
  Be careful these types are simple, anything refcounted or otherwise screwed up by
  naive struct copy should be pointers!
*/
typedef struct N(args) {
#define X(type, name) type name;
	FOR_ARGS
#undef X		
} N(args);

typedef void (*N(callback))(struct N(args));

typedef struct N(closure) {
	struct N(args);
	N(callback) call;
} N(closure);

static
void N(call_closure)(N(closure) self) {
	return self.call(self.N(args));
}

#undef FOR_ARGS
