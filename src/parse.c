#include "parse.h"
#include "parse_for_types.h"
#include "mystring.h"
#include "output.h"
#include "for_types.h"

#include <ctype.h>
#include <setjmp.h>
#include <assert.h> //
#include <stdarg.h> // va_*
#include <signal.h> // raise

enum failure_state {
	SUCCESS,
	PAST_END
};

#define OUTPUT

enum section { NO_SECTION, VAR, AUX, ALL };

struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
#ifdef OUTPUT
	bool output;
#endif
	bool gotsome;
	bool add_head;
	enum section section;
	enum section previous_section;
};	

#include "parser-snippet.h"

static
bool consume_for_types(struct parser* p);

static
bool consume_universal_stuff(struct parser* p) {
	if(consume(p, "RETURNS")) {
		output_return_type();
	} else if(consume(p, "SAFE_CLOSURE")) {
		output_closure_name(true);
	} else if(consume(p, "CLOSURE")) {
		output_closure_name(false);
	} else if(consume(p, "PREAMBLE")) {
		output_preamble();
		consume(p, ";"); // maybe
	} else {
		return false;
	}
	return true;
}

static
void do_one(struct parser* p, string delim, string expression, bool aux) {
	size_t i, n;
	const struct var* types = for_types(aux, &n);
	for(i=0;i<n;++i) {
		if(P(gotsome)) {
			output_string(delim);
		} else {
			P(gotsome) = true;
			if(P(add_head)) {
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
void prepare_for_section(struct parser* p, size_t start, enum section which) {
	eat_space(p);
	if(P(previous_section) != NO_SECTION) {
		string expression = {
			.base  = P(buf.base) + start,
			.len = P(pos) - start
		};
		string delim = {};
		parse_for_types_expression(
			expression,
			&delim,
			(struct var){});		
		switch(P(previous_section)) {
		case VAR:
			do_one(p, delim, expression, false);
			break;
		case AUX:
			do_one(p, delim, expression, true);
			break;
		case ALL:
			do_one(p, delim, expression, true);
			do_one(p, delim, expression, false);
			break;
		};
	}
	P(previous_section) = P(section);
	P(section) = which;
}

static
bool consume_for_types(struct parser* p) {
	eat_space(p);
	if(P(pos) == P(buf.len)) return false;
	if(!consume(p, "FOR_TYPES")) return false;
	P(noexit) = true;
	P(output) = false;
	eat_space(p);
	enum section section = VAR, previous_section = NO_SECTION;
	size_t start = P(pos);
	bool add_tail = false;
	P(add_head) = false;
	P(gotsome) = false;
	string delim = {};

	for(;;) {
		eat_space(p);
		start = P(pos);
		if(consume(p, "VAR")) {
			prepare_for_section(p, start, VAR);
		} else if(consume(p, "AUX")) {
			prepare_for_section(p, start, AUX);
		} else if(consume(p, "ALL")) {
			prepare_for_section(p, start, ALL);
		} else if(consume(p, "TAIL")) {
			assert(previous_section == NO_SECTION);
			add_tail = true;
		} else if(consume(p, "HEAD")) {
			assert(previous_section == NO_SECTION);
			P(add_head) = true;
		} else if(consume(p, "END_FOR_TYPES")) {
			prepare_for_section(p, start, NO_SECTION);
			break;
		} else {
			if(++P(pos) == P(buf.len)) fail(p, PAST_END, "EOF without END_FOR_TYPES");
		}
	}		
	if(P(gotsome)) {
		if(add_tail) {
			output_string(delim);
		}
	}
	// END_FOR_TYPES followed by stuff that could be useless
	// XXX: this is a total hack
	string derp = {delim.base,1};
	consumef(p, delim) ||
		consumef(p, derp) ||
		consume(p, ";") ||
		consume(p, ",") ||
		consume(p, "."); // ?
	eat_space(p);
	p->output = true;
	p->noexit = false;
	return true;
}

void parse(string buf) {
	struct parser pp = {};
	struct parser* p = &pp;

	////////////////// main:

	int err = setjmp(pp.onerr);
	if(err == 0) {
		while(P(pos) < P(buf.len)) {
			eat_space(p);
			if(consume_universal_stuff(p)) {
			} else if(consume_for_types(p)) {
			} else {
				onechar(p);
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "Uhh %d\n", err);
		abort();
	}
}
