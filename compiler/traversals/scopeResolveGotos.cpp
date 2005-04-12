#include <typeinfo>
#include "scopeResolveGotos.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"


class FindBreakOrContinue : public Traversal {
 public:
  bool found;
  FindBreakOrContinue(void) : found(false) { }
  void preProcessStmt(Stmt* stmt) {
    if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt)) {
      if (goto_stmt->label == NULL) {
        found = true;
      }
    }
  }
};


ScopeResolveGotos::ScopeResolveGotos() :
  currentLoop(NULL)
{ }


void ScopeResolveGotos::preProcessStmt(Stmt* stmt) {
  static int uid = 1;

  if (dynamic_cast<WhileLoopStmt*>(stmt) ||
      dynamic_cast<ForLoopStmt*>(stmt)) { 

    BlockStmt* loop_stmt = dynamic_cast<BlockStmt*>(stmt);

    if (!loop_stmt) {
      INT_FATAL(stmt, "BlockStmt expected in ScopeResolveGotos");
    }

    FindBreakOrContinue* traversal = new FindBreakOrContinue();
    TRAVERSE_LS(loop_stmt->body, traversal, true);
    if (traversal->found) {
      NoOpStmt* noop_stmt = new NoOpStmt();
      loop_stmt->replace(noop_stmt);

      char* label_name = glomstrings(2, "_loop_label_", intstring(uid++));
      LabelSymbol* label_symbol = new LabelSymbol(label_name);
      LabelStmt* label_stmt = new LabelStmt(label_symbol, loop_stmt);
      noop_stmt->replace(label_stmt);
      currentLoop = label_stmt;
      return;
    }
  }

  GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt);

  if (!goto_stmt) {
    return;
  }

  if (!goto_stmt->label) {
    if (!currentLoop) {
      USR_FATAL(stmt, "break or continue is not in a loop");
    } else {
      goto_stmt->label = currentLoop->label;
    }
  } else if (dynamic_cast<UnresolvedSymbol*>(goto_stmt->label)) {
    Symbol* new_symbol = Symboltable::lookup(goto_stmt->label->name);
    if (typeid(*new_symbol) == typeid(LabelSymbol)) {
      goto_stmt->label = new_symbol;
    } else {
      INT_FATAL(stmt, "Unable to resolve goto label");
    }
  } else {
    INT_FATAL(stmt, "Label already resolved in goto");
  }
}
