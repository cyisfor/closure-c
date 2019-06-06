#include <unistd.h>

int main(int argc, char *argv[])
{
    execlp("diff", "diff", "example2.h", "example_closure.h", NULL);
    return 0;
}
