#ifndef TEST_ARGPARSING_H
#define TEST_ARGPARSING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef SST
# define CHECK_VERBOSE() verbose = (getenv("VERBOSE") != NULL)
#else
# define CHECK_VERBOSE()
#endif

#define NUMARG(var,name) do { \
    char *str; \
    if ((str = getenv(name)) != NULL) { \
	char *stre = NULL; \
	size_t tmp = strtoul(str, &stre, 0); \
	if (stre == NULL || stre == str) { \
	    fprintf(stderr, "unparsable "name" (%s)\n", str); \
	} else { \
	    var = tmp; \
	} \
    } \
    iprintf(name" = %lu\n", (unsigned long)var); \
} while (0)

#ifdef SST
static int verbose = 1;
#else
static int verbose = 0;
#endif

#if defined(__tile__) || defined(__CYGWIN32__)
# define iprintf printf
#else
static void iprintf(const char * restrict format, ...)
{
    if (verbose != 0) {
	va_list ap;

	va_start(ap, format);
	vprintf(format, ap);
	fflush(stdout);
	va_end(ap);
    }
}
#endif

#endif
