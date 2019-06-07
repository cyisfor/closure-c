enum failure_state {
	SUCCESS
};

#define OUTPUT
#include "parser-snippet.h"

static
bool consume_universal_stuff(void) {
	return false;
}

static
void parse_for_types_expression(string buf, string* delim, const struct var v) {
	P(output) = false;
	size_t last_thing = 0;
	void commit(int except_for) {
		if(delim == NULL && pos - except_for > last_thing) {
			output_string((string){
							  buf.base + last_thing,
							  pos - except_for - last_thing
								  });
		}
		last_thing = pos;
	}
	int err = setjmp(onerr);
	if(err == 0) {
		for(;;) {
			// longjmp...
			if(consume("type")) {
				commit(4);
				if(delim == NULL)
					output_string(v.type);
			} else if(consume("name")) {
				commit(4);
				if(delim == NULL)
					output_string(v.name);
			} else {
				onechar();
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
		delim->base = buf.base + last_thing;
		delim->len = buf.len - last_thing;
	}
}
