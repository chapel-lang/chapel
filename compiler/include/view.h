/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


#ifndef _VIEW_H_
#define _VIEW_H_

#include "baseAST.h"

BaseAST* aid(int id);
void print_view(BaseAST* ast);
void print_view_noline(BaseAST* ast);
void iprint_view(int id);
void nprint_view(BaseAST* ast);
void nprint_view_noline(BaseAST* ast);
void mark_view(BaseAST* ast, int id);
void list_view(BaseAST* ast);
void list_view_noline(BaseAST* ast);

void html_view(const char* passName);
void dump_ast(const char* passName, int pass_num);
void map_view(SymbolMap* map, const char* msg = NULL);

#endif
