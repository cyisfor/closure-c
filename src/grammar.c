#include "grammar.h"
#include "mystring.h"
#include "output.h"
#include "for_types.h"

#include <ctype.h>
#include <setjmp.h>

static
void parse_for_types_expression(string buf, string* delim, const struct var v);

void parse(string buf) {
#include "parser-snippet.h"
	auto bool advance_for_types(void);

	////////////////// main:

	int err = setjmp(onerr);
	if(err == 0) {
		while(pos < buf.len) {
			eat_space();
			if(advance("RETURNS")) {
				output_return_type();
			} else if(advance("CLOSURE")) {
				output_closure_name();
			} else if(advance_for_types()) {
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
	bool advance_for_types(void) {
		eat_space();
		if(pos == buf.len) return false;
		if(!advance("FOR_TYPES")) return false;
		canexit = -1;
		eat_space();
		bool do_init = false;
		size_t start = pos;
		for(;;) {
			if(advance("INIT")) {
				do_init = true;
				eat_space();
				start = pos;
			} else if(advance("END_FOR_TYPES")) {
				break;
			} else {
				if(++pos == buf.len) longjmp(onerr, 4);
			}
		}
		string expression = {
			.base = buf.base + start,
			.len = pos - LITSIZ("END_FOR_TYPES") - start
		};
		size_t i, n;
		struct var* types = for_types(do_init, &n);
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
		canexit = 1;
		return true;
	}
}

static
void parse_for_types_expression(string buf, string* delim, const struct var v) {
#include "parser-snippet.h"
	int err = setjmp(onerr);
	size_t last_thing = 0;
	if(err == 0) {
		while(pos < buf.len) {
			if(advance("type")) {
				last_thing = pos;
				output_string(v.type);
			} else if(advance("name")) {
				last_thing = pos;
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
	if(delim) {
		delim->base = buf.base + last_thing;
		delim->len = buf.len - last_thing;
	} 	
}
				
	
	
