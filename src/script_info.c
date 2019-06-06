#include "script_info.h"
#include "output.h"
#include "mmapfile.h"
#include "mystring.h"

#include <unistd.h> // read
#include <stdlib.h> // realloc
#include <setjmp.h> // 
#include <assert.h> // 
#include <ctype.h> // isspace
#include <stdarg.h> // va_*
#include <signal.h> // raise



static void showbuf(string s, size_t pos) {
	printf("BUF: =====\n%.*s\n====\n", (int)(s.len-pos), s.base+pos);
}

struct var* aux_vars = NULL;
struct var* vars = NULL;
size_t nvars = 0;
size_t naux_vars = 0;

bstring preamble = {};

string closure_name = {};
string return_type = {};


void script_info_load(int fd) {
	size_t size = 0;
	char* script = mmapfd(fd, &size);
	if(script == NULL) {
		size_t space = 0;
		for(;;) {
			if(space - size < 1024) {
				space += 1024;
				script = realloc(script, space);
			}
			ssize_t amt = read(fd, script+size, 1024);
			if(amt == 0) break;
			assert(amt > 0);
			size += amt;
		}
	}
			
	string buf = {script, size};

	enum failure_state {
		SUCCESS,
		NO_SPACES_FOUND,
		NO_NAME,
		NO_RETURN,
		EXTRANEOUS
	};
	
#include "parser-snippet.h"

	bool consume_statement(string* line) {
		size_t start = pos;
		if(seek(";")) {
			line->base = buf.base + start;
			line->len = pos - start;
			++pos; // consume(";");
		} else {
			line->base = buf.base + start;
			line->len = buf.len - start;
			pos = buf.len;
		}
		return true;
	}

	bool aux = false;

	/* smallest var:
	   start_type = 0
	   end_type = 1
	   (1 space)
	   start_name = 3
	   end_name = 4
	*/

	bool consume_var(void) {
		string line;
		size_t oldpos = pos;
		consume_statement(&line);
		size_t start_type = 0;
		size_t end_name = line.len;
		if(end_name < 4) {
			pos = oldpos;
			return false;
		}
		/* have to work backwards because no space in identifiers */
		if(end_name <= start_type) {
			pos = oldpos;
			return false;
		}
		size_t start_name = end_name-1;
		for(;;) {
			if(start_name < 3) {
				pos = oldpos;
				return false;
			}
			if(isspace(line.base[start_name-1])) break;
			--start_name;
		}
		size_t end_type = start_name ;
		for(;;) {
			if(--end_type == start_type) {
				fail(NO_SPACES_FOUND,
					 "no space in argument definition");
			}
			if(!isspace(line.base[end_type-1])) break;
		}
		struct var v = {
			.type = (string){
				.base = line.base + start_type,
				.len = end_type - start_type
			},
			.name = (string) {
				.base = line.base + start_name,
				.len = end_name - start_name
			}
		};
		if(aux) {
			++naux_vars;
			aux_vars = reallocarray(aux_vars,
									 naux_vars,
									 sizeof(*aux_vars));
			assert(aux_vars);
			aux_vars[naux_vars-1] = v;
		} else {
			++nvars;
			vars = reallocarray(vars,
								nvars,
								sizeof(*vars));
			assert(vars);
			vars[nvars-1] = v;
		}
		return true;
	}

	bool consume_include() {
		size_t start = pos;
		eat_space();
		if(consume("#include ")) {
			eat_space();
			size_t start2 = pos;
			if(seek("\n")) {
				straddn(&preamble, LITLEN("#include "));
				straddn(&preamble,
						buf.base + start2,
						pos - start2);
				straddn(&preamble, "\n", 1);
				return true;
			} else {
				pos = start;
				return false;
			}
		}
		pos = start;
		return false;
	}	
	
	int err = setjmp(onerr);
	if(err == 0) {
		size_t start = pos;
		while(consume_include());
			
		if(false == consume_statement(&closure_name)) {
			fail(NO_NAME, "no closure name specified");
		}
		eat_space();
		if(true == consume("RETURNS:")) {
			eat_space();
			if(false == consume_statement(&return_type)) {
				fail(NO_RETURN, "RETURNS: without return type");
			}
		} else {
			return_type = LITSTR("void");
		}
		aux = false;
		for(;;) {
			eat_space();
			if(consume("AUX:")) {
				aux = true;
			} else if(consume_var()) {
				// ok
			} else if(buf.len == pos) {
				break;
			} else {
				fail(EXTRANEOUS, "extraneous characters");
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "uhhh %d\n", err);
		fprintf(stderr, "what '%.*s'\n",
				(int)(buf.len - pos), buf.base+pos);
		abort();
	}
}
