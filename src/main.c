#include "serialized_interface.h"
#include "mmapfile.h"
#include "grammar.h"

int main(int argc, char *argv[])
{
	parse((string){INTERFACE, INTERFACE_SIZE});
    return 0;
}
