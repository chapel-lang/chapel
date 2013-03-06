/*
 * Some tracing functionality.
 */

#ifndef TRACE_H
#define TRACE_H

#define STDLIB_STR_OFFSET 62

#define TRACE_TRUE          true
#define TRACE_RESOLVE       true
#define TRACE_CANDIDATE     false
#define TRACE_GENERICS      true
#define TRACE_INSTANTIATION true
#define TRACE_VERBOSE       false

extern FnSymbol* currentTraceFn;

void trace(bool tag, const char* str, ...);
void trace_enter(bool tag, FnSymbol* fn, const char* msg);
void trace_flags(bool tag);
void trace_leave(bool tag, const char* str, ...);
void trace_vcf(bool tag);

#endif
