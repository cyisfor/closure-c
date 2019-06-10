#include "concatsym.h"
#ifndef NAMESPACE
#error define a namespace!
#endif

#ifdef T
#pragma push_macro("T")
#pragma push_macro("T_")
#pragma push_macro("N")
#pragma push_macro("NAMESPACE")
#endif

#define T NAMESPACE
#define T_ CONCATSYM(T,_)
#define N(x) CONCATSYM(T_, x)
