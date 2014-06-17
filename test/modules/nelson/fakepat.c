
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "fakepat.h"

int PAT_region_begin(int id, const char* lbl) {
  return printf("Fake PAT opening region %d: %s\n", id, lbl);
}

int PAT_region_end(int id) {
  return printf("Fake PAT closing region %d\n", id);
}

int PAT_record(int state) {
  return printf("Fake PAT setting profiling state to %d\n", state);
}

int PAT_trace_user (const char * msg) {
  return printf("Fake PAT user trace string \"%s\".\n", msg);
}

int PAT_trace_user_l (const char * msg, int enable, ...) {
  va_list a;
  int i;
  printf("Fake PAT user trace string \"%s\". ", msg);
  va_start(a, enable);
  
  for (i=0; i < 16; i++) {
    
    int64_t v = (int64_t) va_arg(a, int64_t);
    //printf("%ld ", v); // for simplicity
  }
  va_end(a);
  return 0;
}
