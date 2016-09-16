#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "common_rcrtool.h"
#include "qt_rcrtool.h"
#include "bcGen.h"
#include "blackboard.h"

typedef struct _parallelRegion{
    void (*func) (void *);
    const char* funcName;
    unsigned int entryNum;
} parallelRegion;

#define LATENCYTEST 10000

#define AMD_OPTERON

/*!
 * Complain with perror, then exit.
 * 
 * \param msg Nature of the problem (don't include newline)
 */
void die(char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

