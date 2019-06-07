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
	showstr(s.base, from, to);
	size_t i;
	for(i=0;i<from;++i) {
		putchar(' ');
	}
	putchar('^');
	putchar('\n');
}


void showstr(const char* s, size_t from, size_t to) {
	char* ss = malloc(to-from);
	size_t i;
	fputs("\nBUF: =====\n", stdout);
	for(i=0;i<to;++i) {
		switch(s[from+i]) {
		case ' ':
			putchar('-');
			break;
		case '\t':
			fputs("→",stdout);
			break;
		case '\n':
			fputs("¶",stdout);
			break;
		default:
			putchar(s[from+i]);
		};
	}
	fputs("\n=====\n", stdout);
}
