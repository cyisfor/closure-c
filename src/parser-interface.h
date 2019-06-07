#include "mystring.h"
#include <setjmp.h>
#include <string.h>

struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
#ifdef OUTPUT
	bool output;
#endif
};
