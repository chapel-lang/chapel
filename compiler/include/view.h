#ifndef _VIEW_H_
#define _VIEW_H_

#include "baseAST.h"
#include "chplenum.h"

extern void print_view(BaseAST* ast);
extern void print_view_noline(BaseAST* ast);
extern void nprint_view(BaseAST* ast);
extern void nprint_view_noline(BaseAST* ast);
extern void mark_view(BaseAST* ast, long id);

extern void html_view(int show_analysis_info);

#endif
