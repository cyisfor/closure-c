#include "showbuf.h"

void showbuf(string s) {
	showstr(s.base, 0, s.len);
}

void buftail(string s, size_t pos) {
	showstr(s.base, pos, s.len);
}

void bufhead(string s, size_t pos) {
	showstr(s.base, 0, pos);
}

void bufaround(string s, size_t pos, size_t margin) {
	size_t from = pos;
	size_t to;
	if(from >= margin) {
		from -= margin;
		to = pos + margin;
	} else {
		to = pos + margin - from;
		from = 0;
	}
	printf("\n.*s\n", to - from, s.base + from);
	size_t i;
	for(i=0;i<from;++i) {
		putchar(' ');
	}
	putchar('^');
	putchar('\n');
}


void showstr(const char* s, size_t from, size_t to) {
	printf("BUF: =====\n%.*s\n====\n", (int)(to - from), s + from);
}
