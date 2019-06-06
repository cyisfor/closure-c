#include "script_info.h"
#include "output.h"
#include "mmapfile.h"

#include <unistd.h> // read
#include <stdlib.h> // realloc
#include <setjmp.h> // 
#include <assert.h> // 
#include <ctype.h> // isspace


struct var* init_vars = NULL;
struct var* vars = NULL;

string closure_name = {};
string return_type = {};

size_t nvars = 0;
size_t ninit_vars = 0;

void load_script_info(int fd) {
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

	string parse_line(void) {
		size_t start = pos;
		while(!expect("\n")) {
			onechar();
		}
		string s = {
			.base = buf.base + start,
			.len = pos - start - 1
		};
		return s;
	}

	bool in_init = false;

	bool expect_var(void) {
		size_t start_type = pos;
		if(!expect(";")) return false;
		size_t end_name = pos-1;
		size_t start_name = start_type;
		if(++start_name > buf.len) {
			longjmp(onerr, 5);
		}
		while(start_name < buf.len) {
			if(!isspace(buf.base[++start_name])) break;
		}
		size_t end_type = start_name - 1;
		while(end_type > start_type) {
			if(!isspace(buf.base[--end_type])) break;
		}
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
		if(in_init) {
			++ninit_vars;
			init_vars = reallocarray(init_vars,
									 ninit_vars,
									 sizeof(*init_vars));
			assert(init_vars);
			init_vars[ninit_vars-1] = v;
		} else {
			++nvars;
			vars = reallocarray(vars,
								nvars,
								sizeof(*vars));
			assert(vars);
			vars[nvars-1] = v;
		}
	}
	
	int err = setjmp(onerr);
	if(err == 0) {
		size_t start = pos;
		closure_name = parse_line();
		if(closure_name.len == 0) {
			longjmp(onerr, 3);
		}
		return_type = parse_line();
		if(return_type.len == 0) {
			return_type = LITSTR("void");
		}
		for(;;) {
			eat_space();
			if(expect("INIT:\n")) {
				in_init = true;
			} else if(expect("VARS:\n")) {
				in_init = false;
			} else if(expect_var()) {
			} else {
				onechar(); // XXX: ehhhhh
			}
		}
	} else if(err == 1) {
		// ok
	} else {
		fprintf(stderr, "uhhh %d\n", err);
		abort();
	}
}
