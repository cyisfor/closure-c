#include "script_info.h"
#include "output.h"
#include "mmapfile.h"

#include <unistd.h> // read
#include <stdlib.h> // realloc
#include <setjmp.h> // 
#include <assert.h> // 
#include <ctype.h> // isspace

static void showbuf(string s, size_t pos) {
	printf("BUF: =====\n%.*s\n====\n", (int)(s.len-pos), s.base+pos);
}

struct var* aux_vars = NULL;
struct var* vars = NULL;

string closure_name = {};
string return_type = {};

size_t nvars = 0;
size_t naux_vars = 0;

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

#include "parser-snippet.h"

	bool consume_line(string* line) {
		size_t start = pos;
		if(!seek(";")) {
			return false;
		}
		line->base = buf.base + start;
		line->len = pos - start;
		++pos; // consume("\n");
		return true;
	}

	bool aux = false;

	bool consume_var(void) {
		size_t start_type = pos;
		size_t end_name;
		size_t end;
		if(seek(";")) {
			end_name = pos;
			++pos;
			end = pos;
		} else {
			end_name = buf.len;
			end = buf.len;
		}
		/* have to work backwards because no space in identifiers */
		size_t start_name = end_name;
		if(end_name <= start_type) {
			pos = start_type;
			return false;
		}
		for(;;) {
			if(isspace(buf.base[start_name-1])) break;
			if(--start_name <= start_type) {
				longjmp(onerr, 6);
			}
		}
		size_t end_type = start_name;
		assert(end_type != start_type);
		for(;;) {
			if(--end_type == start_type) {
				longjmp(onerr, 7);
			}
			if(!isspace(buf.base[end_type-1])) break;
		}			
		pos = end;
		struct var v = {
			.type = (string){
				.base = buf.base + start_type,
				.len = end_type - start_type
			},
			.name = (string) {
				.base = buf.base + start_name,
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
	
	int err = setjmp(onerr);
	if(err == 0) {
		size_t start = pos;
		if(false == consume_line(&closure_name)) {
			longjmp(onerr, 3);
		}
		eat_space();
		if(true == consume("RETURNS:")) {
			eat_space();
			if(false == consume_line(&return_type)) {
				longjmp(onerr, 4);
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
				longjmp(onerr, 5); // onechar(); // XXX: ehhhhh
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
