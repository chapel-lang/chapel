/* 
 * myth_wrap_util_func.h
 */
#pragma once
#ifndef MYTH_WRAP_UTIL_FUNC_H
#define MYTH_WRAP_UTIL_FUNC_H
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "myth_config.h"

static int trace_wrapped_func(void) {
  static int s_trace_wrapped_func = -1;
  if (s_trace_wrapped_func == -1) {
    const char * s = getenv("MYTH_TRACE_WRAPPED_FUNC");
    if (s && atoi(s)) {
      s_trace_wrapped_func = 1;
    } else {
      s_trace_wrapped_func = 0;
    }
  }
  return s_trace_wrapped_func;
}

static int enter_wrapped_func_(const char * func, ...) {
  if (trace_wrapped_func()) {
    va_list ap;
    va_start(ap, func);
    const char * fmt = va_arg(ap, const char *);
    fprintf(stderr, "enter %s(", func);
    if (fmt) {
      vfprintf(stderr, fmt, ap);
    }
    fprintf(stderr, ") ");
    va_end(ap);
  }
  return 0;
}

static int leave_wrapped_func_(const char * func, ...) {
  if (trace_wrapped_func()) {
    va_list ap;
    va_start(ap, func);
    const char * fmt = va_arg(ap, const char *);
    if (fmt) {
      fprintf(stderr, "--> ");
      vfprintf(stderr, fmt, ap);
    }
    fprintf(stderr, "\n");
    va_end(ap);
  }
  return 0;
}

#define enter_wrapped_func(...) enter_wrapped_func_(__func__, __VA_ARGS__)
#define leave_wrapped_func(...) leave_wrapped_func_(__func__, __VA_ARGS__)

static void __attribute__((unused)) sorry_unimplemented_(char * function) {
  fprintf(stderr,
	  "sorry, myth_wrap_pthread does not implement function %s. abort\n",
	  function);
  abort();
}

#define sorry_unimplemented() sorry_unimplemented_(__func__)

#if MYTH_WRAP == MYTH_WRAP_LD
#define __wrap(x) __wrap_ ## x
//#define __real(x) __real_ ## x
#elif MYTH_WRAP == MYTH_WRAP_DL
#define __wrap(x) x
//#define __real(x) x
#else
#error MYTH_WRAP must be set to MYTH_WRAP_LD or MYTH_WRAP_DL
#endif

#endif	/* MYTH_WRAP_UTIL_FUNC_H */
