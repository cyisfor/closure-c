#include "parse_for_expression.h"
#include "output.h"

enum failure_state { SUCCESS };

#include "parser-interface.h"
#include "parser-impl.c.h"

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}

void parse_for_expression(string buf, const struct var v) {
	struct parser pp = {
		.buf = buf
	};
	struct parser* p = &pp;
	size_t last_thing = 0;
	void commit(int except_for) {
		if(P(pos) - except_for > last_thing) {
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
				output_string(v.type);
			} else if(consume(p, "name")) {
				commit(4);
				output_string(v.name);
			} else {
				if(p->pos < p->buf.len) {
					output_char(p->buf.base[p->pos]);
				}
				onechar(p);
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "expression %d\n", err);
		abort();
	}
}

