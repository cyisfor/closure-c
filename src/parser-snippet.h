size_t pos = 0;

void fail(enum failure_state state, const char* fmt, ...) {
	fprintf(stderr, "buffer: ==============\n%.*s\n============\n",
			buf.len - pos, buf.base + pos);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
	raise(state);
}

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

bool seekf(string s) {
	size_t off = 0;
	while(buf.len >= s.len + pos + off) {
		if(0 == memcmp(s.base, buf.base + pos + off, s.len)) {
			pos += off;
			return true;
		}
		++off;
	}
	return false;
}
#define seek(lit) seekf(LITSTR(lit))

jmp_buf onerr;
int canexit = 1;
#ifdef OUTPUT
bool output = true;
#endif

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
