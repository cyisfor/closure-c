#include "mystring.h"
#include "for_types.h"

extern struct var* aux_vars;

// note: aux vars could be auxialized first, set to null, set later, etc
// but these vars have to be set at the time of the closure call:
extern struct var* vars;

extern size_t nvars;
extern size_t naux_vars;

extern bstring preamble;

extern string closure_name;
extern string safe_closure_name;
extern string return_type;

void script_info_load(int fd);
#define SCRIPT_INFO // XXX some weird cpp thing eh...
