#ifndef _VIEW_H_
#define _VIEW_H_

#include "baseAST.h"
#include "chplenum.h"

extern BaseAST* ast(long id);
extern void print_view(BaseAST* ast);
extern void print_view_noline(BaseAST* ast);
extern void iprint_view(long id);
extern void nprint_view(BaseAST* ast);
extern void nprint_view_noline(BaseAST* ast);
extern void mark_view(BaseAST* ast, long id);

extern void print_symtab(BaseAST* ast);
extern void nprint_symtab(BaseAST* ast);

extern void html_view(void);

#endif
