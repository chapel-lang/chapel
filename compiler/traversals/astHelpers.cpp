#include "traversal.h"
#include "baseAST.h"
#include "stmt.h"
#include "expr.h"


class ClearFileInfo : public Traversal {
public:
  int lineno;
  char* filename;
  ClearFileInfo(int lineno = -1, char* filename = "<internal>");
  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
};


ClearFileInfo::ClearFileInfo(int iLineno, char* iFilename) :
  lineno(iLineno),
  filename(iFilename)
{ }


void ClearFileInfo::preProcessStmt(Stmt* stmt) {
  stmt->lineno = lineno;
  stmt->filename = filename;
}


void ClearFileInfo::preProcessExpr(Expr* expr) {
  expr->lineno = lineno;
  expr->filename = filename;
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    defExpr->sym->lineno = lineno;
    defExpr->sym->filename = filename;
  }
}


void clear_file_info(BaseAST* baseAST) {
  TRAVERSE(baseAST, new ClearFileInfo(), true);
}


void reset_file_info(BaseAST* baseAST, int lineno, char* filename) {
  TRAVERSE(baseAST, new ClearFileInfo(lineno, filename), true);
}
