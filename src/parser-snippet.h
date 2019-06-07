struct parser_c
size_t pos = 0;

jmp_buf onerr;

void fail(struct parser* p, enum failure_state state, const char* fmt, ...) {
	fprintf(stderr, "buffer: ==============\n%.*s\n============\n",
			(int)(P(buf.len) - P(pos)), P(buf.base) + P(pos));
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
	exit(state); // XXX: eh
	longjmp(P(onerr), state);
}

bool consumef(struct parser* p, string s) {
	size_t left = P(buf.len) - P(pos);
	if(left < s.len) return false;
	if(0 == memcmp(s.base, P(buf.base) + P(pos), s.len)) {
		P(pos) += s.len;
		return true;
	}
	return false;
}
#define consume(lit) consumef(p, LITSTR(lit))

bool seekf(struct parser* p, string s) {
	size_t off = 0;
	while(P(buf.len) >= s.len + P(pos) + off) {
		if(0 == memcmp(s.base, P(buf.base) + P(pos) + off, s.len)) {
			P(pos) += off;
			return true;
		}
		++off;
	}
	return false;
}
#define seek(lit) seekf(p, LITSTR(lit))

int canexit = 1;
#ifdef OUTPUT
bool output = true;
#endif

void onechar(struct parser* p) {
#ifdef OUTPUT
	if(output) fputc(P(buf.base)[P(pos)],stdout);
#endif
	if(++P(pos) == P(buf.len)) longjmp(onerr, canexit);
}

// XXX: uh oh...
static bool consume_universal_stuff(struct parser* p);

static void eat_space(void);

void eat_comment(void) {
	for(;;) {
		eat_space();
		if(consume("*/")) {
#ifdef OUTPUT
			output_string(LITSTR("*/"));
#endif
			break;
		} else if(consume_universal_stuff()) {
		} else {
			onechar();
		}
	}
}


void eat_space(void) {
	for(;;) {
		if(consume_universal_stuff()) {
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
				if(P(buf.base)[P(pos)] == '\n') {
					onechar();
					break;
				}
			}
		} else if(isspace(P(buf.base)[P(pos)])) {
			onechar();
		} else {
			return;
		}
	}
}

#ifdef OUTPUT
#undef OUTPUT
#endif
