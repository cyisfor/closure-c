#include "mmapfd.h"
#include <unistd.h> // read
#include <stdlib.h> // realloc

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

}
