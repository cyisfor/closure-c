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

// debugging
#include "showbuf.h"
static
void showP(struct parser* p) {
	buftail(p->buf, p->pos);
	if(p->noexit) { fputs("NOEXIT ",stdout); }
#ifdef OUTPUT
	if(p->output) { fputs("OUTPUT ",stdout); }
#endif
	putchar('\n');
}
