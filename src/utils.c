//
// Created by sheverdin on 4/18/25.
//
#include <stdio.h>
#include "utils.h"

void print_usage(const char *prog_name)
{
    fprintf(stderr, "Usage: %s [-n|-g|-s] OID\n", prog_name);
}
