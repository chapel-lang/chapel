/* Test program for gasnet_portable_platform.h */

#ifdef CONFIG_HEADER
#include CONFIG_HEADER
#endif

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif

#ifdef __cplusplus
#define LANGSLUG CXX
#else
#define LANGSLUG C
#endif

typedef struct {
  const char *key;
  const char *valstr;
  const int valint;
} entry_t;
typedef entry_t *snap_t;

#define CAPTURE_FULL(key, valstr, valint) { key, valstr, (int)(valint) }
#define CAPTURE_TOK(var) CAPTURE_FULL("PLATFORM_" #var, PLATFORM_STRINGIFY(PLATFORM_##var), 0)
#define CAPTURE_STR(var) CAPTURE_FULL("PLATFORM_" #var, "\"" PLATFORM_##var "\"", 0)
#define CAPTURE_INT(var) CAPTURE_FULL("PLATFORM_" #var, 0, PLATFORM_##var)
#define CAPTURE_DEF(var) CAPTURE_FULL("IFDEF_PLATFORM_" #var, PLATFORM_STRINGIFY(PLATFORM_##var), 0)

#define CAPTURE_SNAP(id)                  \
  entry_t snap##id[] = {                  \
    CAPTURE_INT(HEADER_VERSION),          \
    \
    CAPTURE_TOK(COMPILER_FAMILYNAME),     \
    CAPTURE_INT(COMPILER_FAMILYID),       \
    CAPTURE_INT(COMPILER_ID),             \
    CAPTURE_INT(COMPILER_VERSION),        \
    CAPTURE_FULL("PLATFORM_COMPILER_" PLATFORM_STRINGIFY(LANGSLUG) "_LANGLVL",               \
                             0,  _CONCAT(_CONCAT(PLATFORM_COMPILER_,LANGSLUG),_LANGLVL)),    \
    CAPTURE_STR(COMPILER_VERSION_STR),    \
    CAPTURE_STR(COMPILER_IDSTR),          \
    CAPTURE_FULL("PLATFORM_COMPILER_" PLATFORM_STRINGIFY(PLATFORM_COMPILER_FAMILYNAME),      \
                          0,  _CONCAT(PLATFORM_COMPILER_,PLATFORM_COMPILER_FAMILYNAME)),     \
    CAPTURE_FULL("PLATFORM_COMPILER_" PLATFORM_STRINGIFY(PLATFORM_COMPILER_FAMILYNAME)       \
                                      "_" PLATFORM_STRINGIFY(LANGSLUG),                      \
                  0,  _CONCAT(PLATFORM_COMPILER_,                                            \
                              _CONCAT(_CONCAT(PLATFORM_COMPILER_FAMILYNAME, _), LANGSLUG))), \
    \
    CAPTURE_TOK(OS_FAMILYNAME),                                               \
    CAPTURE_FULL("PLATFORM_OS_" PLATFORM_STRINGIFY(PLATFORM_OS_FAMILYNAME),   \
                           0,  _CONCAT(PLATFORM_OS_,PLATFORM_OS_FAMILYNAME)), \
    \
    CAPTURE_TOK(ARCH_FAMILYNAME),                                                 \
    CAPTURE_FULL("PLATFORM_ARCH_" PLATFORM_STRINGIFY(PLATFORM_ARCH_FAMILYNAME),   \
                           0,  _CONCAT(PLATFORM_ARCH_,PLATFORM_ARCH_FAMILYNAME)), \
    CAPTURE_DEF(ARCH_32),                                                         \
    CAPTURE_DEF(ARCH_64),                                                         \
    CAPTURE_DEF(ARCH_LITTLE_ENDIAN),                                              \
    CAPTURE_DEF(ARCH_BIG_ENDIAN),                                                 \
    \
    }

#include <gasnet_portable_platform.h>

CAPTURE_SNAP(0);

#include <gasnet_portable_platform.h> /* test double-include */

#ifdef INJECT_ERROR /* for testing this file */
#undef  PLATFORM_COMPILER_ID
#define PLATFORM_COMPILER_ID 999
#undef  PLATFORM_COMPILER_IDSTR
#define PLATFORM_COMPILER_IDSTR "junk"
#undef  PLATFORM_ARCH_32
#undef  PLATFORM_ARCH_64
#endif

CAPTURE_SNAP(1);

/* test for system header interference
 * use the C89 headers that should be safe everywhere
 */
#ifndef SKIP_SYS_HEADERS
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif

#ifdef USE_MPI
#include <mpi.h>
#endif

#define PLATFORM_SHOW 1 /* enable self-test */
#define main platform_show
#undef _PORTABLE_PLATFORM_H /* force a re-include */
#include <gasnet_portable_platform.h>

CAPTURE_SNAP(2);

#include <assert.h>

#undef main
int errs = 0;
int main() {

  /* first do the header self-test */
  printf("PLATFORM_SHOW:\n-------------\n");
  platform_show(); 
  printf("\n"); 
 
 { /* now look at our own snapshots of output */
  size_t i,k;
  snap_t snaps[] = { snap0, snap1, snap2 };
  size_t numsnaps = sizeof(snaps)/sizeof(snap_t);
  size_t numkeys = sizeof(snap1)/sizeof(entry_t);
  for (i=1; i<numsnaps; i++) {
    snap_t snap = snaps[i];
    for (k=0; k < numkeys; k++) {
       entry_t *e0 = &snap0[k];
       entry_t *ei = &snap[k];
       errs++;
       if (strcmp(ei->key, e0->key)) {
         printf("ERROR: key %i mismatch in snap %i: %s != %s\n", (int)k, (int)i, ei->key, e0->key);
       } else if (!ei->valstr != !e0->valstr) {
         printf("ERROR: %s type mismatch in snap %i\n", ei->key, (int)i);
       } else if (ei->valint != e0->valint) {
         printf("ERROR: %s mismatch in snap %i: %i != %i\n", ei->key, (int)i, ei->valint, e0->valint);
       } else if (ei->valstr && strcmp(ei->valstr, e0->valstr)) {
         printf("ERROR: %s mismatch in snap %i: %s != %s\n", ei->key, (int)i, ei->valstr, e0->valstr);
       } else errs--;
    }
  }
  for (i=0; i<numsnaps; i++) {
    snap_t snap = snaps[i];
    printf("Snap %i:\n-------\n",(int)i);
    for (k=0; k < numkeys; k++) {
       entry_t *e = &snap[k];
       if (e->valstr) {
         if (!strncmp(e->key,"IFDEF_",6)) {
           printf("%-30s: %s\n", e->key+6, (strcmp(e->key+6,e->valstr)?e->valstr:"<undef>"));
         } else {
           printf("%-30s: %s\n", e->key, e->valstr);
         }
       } else {
         printf("%-30s: (%i)\n", e->key, e->valint);
       }
    }
    if (!errs) break;
  }
 }
  return errs;
}
