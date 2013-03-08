#include "astutil.h"
#include "expr.h"
#include "symbol.h"
#include "vec.h"
#include "view.h"

#include "trace.h"

#define FULL_TRACE true

#define PRINT_INDENT for (int i = traceIndentLevel; --i >= 0;) printf("\t");

static unsigned int traceIndentLevel    = 0;
static bool         tracingTarget       = false;
static const char*  targetContextName   = "==";

static Vec<Symbol*> traceStack;

Symbol* currentTraceContext = NULL;

void trace(bool tag, const char* str, ...) {
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTarget) {
    
    PRINT_INDENT;
    
    va_start(fmt_args, str);
    vprintf(str, fmt_args);
    printf("\n");
    va_end(fmt_args);
    
    fflush(stdout);
  }
}

void trace_enter(bool tag, Symbol* sym, const char* msg, ...) {
  const char* fileName;
  
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or
      tracingTarget or
      (tracingTarget = (strcmp(sym->name, targetContextName) == 0))) {
    
    if (currentTraceContext != NULL) traceStack.add(currentTraceContext);
    currentTraceContext = sym;
    
    fileName = sym->astloc.filename == NULL ? NULL : sym->astloc.filename + STDLIB_STR_OFFSET;
    
    if (msg != NULL) {
      PRINT_INDENT;
      
      va_start(fmt_args, msg);
      vprintf(msg, fmt_args);
      printf(": %s (%s : %d)\n", sym->name, fileName, sym->astloc.lineno);
      va_end(fmt_args);
      
      fflush(stdout);
    }
    
    ++traceIndentLevel;
  }
}

void trace_flags(bool tag) {
  if (tag and (FULL_TRACE or tracingTarget)) {
    viewFlags(currentTraceContext);
    
    fflush(stdout);
  }
}

void trace_leave(bool tag, const char* msg, ...) {
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTarget) {
    if (msg != NULL) {
      PRINT_INDENT;
      
      va_start(fmt_args, msg);
      vprintf(msg, fmt_args);
      printf("\n\n");
      va_end(fmt_args);
      
      fflush(stdout);
    }
    
    --traceIndentLevel;
    
    tracingTarget        = not (tracingTarget and traceIndentLevel == 0 and strcmp(currentTraceContext->name, targetContextName) == 0);
    currentTraceContext  = traceStack.pop();
  }
}

void trace_vcf(bool tag) {
  if (tag and (FULL_TRACE or tracingTarget)) {
    print_view(currentTraceContext);
    
    fflush(stdout);
  }
}
