#include "example_closure.h"

struct thing my_callback(void* arg, struct thing thing, void* arglebargle,
						 int foo, double bar) {
	printf("thing: %d\n", thing.foo);
	printf("arg: %p %p\n", arg, arglebargle);
	printf("foo: %d\n", foo);
	printf("bar: %lf\n", bar);
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
    return 0;
}
