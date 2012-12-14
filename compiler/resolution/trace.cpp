#include "astutil.h"
#include "expr.h"
#include "symbol.h"
#include "vec.h"

#include "trace.h"

#define FULL_TRACE false

#define PRINT_INDENT for (int i = traceIndentLevel; i-- > 0;) printf("\t");

static unsigned int traceIndentLevel  = 0;
static bool         tracingTargetFn   = false;
static const char*  targetFnName      = "readThis";

static Vec<FnSymbol*> traceStack;

FnSymbol* currentTraceFn = NULL;

void trace(bool tag, const char* str, ...) {
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTargetFn) {
    
    PRINT_INDENT;
    
    va_start(fmt_args, str);
    vprintf(str, fmt_args);
    printf("\n");
    va_end(fmt_args);
  }
}

void trace_enter(bool tag, FnSymbol* fn, const char* msg) {
  const char* fileName;
  
  if (not tag) return;
  
  if (FULL_TRACE or
      tracingTargetFn or
      (tracingTargetFn = (strcmp(fn->name, targetFnName) == 0))) {
    
    if (currentTraceFn != NULL) traceStack.add(currentTraceFn);
    currentTraceFn = fn;
    
    fileName = fn->astloc.filename == NULL ? NULL : fn->astloc.filename + STDLIB_STR_OFFSET;
    
    PRINT_INDENT;
    printf("%s: %s (%s : %d)\n", msg, fn->name, fileName, fn->astloc.lineno);
    
    ++traceIndentLevel;
  }
}

void trace_leave(bool tag, const char* str, ...) {
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTargetFn) {
    if (str != NULL) {
      PRINT_INDENT;
      
      va_start(fmt_args, str);
      vprintf(str, fmt_args);
      printf("\n\n");
      va_end(fmt_args);
    }
    
    --traceIndentLevel;
    
    tracingTargetFn = not (tracingTargetFn and traceIndentLevel == 0 and strcmp(currentTraceFn->name, targetFnName) == 0);
    currentTraceFn  = traceStack.pop();
  }
}
