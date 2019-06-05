#ifndef FOR_TYPES_H
#define FOR_TYPES_H

#include "mystring.h"
#include <stdbool.h>

struct var {
	string type;
	string name;
};

const struct var* for_types(bool do_init, size_t* num);

#endif /* FOR_TYPES_H */
