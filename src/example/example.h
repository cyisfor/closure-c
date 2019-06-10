#define NAMESPACE example
#include "../namespace.h"

#define FOR_TYPES X(struct thing1, foo) X(double,bar) X(void*,baz)
#include "../closure.h"
#include "../end_namespace.h"
