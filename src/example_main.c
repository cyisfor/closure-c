#include "example_closure.h"
#include <glib.h> // 


struct thing my_callback(void* arg, struct thing thing, void* arglebargle,
						 char foo, string bar) {
	printf("thing: %d\n", thing.foo);
	printf("arg: %p %p\n", arg, arglebargle);
	printf("foo: %d\n", foo);
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
		my_callback, (void*)0x42,
		(struct example_derp){
			.oof = thing,
				.arglebargle = (void*)0x23
				});
	struct thing answer = call_example_derp(c, 'a', LITSTR("bee"));
	printf("answer %d\n", answer.foo);
	struct example_derp* cc = g_slice_copy(
		sizeof(struct example_derp),
		example_derp(
			my_callback, &answer,
			(struct example_derp) {
				.oof = thing,
					.arglebargle = (void*)answer.foo
					}));
	// ...

	call_example_derp(*cc, 'a', LITSTR("bee Q"));
	g_slice_free(struct example_derp, cc);
    return 0;
}
