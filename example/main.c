#include "example.h"

#include <glib.h>

void my_callback(example_closure* args) {
	printf("foo: %d\n", args->foo.foo);
	printf("bar: %f\n", args->bar);
	printf("baz: %p\n", args->baz);
	printf("message: %.*s\n", (int)args->message.len, args->message.base);
	*args->answer = (*args->answer + 419) / 11;
}

int main(int argc, char *argv[])
{
	int somenum = 9 * 6;
	struct example_closure c = {
		.call = my_callback,
		.foo = {
			.foo = 82
		},
		.bar = 2.7818281828459045,
		.baz = (void*)0x23,
		.message = LITSTR("this is a test"),
		.answer = &somenum
	};

	// do stuff...

	example_call(&c);
	printf("\nanswer? %d\n\n", somenum);
	c.baz = (void*)(long int)somenum;
	example_call(&c);
	printf("\nanswer %d\n", somenum);

    return 0;
}
