#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>

#define HANDLE_ERROR(err) \
    do { perror(err); exit(EXIT_FAILURE); } while (0)

#endif