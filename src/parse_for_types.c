#include "parse_for_types.h"
#include "mystring.h"
#include "output.h"

#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h> //


enum failure_state {
	SUCCESS
};

struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
	bool ready_for_delim;
	string delim;
	enum section section;
	enum section previous_section;
	size_t prevpos;
};

#include "parser-snippet.h"

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}

static string find_delim(struct parser* p, string expression) {
	/* minimum expression "type name" */
	if(expression.len < 9) {
		fail(p, TINY_EXPRESSION, "Useless tiny expression cannot exist");
	}
	size_t start = expression.len - 4;
	for(;;) {
		bool gotit = false;
		
		if((0 == memcmp(expression.buf + start, LITLEN("type"))) ||
		   (0 == memcmp(expression.buf + start, LITLEN("name")))) {
			return (string) {
				.buf = expression.buf + start + 4,
				.len = expression.len - start - 4
			};
		}
		if(start < 9) {
			fail(p, TINY_EXPRESSION, "Useless expression has no type or name.");
		}
	}
}

static
void do_one(struct parser* p, string delim, string expression, bool aux) {
	size_t i, n;
	const struct var* types = for_types(aux, &n);
	for(i=0;i<n;++i) {
		if(P(ready_for_delim)) {
			output_string(delim);
		} else {
			P(ready_for_delim) = true;
		}
		parse_for_expression(
			expression,
			NULL,
			types[i]);
	}
}

static
void prepare_for_section(struct parser* p, enum section which) {
	eat_space(p);
	if(P(previous_section) != NO_SECTION) {
		string expression = {
			.base  = P(buf.base) + P(prevpos),
			.len = P(pos) - P(prevpos)
		};
		string delim = find_delim(p, expression);
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
	P(prevpos) = P(pos);
	P(section) = which;
}

bool parse_for_types(string expression, string* outdelim) {
	struct parser pp = {
		.buf = expression,
		.noexit = true,
	};
	struct parser* p = &pp;
	bool add_tail = false;
	for(;;) {
		eat_space(p);
		if(consume(p, "VAR")) {
			prepare_for_section(p, VAR);
		} else if(consume(p, "AUX")) {
			prepare_for_section(p, AUX);
		} else if(consume(p, "ALL")) {
			prepare_for_section(p, ALL);
		} else if(consume(p, "TAIL")) {
			assert(previous_section == NO_SECTION);
			add_tail = true;
		} else if(consume(p, "HEAD")) {
			assert(previous_section == NO_SECTION);
			P(ready_for_delim) = true;
		} else {
			if(++P(pos) == P(buf.len)) {
				prepare_for_section(p, NO_SECTION);
				if(P(ready_for_delim) && add_tail) {
					output_string(p->delim);
				}
				break;
			}
		}
	}
	*outdelim = pp.delim;
	return true;
}

