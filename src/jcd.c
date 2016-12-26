#include <stdio.h>

#include "jcd.h"

int main (int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
    }
    exit(argc-i);
}
