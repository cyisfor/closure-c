size_t pos = 0;

bool consumef(string s) {
	size_t left = buf.len - pos;
	if(left < s.len) return false;
	if(0 == memcmp(s.base, buf.base + pos, s.len)) {
		pos += s.len;
		return true;
	}
	return false;
}
#define consume(lit) consumef(LITSTR(lit))

jmp_buf onerr;
int canexit = 1;
#ifdef OUTPUT
bool output = true;
#endif

void seekf(string s) {
	while(buf.len - pos > s.len) {
		if(0 == memcmp(s.base, buf.base + pos, s.len)) {
			return;
		}
		assert(++pos != buf.len);
	}
	longjmp(onerr, canexit);
}
#define seek(lit) seekf(LITSTR(lit))


void onechar(void) {
#ifdef OUTPUT
	if(output) fputc(buf.base[pos],stdout);
#endif
	if(++pos == buf.len) longjmp(onerr, canexit);
}

auto void eat_space(void);

void eat_comment(void) {
	for(;;) {
		eat_space();
		if(consume("*/")) {
#ifdef OUTPUT
			output_string(LITSTR("*/"));
#endif
			break;
		} else {
			onechar();
		}
	}
}


void eat_space(void) {
	for(;;) {
		if(consume("CLOSURE")) {
			/* XXX: why put this here? where to put it? */
#ifdef OUTPUT
			output_closure_name();
#endif
		} else if(consume("/*")) {
#ifdef OUTPUT			
			output_string(LITSTR("/*"));
#endif
			eat_comment();
		} else if(consume("//")) {
#ifdef OUTPUT
			output_string(LITSTR("//"));
#endif
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

#ifdef OUTPUT
#undef OUTPUT
#endif
