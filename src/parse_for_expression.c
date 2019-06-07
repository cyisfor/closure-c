struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
};

#include "parser-snippet.h"

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}


