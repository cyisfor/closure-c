#include "mystring.h"
#include "for_types.h"

extern struct var* init_vars;

// note: init vars could be initialized first, set to null, set later, etc
// but these vars have to be set at the time of the closure call:
extern struct var* vars;

extern string closure_name;
extern string return_type;

void script_info_load(int fd);
