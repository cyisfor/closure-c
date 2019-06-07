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

enum failure_state {
	SUCCESS,
	PAST_END
};

void parse(string buf) {

	
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
		enum section { NO_SECTION, VAR, AUX, ALL };
		enum section section = VAR, previous_section = NO_SECTION;
		size_t start = pos;
		bool add_tail = false;
		bool add_head = false;
		string expression = {};
		bool gotsome = false;
		string delim = {};

		void do_one(bool aux) {
			size_t i, n;
			const struct var* types = for_types(aux, &n);
			for(i=0;i<n;++i) {
				if(gotsome) {
					output_string(delim);
				} else {
					gotsome = true;
					if(add_head) {
						output_string(delim);
					}
				}
				parse_for_types_expression(
					expression,
					NULL,
					types[i]);
			}
		}

		void prepare_for_section(enum section which) {
			eat_space();
			if(previous_section != NO_SECTION) {
				expression.base = buf.base + start;
				expression.len = pos - start;
				parse_for_types_expression(
					expression,
					&delim,
					(struct var){});		
				switch(previous_section) {
				case VAR:
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
			}
			previous_section = section;
			section = which;
		}
		for(;;) {
			eat_space();
			start = pos;
			if(consume("VAR")) {
				prepare_for_section(VAR);
			} else if(consume("AUX")) {
				prepare_for_section(AUX);
			} else if(consume("ALL")) {
				prepare_for_section(ALL);
			} else if(consume("TAIL")) {
				assert(previous_section == NO_SECTION);
				add_tail = true;
			} else if(consume("HEAD")) {
				assert(previous_section == NO_SECTION);
				add_head = true;
			} else if(consume("END_FOR_TYPES")) {
				prepare_for_section(NO_SECTION);
				break;
			} else {
				if(++pos == buf.len) fail(PAST_END, "EOF without END_FOR_TYPES");
			}
		}		
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
