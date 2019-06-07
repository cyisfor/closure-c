#include "parse_for_types.h"
#include "mystring.h"
#include "output.h"

#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h> // 


enum failure_state {
	SUCCESS
};

#define OUTPUT

struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
#ifdef OUTPUT
	bool output;
#endif
};	

#include "parser-snippet.h"

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}

void parse_for_types_expression(string buf, string* delim, const struct var v) {
	struct parser pp = {
		.output = false,
		.buf = buf
	};
	struct parser* p = &pp;
	size_t last_thing = 0;
	void commit(int except_for) {
		if(delim == NULL && P(pos) - except_for > last_thing) {
			output_string((string){
							  P(buf.base) + last_thing,
							  P(pos) - except_for - last_thing
								  });
		}
		last_thing = P(pos);
	}
	int err = setjmp(pp.onerr);
	if(err == 0) {
		for(;;) {
			// longjmp...
			if(consume(p, "type")) {
				commit(4);
				if(delim == NULL)
					output_string(v.type);
			} else if(consume(p, "name")) {
				commit(4);
				if(delim == NULL)
					output_string(v.name);
			} else {
				onechar(p);
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "expression %d\n", err);
		abort();
	}
	// don't commit the last last thing, save for delim
	if(delim) {
		delim->base = P(buf.base) + last_thing;
		delim->len = P(buf.len) - last_thing;
	}
}
