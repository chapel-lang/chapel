#ifndef QT_OUTPUT_MACROS_H
#define QT_OUTPUT_MACROS_H

#include <stdio.h>

#define print_status(format, ...)  printf("QTHREADS: " format, ## __VA_ARGS__)
#define print_warning(format, ...) fprintf(stderr, "QTHREADS: " format, ## __VA_ARGS__)
#define print_error(format, ...)   do { fprintf(stderr, "QTHREADS ERROR: " format, ## __VA_ARGS__); \
                                        fflush(stderr); } while (0)

#endif
/* vim:set expandtab: */
