#include <stdio.h>
#include <string.h>
#include "types.h"
#include "strings.h"
#include "util.h"

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("stars CLI %s\n", SzVersion());

    printf("PszGetCompressedString(0): %s\n", PszGetCompressedString(0));

    OutputSz(0, "test");
    return 0;
}
