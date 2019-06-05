#include "mystring.h"
#include <stdbool.h>

struct var {
	string type;
	string name;
};

struct var* for_types(bool do_init, size_t* num);
