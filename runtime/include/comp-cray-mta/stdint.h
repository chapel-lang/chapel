#ifndef _MTA_STDINT_H_
#define _MTA_STDINT_H_

/* This defines the intN_t and uintN_t data types. */
#include <machine/types.h>

/* This defines many of the INT<N>_MIN/MAX macros we need */
#include <machine/limits.h>

/* Here are a few that it doesn't, defined in other constants that
   the MTA defines (noting that INT on MTA is 64-bits */
#define INT64_MIN INT_MIN
#define INT64_MAX INT_MAX
#define UINT64_MAX UINT_MAX

typedef unsigned long int   intptr_t;

#endif
