/*
 * Some tracing functionality.
 */

#ifndef TRACE_H
#define TRACE_H

#define STDLIB_STR_OFFSET 62

#define TRACE_TRUE                true

#define TRACE_BLOCK               true
#define TRACE_CANDIDATE           false
#define TRACE_DEFAULT_CONSTRUCTOR true
#define TRACE_GENERICS            false
#define TRACE_INSTANTIATION       false
#define TRACE_REF_TYPE            false
#define TRACE_RESOLVE             true
#define TRACE_RESOLVE_CALL        true
#define TRACE_RESOLVE_USE         false
#define TRACE_VERBOSE             false
#define TRACE_VISIBLE_FM          false

extern BaseAST* currentTraceContext;

void trace(bool tag, const char* msg, ...);
void trace_with_astloc(bool tag, BaseAST* node, const char* msg, ...);
void trace_enter(bool tag, BaseAST* node, const char* msg, ...);
void trace_flags(bool tag);
void trace_leave(bool tag, const char* msg = NULL, ...);
void trace_context(bool tag);
void trace_view(bool tag, BaseAST* node);
void trace_vcc(bool tag);
void trace_vcf(bool tag);
void trace_vpf(bool tag, Expr* node);

#endif
