#include "mystring.h"
struct thing {
	int foo;
};

#include "example_closure.h"

#include <glib.h> 

struct thing my_callback(struct thing thing, void* arglebargle,
						 char foo, string bar) {
	printf("thing: %d\n", thing.foo);
	printf("arg: %p\n", arglebargle);
	printf("foo: %c\n", foo);
	printf("bar: %.*s\n", (int)bar.len, bar.base);
	thing.foo += 23;
	return thing;
}

int main(int argc, char *argv[])
{
	struct thing thing = {
		.foo = 19
	};
    struct example_derp c = example_derp(
		my_callback, 
		(struct example_derp){
			.oof = thing,
				.arglebargle = (void*)0x23
				});
	struct thing answer = example_derp_call(c, 'a', LITSTR("bee"));
	printf("answer %d\n\n", answer.foo);

	struct example_derp* cc = g_slice_copy(
		sizeof(struct example_derp), &c);
	// then later in some callback...
	answer = example_derp_call(*cc, 'Q', LITSTR("bee Q"));
	g_slice_free(struct example_derp, cc);
	printf("answer %d\n", answer.foo);
    return 0;
}
