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
		enum { NO_AUX, AUX, ALL } in_aux = NO_AUX;
		size_t start = pos;
		bool add_tail = false;
		bool add_head = false;
		for(;;) {
			eat_space();
			if(consume("AUX")) {
				in_aux = AUX;
				eat_space();
				start = pos;
			} else if(consume("ALL")) {
				in_aux = ALL;
				eat_space();
				start = pos;
			} else if(consume("TAIL")) {
				add_tail = true;
				eat_space();
				start = pos;
			} else if(consume("HEAD")) {
				add_head = true;
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
		bool gotsome = false;
		string delim = {};
		parse_for_types_expression(
			expression,
			&delim,
			(struct var){});
		if(add_head) {
			output_string(delim);
		}
		void do_one(bool aux) {
			size_t i, n;
			const struct var* types = for_types(aux, &n);
			for(i=0;i<n;++i) {
				if(gotsome) {
					output_string(delim);
				} else {
					gotsome = true;
				}
				parse_for_types_expression(
					expression,
					NULL,
					types[i]);
			}
		}
		switch(in_aux) {
		case NO_AUX:
			do_one(false);
			break;
		case AUX:
			do_one(true);
			break;
		case ALL:
			do_one(true);
			do_one(false);
			break;
		};
		
		if(gotsome) {
			if(add_tail) {
				output_string(delim);
			}
		}
		// END_FOR_TYPES followed by stuff that could be useless
		// XXX: this is a total hack
		string derp = {delim.base,1};
		consumef(delim) ||
			consumef(derp) ||
			consume(";") ||
			consume(",") ||
			consume("."); // ?
		eat_space();
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
