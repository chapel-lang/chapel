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
  preCurrentLoop(NULL),
  postCurrentLoop(NULL)
{ }


void ScopeResolveGotos::preProcessStmt(Stmt* stmt) {
  static int uid = 1;

  if (dynamic_cast<WhileLoopStmt*>(stmt) ||
      dynamic_cast<ForLoopStmt*>(stmt)) { 

    BlockStmt* loop_block;

    if (WhileLoopStmt* loop = dynamic_cast<WhileLoopStmt*>(stmt)) {
      loop_block = loop->block;
    }
    if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(stmt)) {
      loop_block = loop->innerStmt;
    }

    if (!loop_block) {
      INT_FATAL(stmt, "BlockStmt expected in ScopeResolveGotos");
    }

    if (LabelStmt* labelStmt = dynamic_cast<LabelStmt*>(loop_block->parentStmt->prev)) {
      LabelSymbol* post_label_symbol = new LabelSymbol(glomstrings(2, "_post", labelStmt->defLabel->sym->name));
      loop_block->parentStmt->insertAfter(new LabelStmt(new DefExpr(post_label_symbol)));
    }

    FindBreakOrContinue* traversal = new FindBreakOrContinue();
    loop_block->body->traverse(traversal, true);
    if (traversal->found) {
      char* label_name = glomstrings(2, "_loop_label_", intstring(uid++));
      LabelSymbol* label_symbol = new LabelSymbol(label_name);
      LabelStmt* label_stmt = new LabelStmt(new DefExpr(label_symbol));
      stmt->insertBefore(label_stmt);
      preCurrentLoop = label_stmt;
      label_name = glomstrings(2, "_post", label_name);
      label_symbol = new LabelSymbol(label_name);
      label_stmt = new LabelStmt(new DefExpr(label_symbol));
      stmt->insertAfter(label_stmt);
      postCurrentLoop = label_stmt;
      return;
    }
  }

  GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt);

  if (!goto_stmt) {
    return;
  }

  if (!goto_stmt->label) {
    if (!preCurrentLoop) {
      USR_FATAL(stmt, "break or continue is not in a loop");
    } else {
      if (goto_stmt->goto_type == goto_break) {
        goto_stmt->label = postCurrentLoop->defLabel->sym;
      } else if (goto_stmt->goto_type == goto_continue) {
        goto_stmt->label = preCurrentLoop->defLabel->sym;
      } else {
        INT_FATAL(stmt, "Unexpected goto type");
      }
    }
  } else if (dynamic_cast<UnresolvedSymbol*>(goto_stmt->label)) {
    char* label_name = goto_stmt->label->name;
    if (goto_stmt->goto_type == goto_break) {
      label_name = glomstrings(2, "_post", label_name);
    }
    Symbol* new_symbol = Symboltable::lookup(label_name);
    if (dynamic_cast<LabelSymbol*>(new_symbol)) {
      goto_stmt->label = new_symbol;
    } else {
      INT_FATAL(stmt, "Unable to resolve goto label");
    }
  } else {
    INT_FATAL(stmt, "Label already resolved in goto");
  }
}
