#include "astutil.h"
#include "baseAST.h"
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

static Vec<BaseAST*> traceStack;

BaseAST* currentTraceContext = NULL;

void trace(bool tag, const char* msg, ...) {
  va_list fmt_args;
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTarget) {
    
    PRINT_INDENT;
    
    va_start(fmt_args, msg);
    vprintf(msg, fmt_args);
    printf("\n");
    va_end(fmt_args);
    
    fflush(stdout);
  }
}

void trace_with_astloc(bool tag, BaseAST* node, const char* msg, ...) {
  const char* fileName;
  
  va_list fmt_args;
  
  Symbol* sym = toSymbol(node);
  
  if (not tag) return;
  
  if (FULL_TRACE or tracingTarget) {
    
    fileName = node->astloc.filename == NULL ? NULL : node->astloc.filename + STDLIB_STR_OFFSET;
    
    PRINT_INDENT;
    
    va_start(fmt_args, msg);
    vprintf(msg, fmt_args);
    printf(" : %s (%s : %d)\n", sym ? sym->name : "expression", fileName, node->astloc.lineno);
    va_end(fmt_args);
    
    fflush(stdout);
  }
}

void trace_enter(bool tag, BaseAST* node, const char* msg, ...) {
  const char* fileName;
  
  va_list fmt_args;
  
  Symbol* sym = toSymbol(node);
  
  if (not tag) return;
  
  if (FULL_TRACE or
      tracingTarget or
      (sym != NULL and (tracingTarget = (strcmp(sym->name, targetContextName) == 0)))) {
    
    if (currentTraceContext != NULL) traceStack.add(currentTraceContext);
    currentTraceContext = node;
    
    fileName = node->astloc.filename == NULL ? NULL : node->astloc.filename + STDLIB_STR_OFFSET;
    
    if (msg != NULL) {
      PRINT_INDENT;
      
      va_start(fmt_args, msg);
      vprintf(msg, fmt_args);
      printf(" : %s (%s : %d) (%p)\n", sym ? sym->name : "expression", fileName, node->astloc.lineno, currentTraceContext);
      va_end(fmt_args);
    }
    
    if (TRACE_VERBOSE) {
      if (isSymbol(node)) viewFlags(node);
      print_view(node);
    }
    
    fflush(stdout);
    
    ++traceIndentLevel;
  }
}

void trace_flags(bool tag) {
  if (tag and (FULL_TRACE or tracingTarget) and isSymbol(currentTraceContext)) {
    viewFlags(currentTraceContext);
    
    fflush(stdout);
  }
}

void trace_leave(bool tag, const char* msg, ...) {
  const char* fileName;
  
  va_list fmt_args;
  
  Symbol* sym = toSymbol(currentTraceContext);
  
  if (tag and (FULL_TRACE or tracingTarget)) {
    
    fileName = currentTraceContext->astloc.filename == NULL ? NULL : currentTraceContext->astloc.filename + STDLIB_STR_OFFSET;
    
    if (msg != NULL) {
      PRINT_INDENT;
      
      va_start(fmt_args, msg);
      vprintf(msg, fmt_args);
      printf(" : %s (%s : %d)\n", sym ? sym->name : "", fileName, currentTraceContext->astloc.lineno);
      va_end(fmt_args);
      
      
    } else {
      PRINT_INDENT;
      
      printf("Poping context : %s (%s : %d)\n", sym ? sym->name : "", fileName, currentTraceContext->astloc.lineno);
    }
    
    --traceIndentLevel;
    
    tracingTarget        = not (tracingTarget and traceIndentLevel == 0 and sym != NULL and strcmp(sym->name, targetContextName) == 0);
    currentTraceContext  = traceStack.pop();
    
    if (currentTraceContext != NULL) {
      sym      = toSymbol(currentTraceContext);
      fileName = currentTraceContext->astloc.filename == NULL ? NULL : currentTraceContext->astloc.filename + STDLIB_STR_OFFSET;
      
      PRINT_INDENT;
      
      printf("Returning to context : %s (%s : %d) (%p)\n\n", sym ? sym->name : "",
        fileName, currentTraceContext->astloc.lineno, currentTraceContext);
    }
    
    fflush(stdout);
  }
}

void trace_context(bool tag) {
  int j = 0;
  
  if (tag) {
    PRINT_INDENT;
    printf("Begin full context dump.\n\n");
    
    forv_Vec(BaseAST*, node, traceStack) {
      printf("Context %d:\n", j++);
      print_view(node);
      printf("\n");
    }
    
    PRINT_INDENT;
    printf("End full context dump.\n");
  }
}

void trace_view(bool tag, BaseAST* node) {
  if (tag and (FULL_TRACE or tracingTarget)) {
    print_view(node);
    
    fflush(stdout);
  }
}

void trace_vcc(bool tag) {
  if (tag and (FULL_TRACE or tracingTarget)) {
    print_view(currentTraceContext);
    
    fflush(stdout);
  }
}

void trace_vcf(bool tag) {
  if (tag and (FULL_TRACE or tracingTarget)) {
    if (isFnSymbol(currentTraceContext)) {
      print_view(currentTraceContext);
      
    } else {
      BaseAST* node;
      
      for (int tsIndex = traceStack.n; --tsIndex >= 0;) {
        node = traceStack.v[tsIndex];
        
        if (isFnSymbol(node)) {
          print_view(node);
          break;
        }
      }
    }
    
    fflush(stdout);
  }
}

void trace_vpf(bool tag, Expr* node) {
  if (tag) {
    print_view(node->parentSymbol);
    
    fflush(stdout);
  }
}
