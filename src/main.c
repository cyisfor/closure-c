#include "template.h"
#include MY_INFO
#include "mmapfile.h"
#include "parse.h"

int main(int argc, char *argv[])
{
#ifdef SCRIPT_INFO
	script_info_load(0);
#endif
	parse((string){TEMPLATE, TEMPLATE_length});
    return 0;
}
