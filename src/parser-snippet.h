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
bool output = true;

void onechar(void) {
	if(output) fputc(buf.base[pos],stdout);
	if(++pos == buf.len) longjmp(onerr, canexit);
}

auto void eat_space(void);

void eat_comment(void) {
	for(;;) {
		eat_space();
		if(advance("*/")) {
			output_string(LITSTR("*/"));
			break;
		} else {
			onechar();
		}
	}
}


void eat_space(void) {
	for(;;) {
		if(advance("CLOSURE")) {
			output_closure_name();
		} else if(advance("/*")) {
			output_string(LITSTR("/*"));
			eat_comment();
		} else if(advance("//")) {
			output_string(LITSTR("//"));
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
