#include "../test_framework/runner.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    const char *filter = NULL;

    if (argc > 1)
        filter = argv[1];

    run(filter);

    return 0;
}
