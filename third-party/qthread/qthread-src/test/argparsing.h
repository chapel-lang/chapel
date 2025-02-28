#ifndef TEST_ARGPARSING_H
#define TEST_ARGPARSING_H

#if defined(__cplusplus) && __cplusplus < 202302L
#include <atomic>
#define ARGP_Atomic(T) std::atomic<T>
using std::atomic_load_explicit;
using std::atomic_store_explicit;
using std::memory_order_relaxed;
#else
#include <stdatomic.h>
#define ARGP_Atomic(T) _Atomic(T)
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for strncmp() */

#define CHECK_VERBOSE()                                                        \
  do {                                                                         \
    const char *v = getenv("VERBOSE");                                         \
    if (v == NULL) {                                                           \
      atomic_store_explicit(                                                   \
        &verbose,                                                              \
        (int)!(getenv("MAKEFLAGS") && getenv("MAKELEVEL")),                    \
        memory_order_relaxed);                                                 \
    } else {                                                                   \
      atomic_store_explicit(                                                   \
        &verbose, (int)strncmp(v, "0", 2), memory_order_relaxed);              \
    }                                                                          \
  } while (0)

#define NUMARG(var, name)                                                      \
  do {                                                                         \
    char *str;                                                                 \
    if ((str = getenv(name)) != NULL) {                                        \
      char *stre = NULL;                                                       \
      size_t tmp = strtoul(str, &stre, 0);                                     \
      if (stre == NULL || stre == str) {                                       \
        fprintf(stderr, "unparsable " name " (%s)\n", str);                    \
      } else {                                                                 \
        var = tmp;                                                             \
      }                                                                        \
    }                                                                          \
    iprintf(name " = %lu\n", (unsigned long)var);                              \
  } while (0)

#define DBLARG(var, name)                                                      \
  do {                                                                         \
    char *str;                                                                 \
    if ((str = getenv(name)) != NULL) {                                        \
      char *stre = NULL;                                                       \
      double tmp = strtod(str, &stre);                                         \
      if (stre == NULL || stre == str) {                                       \
        fprintf(stderr, "unparsable " name " (%s)\n", str);                    \
      } else {                                                                 \
        var = tmp;                                                             \
      }                                                                        \
    }                                                                          \
    iprintf(name " = %f\n", (double)var);                                      \
  } while (0)

// Given an environment variable "FOO=,,,80,,"
// And buffer "size_t foo[5]"
// Then NUMARRARG(foo, bar, 5, 100)
//   => foo = {100,100,100,80,100}
#define NUMARRARG(var, name, size, val)                                        \
  do {                                                                         \
    char *str = getenv(name);                                                  \
    int i = 0;                                                                 \
    if (str != NULL && *str != '\0') {                                         \
      char *rest = NULL;                                                       \
      do {                                                                     \
        var[i] = strtoul(str, &rest, 0);                                       \
        if (rest == str) { var[i] = val; }                                     \
        if (*rest == ',') {                                                    \
          rest++;                                                              \
        } else if (*rest != '\0') {                                            \
          fprintf(stderr, "malformed " name " ending (%s)\n", rest);           \
          abort();                                                             \
        }                                                                      \
        i++;                                                                   \
        str = rest;                                                            \
      } while (*rest != '\0' && i < size);                                     \
      if (*rest != '\0') {                                                     \
        fprintf(stderr, "extra values in " name " (%s)\n", rest);              \
      }                                                                        \
    }                                                                          \
    while (i < size) {                                                         \
      var[i] = val;                                                            \
      i++;                                                                     \
    }                                                                          \
    iprintf(name " = [%lu", (unsigned long)var[0]);                            \
    for (int i = 1; i < size; i++) iprintf(", %lu", (unsigned long)var[i]);    \
    iprintf("]\n");                                                            \
  } while (0)

static ARGP_Atomic(int) verbose;

#if defined(SILENT_ARGPARSING)
#warning Silencing iprintf() output.
#define iprintf(...)
#else
static void iprintf(char const *restrict format, ...) {
  if (atomic_load_explicit(&verbose, memory_order_relaxed)) {
    va_list ap;

    va_start(ap, format);
    vprintf(format, ap);
    fflush(stdout);
    va_end(ap);
  }
}

#endif // if defined(__CYGWIN32__)

#endif // ifndef TEST_ARGPARSING_H
/* vim:set expandtab: */
