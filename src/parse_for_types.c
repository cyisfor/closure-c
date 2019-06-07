#include "parse_for_types.h"
#include "parse_for_expression.h"
#include "mystring.h"
#include "output.h"
#include <assert.h>


enum failure_state {
	SUCCESS,
	TINY_EXPRESSION
};
#include "parser-interface.h"

enum section { NO_SECTION, VAR, AUX, ALL };

struct ftparser {
	struct parser;
	
	bool ready_for_delim;
	string delim;
	enum section previous_section;
	size_t prevpos;
};

#include "parser-impl.c.h"

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}

static string find_delim(struct ftparser* p, string expression) {
	/* minimum expression "name" */
	if(expression.len < 4) {
		fail(p, TINY_EXPRESSION, "Useless tiny expression cannot exist");
	}
	size_t start = expression.len - 4;
	for(;;) {
		bool gotit = false;
		
		if((0 == memcmp(expression.base + start, LITLEN("type"))) ||
		   (0 == memcmp(expression.base + start, LITLEN("name")))) {
			return (string) {
				.base = expression.base + start + 4,
				.len = expression.len - start - 4
			};
		}
		if(start == 0) {
			fail(p, TINY_EXPRESSION, "Useless expression has no type or name.");
		}
		--start;
	}
}

static
void do_one(struct ftparser* p, string delim, string expression, bool aux) {
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
			types[i]);
	}
}

static
void prepare_for_section(struct ftparser* p, size_t goback, enum section which) {
	if(P(previous_section) != NO_SECTION) {
		string expression = {
			.base  = P(buf.base) + P(prevpos),
			.len = P(pos) - P(prevpos) - goback
		};
		string delim = find_delim(p, expression);
		expression.len -= delim.len;
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
		p->delim = delim;
	}
	P(previous_section) = which;
	P(prevpos) = P(pos);
}

bool parse_for_types(string expression, string* outdelim) {
	struct ftparser pp = {
		.buf = expression,
		.noexit = true,
	};
	int res = setjmp(pp.onerr);
	switch(res) {
	case 0:
		break;
	case 1:
		return false;
	default:
		exit(res);
	};
	struct ftparser* p = &pp;
	bool add_tail = false;
	for(;;) {
		eat_space(p);
		if(consume(p, "VAR")) {
			prepare_for_section(p, 3, VAR);
		} else if(consume(p, "AUX")) {
			prepare_for_section(p, 3, AUX);
		} else if(consume(p, "ALL")) {
			prepare_for_section(p, 3, ALL);
		} else if(consume(p, "TAIL")) {
			assert(P(previous_section) == NO_SECTION);
			add_tail = true;
		} else if(consume(p, "HEAD")) {
			assert(P(previous_section) == NO_SECTION);
			P(ready_for_delim) = true;
		} else {
			if(P(pos) == P(buf.len)) {
				prepare_for_section(p, 0,  NO_SECTION);
				if(P(ready_for_delim) && add_tail) {
					output_string(p->delim);
				}
				break;
			}
			++P(pos);
		}
	}
	*outdelim = pp.delim;
	return true;
}

