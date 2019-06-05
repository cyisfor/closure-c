#include "mmapfile.h"
#include "grammar.h"

int main(int argc, char *argv[])
{
	size_t size;
	char* mem = mmapfd(0, &size);
	if(mem == NULL) return 1;
	parse((string){mem, size});
    return 0;
}
