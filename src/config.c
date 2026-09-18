//Code for handling configuration information

#include "config.h"
#include <stdio.h>

int write_config(FILE* fp) {
    fputs("[Test]", fp);
    return 0;
}

