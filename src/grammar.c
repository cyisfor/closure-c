#include "grammar.h"
#include "mystring.h"
#include "output.h"
#include "for_types.h"

#include <ctype.h>
#include <setjmp.h>
#include <assert.h> //
#include <stdarg.h> // va_*
#include <signal.h> // raise

static
void parse_for_types_expression(string buf, string* delim, const struct var v);

void parse(string buf) {
	enum failure_state {
		SUCCESS,
		PAST_END
	};

	
#define OUTPUT
#include "parser-snippet.h"
	auto bool consume_for_types(void);

	////////////////// main:

	int err = setjmp(onerr);
	if(err == 0) {
		while(pos < buf.len) {
			eat_space();
			if(consume_universal_stuff()) {
			} else if(consume_for_types()) {
			} else {
				onechar();
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "Uhh %d\n", err);
		abort();
	}

	bool consume_universal_stuff(void) {
		if(consume("RETURNS")) {
			output_return_type();
		} else if(consume("SAFE_CLOSURE")) {
			output_closure_name(true);
		} else if(consume("CLOSURE")) {
			output_closure_name(false);
		} else if(consume("PREAMBLE")) {
			output_preamble();
			consume(";"); // maybe
		} else {
			return false;
		}
		return true;
	}
	
	bool consume_for_types(void) {
		eat_space();
		if(pos == buf.len) return false;
		if(!consume("FOR_TYPES")) return false;
		canexit = -1;
		output = false;
		eat_space();
		bool do_init = false;
		size_t start = pos;
		for(;;) {
			if(consume("AUX")) {
				do_init = true;
				eat_space();
				start = pos;
			} else if(consume("END_FOR_TYPES")) {
				break;
			} else {
				if(++pos == buf.len) fail(PAST_END, "EOF without END_FOR_TYPES");
			}
		}
		string expression = {
			.base = buf.base + start,
			.len = pos - LITSIZ("END_FOR_TYPES") - start
		};
		size_t i, n;
		const struct var* types = for_types(do_init, &n);
		if(n == 0) {
			// END_FOR_TYPES followed by stuff that could be useless
			consume(";");
			consume(",");
			consume(".");
			eat_space();
		} else {
			string delim = {};
			for(i=0;i<n;++i) {
				if(i != 0) {
					output_string(delim);
				}
				parse_for_types_expression(
					expression,
					i == 0 ? &delim : NULL,
					types[i]);
			}
		}
		output = true;
		canexit = 1;
		return true;
	}
}

static
void parse_for_types_expression(string buf, string* delim, const struct var v) {
	enum failure_state {
		SUCCESS
	};

#define OUTPUT	
#include "parser-snippet.h"
	bool consume_universal_stuff(void) {
		return false;
	}
	output = false;
	size_t last_thing = 0;
	void commit(int except_for) {
		if(pos - except_for > last_thing) {
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
				output_string(v.type);
			} else if(consume("name")) {
				commit(4);
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
