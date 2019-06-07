#include "parse.h"
#include "parse_for_types.h"
#include "mystring.h"
#include "output.h"
#include "for_types.h"

enum failure_state {
	SUCCESS,
	PAST_END
};
#define OUTPUT

#include "parser-interface.h"
enum section { NO_SECTION, VAR, AUX, ALL };

struct main_parser {
	struct parser;
	bool gotsome;
	bool add_head;
	enum section section;
	enum section previous_section;
	size_t prevpos;
};	

#include "parser-impl.c.h"

static
bool consume_for_types(struct parser* p);

static
bool consume_universal_stuff(struct main_parser* p) {
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


bool consume_for_types(struct main_parser* p) {
	eat_space(p);
	if(P(pos) == P(buf.len)) return false;
	if(!consume(p, "FOR_TYPES")) return false;
	size_t start = P(pos);
	if(consume(p, "END_FOR_TYPES")) {
		string expression = {
			.base = p->buf.base + start,
			.len = P(pos) - start - LITSIZ("END_FOR_TYPES")
		};
		string delim = {};
		if(parse_for_types(expression, &delim)) {
			// END_FOR_TYPES followed by stuff that could be useless
			// XXX: this is a total hack
			string derp = {delim.base,1};
			consumef(p, delim) ||
				consumef(p, derp) ||
				consume(p, ";") ||
				consume(p, ",") ||
				consume(p, "."); // ?
			eat_space(p);
			return true;
		} 
		P(pos) = start;
	}
	return false;
}

void parse(string buf) {
	struct main_parser pp = {
		.buf = buf
	};
	struct main_parser* p = &pp;

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
