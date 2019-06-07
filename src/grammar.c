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

struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
};

	
#define OUTPUT
#include "parser-snippet.h"

static bool consume_for_types(void);

static
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

static
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

static
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


static
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



void parse(string buf) {
	struct parser pp = {};
	struct parser* p = &pp;

	////////////////// main:

	int err = setjmp(pp.onerr);
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
}
