#ifndef _PASSES_H
#define _PASSES_H_

extern bool normalized;

void check_parsed(void);
void check_normalized(void);
void check_resolved(void);
void cleanup(void);
void copy_propagation(void);
void normalize(void);
void resolve_analyzed(void);
void scopeResolve(void);

#endif
