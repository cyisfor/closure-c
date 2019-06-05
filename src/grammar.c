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
				break;
			} else {
				onechar();
			}
		}
	}

	
	void eat_space(void) {
		for(;;) {
			if(advance("/*")) {
				eat_comment();
			} else if(advance("//")) {
				for(;;) {
					onechar();
					if(buf.base[pos] == '\n') {
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
			if(advance("RETURNS")) {
				output_return_type();
			} else if(advance("CLOSURE")) {
				output_closure_name();
			} else if(for_types()) {
			} else {
				fputc(buf.base[pos], stdout);
				++pos;
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
				output_for_types(do_init, do_type, false, LITSTR(""));
				longjmp(onerr, 2);
			}
		}
		if(advance("name")) {
			eat_space();
			if(pos == buf.len) {
				output_for_types(do_init, do_type, true, LITSTR(""));
				longjmp(onerr, 3);
			}
		}
		string delim = {
			buf.base + pos,
			1
		};
		while(!isspace(buf.base[++pos])) {
			++delim.len;
			if(pos == buf.len) {
				output_for_types(do_init, do_type, do_name, delim);
				longjmp(onerr, 1);
			}
		}
		while(!advance("END_FOR_TYPES")) {
			if(++pos == buf.len) {
				longjmp(onerr, 3);
			}
		}
			
		output_for_types(do_init, do_type, do_name, delim);
		canexit = 1;
		return true;
	}
}
