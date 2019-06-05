#include "example_closure.h"

struct thing my_callback(void* arg, struct thing thing, void* arglebargle,
						 char foo, string bar) {
	printf("thing: %d\n", thing.foo);
	printf("arg: %p %p\n", arg, arglebargle);
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
		my_callback, (void*)0x42,
		(struct example_derp){
			.oof = thing,
				.arglebargle = (void*)0x23
				});
	struct thing answer = example_derp_call(c, 'a', LITSTR("bee"));
	printf("answer %d\n", answer.foo);
    return 0;
}