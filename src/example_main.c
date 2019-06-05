#include "example_closure.h"

struct thing my_callback(struct thing thing, void* arg, char foo, string bar) {
	printf("thing: %d\n", thing.foo);
	printf("arg: %p\n", arg);
	printf("foo: %c\n", foo);
	printf("bar: %.*s\n", (int)bar.len, bar.base);
	thing.foo += 23;
	return thing;
}

int main(int argc, char *argv[])
{
	struct thing thing = {
		.foo = 19;
	};
    struct example_closure c = example_closure(my_callback, {
			.oof = thing,
				.arg = NULL
				});
	struct thing answer = example_closure_call(c, 'a', LITSTR("bee"));
	printf("answer %d\n", answer.foo);
    return 0;
}
