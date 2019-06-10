#include "example.h"

#include <glib.h>

void my_callback(example_args args) {
	printf("foo: %d\n", args.foo.foo);
	printf("bar: %f\n", args.bar);
	printf("baz: %p\n", args.baz);
	printf("message: %.*s\n", (int)args.message.len, args.message.base);
	*args.updater += 19;
	return thing;
}

int main(int argc, char *argv[])
{
	int somenum = 23;
	struct example_closure c = {
		.call = my_callback,
		.foo = {
			.foo = 42
		},
		.bar = 2.7818281828459045,
		.baz = (void*)0x23,
		.message = LITSTR("this is a test"),
		.updater = &somenum
	};

	// do stuff...

	example_call(c);
	printf("answer %d\n", somenum);
    return 0;
}
