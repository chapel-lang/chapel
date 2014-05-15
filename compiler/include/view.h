#ifndef _VIEW_H_
#define _VIEW_H_

class BaseAST;

void     list_view_noline(BaseAST* ast);

void     html_view(const char* passName);

void     dump_ast(const char* passName, int pass_num);

void     nprint_view(BaseAST* ast);

BaseAST* aid(int id);

#endif
