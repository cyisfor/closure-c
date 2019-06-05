#include "grammar.h"
#include "mystring.h"
#include "output.h"

#include <ctype.h>
#include <setjmp.h>


void parse(string buf) {
	size_t pos = 0;
	bool advancef(string s) {
		size_t left = buf.len - pos;
		if(left < s.len) return false;
		if(0 == memcmp(s.base, buf.base + pos, s.len)) {
			pos += s.len;
			return true;
		}
		return false;
	}
#define advance(lit) advancef(LITSTR(lit))

	jmp_buf onerr;
	int canexit = 1;

	void onechar(void) {
		fputc(buf.base[pos],stdout);
		if(++pos == buf.len) longjmp(onerr, canexit);
	}

	auto void eat_space(void);

	void eat_comment(void) {
		for(;;) {
			eat_space();
			if(advance("*/")) {
				output_string(LITSTR("*/"));
				break;
			} else {
				onechar();
			}
		}
	}


	void eat_space(void) {
		for(;;) {
			if(advance("/*")) {
				output_string(LITSTR("/*"));
				eat_comment();
			} else if(advance("//")) {
				output_string(LITSTR("//"));
				for(;;) {
					onechar();
					if(buf.base[pos] == '\n') {
						onechar();
						break;
					}
				}
			} else if(isspace(buf.base[pos])) {
				onechar();
			} else {
				return;
			}
		}
	}

	auto bool for_types(void);

	////////////////// main:

	int err = setjmp(onerr);
	if(err == 0) {
		while(pos < buf.len) {
			eat_space();
			if(advance("RETURNS")) {
				output_return_type();
			} else if(advance("CLOSURE")) {
				output_closure_name();
			} else if(for_types()) {
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
	bool for_types(void) {
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
			} else if(advance("END_FOR_TYPES")) {
				string expression = {
					.base = buf.base + start,
					.len = pos - LITSIZ("END_FOR_TYPES") - start
				};
				printf("DERP (%.*s)\n", expression.len, expression.base);
				break;
			} else {
				if(++pos == buf.len) longjmp(onerr, 4);
			}
		}
		
		canexit = 1;
		return true;
	}
}
