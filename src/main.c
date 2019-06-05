#include "template.h"
#include "mmapfile.h"
#include "grammar.h"

int main(int argc, char *argv[])
{
#ifdef SCRIPT_INFO
	load_script_info(0);
#endif
	parse((string){TEMPLATE, TEMPLATE_length});
    return 0;
}
