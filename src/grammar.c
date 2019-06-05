#include "grammar.h"
#include "mystring.h"

#include <ctype.h>
#include <setjmp.h>


void parse(string buf) {
	size_t pos = 0;
	bool advancef(string s) {
		size_t left = buf.len - pos;
		if(left < s.len) return false;
		if(0 == memcmp(s.base, buf.base + pos, s.len)) return true;
		return false;
	}
#define advance(lit) advancef(LITSTR(lit))

	jmp_buf onerr;
	int done = -1;
	
	void eat_space(void) {
		while(isspace(buf.base[pos])) {
			if(++pos == buf.len) longjmp(onerr, done);
		}
		if(advance("/*")) {
			for(;;) {
				eat_space();
				if(advance("*/")) break;
			}
		}
		if(advance("//")) {
			while(buf.base[++pos] != '\n') {
				if(pos == buf.len) longjmp(onerr, done);
			}
		}
	}

	auto void for_statement(void);
	
	int err = setjmp(onerr);
	if(err == 0) {
		while(pos < buf.len) {
			if(advance("RETURNS")) {
				output_return_type();
			} else if(advance("CLOSURE")) {
				output_closure_name();
			} else if(for_statement()) {
			} else {
				fputc(buf.base[pos], ' ');
				++pos;
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "Uhh %d\n", err);
		abort();
	}
	bool for_statement(void) {
		eat_space();
		if(pos == buf.len) return false;
		if(!advance("FOR_STATEMENT")) return false;
		eat_space();
		bool do_init = false;
		if(advance("INIT")) {
			do_init = true;
			eat_space();
		}
		bool do_type = false;
		bool do_name = false;
		if(advance("type")) {
			eat_space();
			do_type = true;
			if(pos == buf.len) {
				output_for_statement(do_init, do_type, false, LITSTR(""));
				longjmp(onerr, 2);
			}
		}
		if(advance("name")) {
			eat_space();
			if(pos == buf.len) {
				output_for_statement(do_init, do_type, true, LITSTR(""));
				longjmp(onerr, 3);
			}
		}
		string delim = {
			buf.base + pos,
			1;
		};
		while(!isspace(buf.base[++pos])) {
			++delim.len;
			if(pos == buf.len) {
				output_for_statement(do_init, do_type, do_name, delim);
				longjmp(onerr, 1);
			}
		}
		while(!advance("END_FOR_STATEMENT")) {
			if(++pos == buf.len) {
				longjmp(onerr, 3);
			}
		}
			
		output_for_statement(do_init, do_type, do_name, delim);
		return true;
	}
}
