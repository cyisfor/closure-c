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

struct var* aux_vars = NULL;
struct var* vars = NULL;
size_t nvars = 0;
size_t naux_vars = 0;

bstring preamble = {};

string closure_name = {};
string return_type = {};

enum failure_state {
	SUCCESS,
	NO_SPACES_FOUND,
	NO_NAME,
	NO_RETURN,
	EXTRANEOUS
};


struct parser {
	string buf;
	size_t pos;
	jmp_buf onerr;
	bool noexit;
#ifdef OUTPUT
	bool output;
#endif
};	

#include "parser-snippet.h"

static
void showbuf(string s, size_t pos) {
	printf("BUF: =====\n%.*s\n====\n", (int)(s.len-pos), s.base+pos);
}

static
bool consume_universal_stuff(struct parser* p) {
	return false;
}

static
bool consume_statement(struct parser* p, string* line) {
	size_t start = P(pos);
	if(seek(p, ";")) {
		line->base = P(buf.base) + start;
		line->len = P(pos) - start;
		++P(pos); // consume(";");
	} else {
		line->base = P(buf.base) + start;
		line->len = P(buf.len) - start;
		P(pos) = P(buf.len);
	}
	return true;
}



	/* smallest var:
	   start_type = 0
	   end_type = 1
	   (1 space)
	   start_name = 3
	   end_name = 4
	*/
static
bool consume_var(struct parser* p, bool aux) {
	string line;
	size_t oldpos = P(pos);
	consume_statement(p, &line);
	size_t start_type = 0;
	size_t end_name = line.len;
	if(end_name < 4) {
		P(pos) = oldpos;
		return false;
	}
	/* have to work backwards because no space in identifiers */
	if(end_name <= start_type) {
		P(pos) = oldpos;
		return false;
	}
	size_t start_name = end_name-1;
	for(;;) {
		if(start_name < 3) {
			P(pos) = oldpos;
			return false;
		}
		if(isspace(line.base[start_name-1])) break;
		--start_name;
	}
	size_t end_type = start_name ;
	for(;;) {
		if(--end_type == start_type) {
			fail(p, NO_SPACES_FOUND,
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

static
bool consume_include(struct parser* p) {
	size_t start = P(pos);
	eat_space(p);
	if(consume(p, "#include ")) {
		eat_space(p);
		size_t start2 = P(pos);
		if(seek(p, "\n")) {
			straddn(&preamble, LITLEN("#include "));
			straddn(&preamble,
					P(buf.base) + start2,
					P(pos) - start2 + 1);
			return true;
		} else {
			P(pos) = start;
			return false;
		}
	}
	P(pos) = start;
	return false;
}

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
			
	struct parser pp = {
		.buf = {script, size}
	};
	struct parser* p = &pp;
	
	int err = setjmp(pp.onerr);
	if(err == 0) {
		size_t start = P(pos);
		while(consume_include(p));
		eat_space(p);
		// #include etc\n always adds an extra \n at the end
		if(preamble.len) {
			assert(preamble.base[preamble.len-1] == '\n');
			--preamble.len;
		}
		if(false == consume_statement(p, &closure_name)) {
			fail(p, NO_NAME, "no closure name specified");
		}
		// can't ever free lazy though... or can you? vvvvv
		eat_space(p);
		if(true == consume(p, "RETURNS:")) {
			eat_space(p);
			if(false == consume_statement(p, &return_type)) {
				fail(p, NO_RETURN, "RETURNS: without return type");
			}
		} else {
			return_type = LITSTR("void");
		}
		bool aux = false;
		for(;;) {
			eat_space(p);
			if(consume(p, "AUX:")) {
				aux = true;
			} else if(consume_var(p, aux)) {
				// ok
			} else if(P(buf.len) == P(pos)) {
				break;
			} else {
				fail(p, EXTRANEOUS, "extraneous characters");
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "uhhh %d\n", err);
		fprintf(stderr, "what '%.*s'\n",
				(int)(P(buf.len) - P(pos)), P(buf.base)+P(pos));
		abort();
	}
}
