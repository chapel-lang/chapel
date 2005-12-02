#include "alist.h"
#include "driver.h"
#include "filesToAST.h"
#include "runInterpreter.h"
#include "symbol.h"
#include "symtab.h"
#include "map.h"

struct IObject;

enum ISlotKind {
  EMPTY_SLOT,
  UNINITIALIZED_SLOT,
  OBJECT_SLOT,
  IMMEDIATE_SLOT
};

class ISlot : public gc { public:
  ISlotKind     kind;
  union {
    IObject *object;
    Immediate *imm;
  };
};

struct IObject {
  Type  *type;
  int   nslots;
  ISlot slot[1];        // nslots for real length
};

struct IFrame {
  IFrame *parent;
  ISlot retval;         // value returned to here from last call
};

Env<BaseAST *, ISlot *> env;

static void interpreter(AList<Stmt> *stmts, ISlot *);

static ISlot *
get_slot(Env<BaseAST *, ISlot *> &env, BaseAST *ast) {
  return NULL;
}

#define S(_t) _t *s = (_t *)ip; (void)s; if (trace_level > 0) printf( #_t " %p\n", s)

static void
check_type(ISlot &slot, Type *t) {
  return;
}

static void
interpreter(Expr *ip, ISlot *val = 0) {
  ISlot slot;
  if (!val) val = &slot;
  switch (ip->astType) {
    default: INT_FATAL("interpreter: bad astType: %d\n", ip->astType);
    case EXPR_SYM: {
      S(SymExpr);
      *val = *env.get(s->var);
      break;
    }
    case EXPR_DEF: {
      S(DefExpr);
      ISlot *slot = new ISlot;
      slot->kind = EMPTY_SLOT;
      env.put(s->sym, slot);
      if (trace_level)
        printf("  '%s' %d\n", !s->sym->name ? "" : s->sym->name, (int)s->id);
      break;
    }
    case EXPR_LET: {
      S(LetExpr);
      env.push();
      for_alist(DefExpr, e, s->symDefs)
        interpreter(e);
      interpreter(s->innerExpr, val);
      break;
    }
    case EXPR_COND: {
      S(CondExpr);
      ISlot slot;
      interpreter(s->condExpr, &slot);
      check_type(slot, dtBoolean);
      if (slot.imm->v_bool) 
        interpreter(s->thenExpr, val);
      else 
        interpreter(s->elseExpr, val);
      break;
    }
    case EXPR_CALL:
    case EXPR_CAST:
    case EXPR_MEMBERACCESS:
    case EXPR_REDUCE:
    case EXPR_NAMED:
    case EXPR_IMPORT:
      break;
  }
}

static void
interpreter(Stmt *ip, ISlot *val = 0) {
  ISlot slot;
  if (!val) val = &slot;
  env.push();
  Vec<Stmt *> stmtStack;
  while (1) {
    switch (ip->astType) {
      default: INT_FATAL("interpreter: bad astType: %d\n", ip->astType);
      case STMT: break;
      case STMT_EXPR: {
        S(ExprStmt);
        interpreter(s->expr);
        break;
      }
      case STMT_RETURN: {
        S(ReturnStmt);
        interpreter(s->expr, val);
        env.pop();
        return;
      }
      case STMT_BLOCK: {
        S(BlockStmt);
        stmtStack.add((Stmt*)ip->next);
        interpreter(s->body, val);
        break;
      }
      case STMT_WHILELOOP: {
        S(WhileLoopStmt);
        /* if (s->isWhileDo); */
        (void)s;
        break;
      }
      case STMT_FORLOOP: {
        S(ForLoopStmt);
        (void)s;
        break;
      }
      case STMT_COND: {
        S(CondStmt);
        ISlot slot;
        interpreter(s->condExpr, &slot);
        check_type(slot, dtBoolean);
        if (slot.imm->v_bool) 
          ip = s->thenStmt;
        else 
          ip = s->elseStmt;
        break;
      }
      case STMT_WHEN: {
        S(WhenStmt);
        SelectStmt *select = (SelectStmt*)s->parentStmt;
        assert(select->astType == STMT_SELECT);
        break;
      }
      case STMT_SELECT: {
        S(SelectStmt);
        interpreter(s->caseExpr, get_slot(env, s->caseExpr));
        break;
      }
      case STMT_LABEL: break;
      case STMT_GOTO: {
        S(GotoStmt);
        ip = s->label->defPoint->parentStmt;
        continue;
      }
    }
    ip = (Stmt*)ip->next;
    while (!ip) {
      if (!stmtStack.n)
        return;
      ip = stmtStack.pop();
    }
  }
}

static void
interpreter(AList<Stmt> *stmts, ISlot *val = 0) {
  if (!stmts->head) return;
  interpreter((Stmt*)stmts->head->next, val);
}

static void
initialize() {
  env.clear();
}

void runInterpreter(void) {
  if (!run_interpreter)
    return;
  initialize();
  forv_Vec(ModuleSymbol, mod, allModules)
    interpreter(mod->stmts);
  exit(1);
}
