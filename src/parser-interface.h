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
	fputs("\nleft:",stdout);
	buftail(p->buf, p->pos);
	if(p->pos) {
		fputs("passed:",stdout);
		bufhead(p->buf, p->pos);
		bufaround(p->buf, p->pos, 5);
	}
	if(p->noexit) { fputs("NOEXIT ",stdout); }
#ifdef OUTPUT
	if(p->output) { fputs("OUTPUT ",stdout); }
#endif
	putchar('\n');
}
