#include <stdarg.h> // va_*
#include <ctype.h> // isspace

#define P(arg) (p->arg)

static
void failf(struct parser* p, const char* file, int line, enum failure_state state, const char* fmt, ...) {
	fprintf(stderr, "%s:%d\n",file,line);
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
#define fail(p, state, fmt, ...) \
	failf((struct parser*)p, __FILE__, __LINE__, state, fmt, ## __VA_ARGS__)

static
bool consumef(struct parser* p, string s) {
	size_t left = P(buf.len) - P(pos);
	if(left < s.len) return false;
	if(0 == memcmp(s.base, P(buf.base) + P(pos), s.len)) {
		P(pos) += s.len;
		return true;
	}
	return false;
}
#define consume(p, lit) consumef((struct parser*)p, LITSTR(lit))
static
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
#define seek(p, lit) seekf((struct parser*)p, LITSTR(lit))

static
bool onecharf(struct parser* p) {
	if(P(pos) == P(buf.len)) return false;
#ifdef OUTPUT
	if(P(output)) fputc(P(buf.base)[P(pos)],stdout);
#endif
	++P(pos);
	return true;
}

#define onechar(p) onecharf((struct parser*)p)

// XXX: uh oh...
// this is for stuff that gets parsed everywhere, even in comments and "whitespace"
static
bool consume_universal_stuff(struct parser* p);

static
void eat_spacef(struct parser* p);
#define eat_space(p) eat_spacef((struct parser*)p)

static
void eat_comment(struct parser* p) {
	for(;;) {
		eat_space(p);
		if(p->pos == p->buf.len) return;
		if(consume(p, "*/")) {
#ifdef OUTPUT
			if(P(output)) {
				output_string(LITSTR("*/"));
			}
#endif
			break;
		} else if(consume_universal_stuff(p)) {
		} else if(!onechar(p)) {
			break;
		}
	}
}

static
void eat_spacef(struct parser* p) {
	for(;;) {
		if(consume_universal_stuff(p)) {
		} else if(consume(p, "/*")) {
#ifdef OUTPUT
			if(P(output)) {
				output_string(LITSTR("/*"));
			}
#endif
			eat_comment(p);
		} else if(consume(p, "//")) {
#ifdef OUTPUT
			if(P(output)) {
				output_string(LITSTR("//"));
			}
#endif
			for(;;) {
				if(!onechar(p)) break;
				if(P(buf.base)[P(pos)] == '\n') {
					onechar(p);
					break;
				}
			}
		} else if(isspace(P(buf.base)[P(pos)])) {
			onechar(p);
		} else {
			return;
		}
	}
}

#ifdef OUTPUT
#undef OUTPUT
#endif
