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
  SELECTOR_SLOT,
  CLOSURE_SLOT,
  OBJECT_SLOT,
  IMMEDIATE_SLOT
};

class ISlot : public gc { public:
  ISlotKind     kind;
  union {
    IObject *object;
    Immediate *imm;
    char *selector;
  };
  void set_selector(char *s) { 
    kind = SELECTOR_SLOT;
    selector = s;
  }
  ISlot &operator=(ISlot &s) {
    kind = s.kind;
    object = s.object; // representitive of the union
    return *this;
  }
  ISlot() : kind(EMPTY_SLOT) {}
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

static void interpreter(AList<Stmt> *stmts, ISlot *val = 0);
static void interpreter(Stmt *ip, ISlot *val = 0);
static void interpreter(Expr *ip, ISlot *val = 0);

static ISlot *
islot(BaseAST *ast) {
  ISlot *s = env.get(ast);
  if (!s)
    env.put(ast, (s = new ISlot));
  return s;
}

#define S(_t) _t *s = (_t *)ip; (void)s; if (trace_level > 0) printf( #_t " %p\n", s)

static void
check_type(BaseAST *ast, ISlot &slot, Type *t) {
  if (slot.kind == EMPTY_SLOT)
    USR_FATAL(ast, "interpreter: accessed empty variable");
  if (slot.kind == UNINITIALIZED_SLOT)
    USR_FATAL(ast, "interpreter: accessed uninitialized variable");
  return;
}

static void
i_call(Expr *e, ISlot *args, int nargs, ISlot *val = 0) {
  char *name = 0;
  if (nargs < 1)
    USR_FATAL(e, "call with no arguments");
  int done = 0;
  do {
    if (args[0].kind == SELECTOR_SLOT) {
      name = args[0].selector;
      done = 1;
    } else if (args[0].kind == CLOSURE_SLOT) {
    } else
      USR_FATAL(e, "call to something other than function name or closure");
  } while (!done);
  Vec<FnSymbol *> visible;
  e->parentScope->getVisibleFunctions(&visible, cannonicalize_string(name));
  if (visible.n != 1)
    USR_FATAL(e, "unable to resolve function to single function '%s'", name);
  return;
}

static void
interpreter(Expr *ip, ISlot *val) {
  ISlot slot;
  if (!val) val = &slot;
  switch (ip->astType) {
    default: INT_FATAL(ip, "interpreter: bad astType: %d", ip->astType);
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
    case EXPR_COND: {
      S(CondExpr);
      ISlot slot;
      interpreter(s->condExpr, &slot);
      check_type(ip, slot, dtBoolean);
      if (slot.imm->v_bool) 
        interpreter(s->thenExpr, val);
      else 
        interpreter(s->elseExpr, val);
      break;
    }
    case EXPR_CALL: {
      S(CallExpr);
      ISlot base_slot;
      int n = s->argList->length() + 1;
      ISlot arg_slot[n];
      interpreter(s->baseExpr, &arg_slot[0]);
      int a = 1;
      for_alist(Expr, x, s->argList)
        interpreter(x, &arg_slot[a++]);
      i_call(ip, arg_slot, n, val);
      break;
    }
    case EXPR_CAST:
    case EXPR_MEMBERACCESS:
    case EXPR_REDUCE:
    case EXPR_NAMED:
    case EXPR_IMPORT:
      break;
  }
}

static ISlot *
eval(Expr *e) {
  ISlot *s = islot(e);
  interpreter(e, s);
  return s;
}

static Symbol *
new_temp(char *name = 0) {
  return new UnresolvedSymbol(name ? name : (char*)"temp");
}

static void
interpreter(Stmt *ip, ISlot *val) {
  ISlot slot;
  if (!val) val = &slot;
  Vec<Stmt *> stmtStack;
  while (1) {
    switch (ip->astType) {
      default: INT_FATAL(ip, "interpreter: bad astType: %d", ip->astType);
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
        if (!s->isWhileDo) 
          interpreter(s->block);
        while (1) {
          ISlot slot;
          interpreter(s->block);
          interpreter(s->condition);
          check_type(ip, slot, dtBoolean);
          if (!slot.imm->v_bool)
            break;
        }
        break;
      }
      case STMT_FORLOOP: {
        S(ForLoopStmt);
        if (!s->indices || s->indices->length() != 1)
          INT_FATAL(ip, "interpreter: bad number of indices");
        if (!s->iterators || s->iterators->length() != 1)
          INT_FATAL(ip, "interpreter: bad number of iterators");
        Expr *iter = s->iterators->only();
        Symbol *indice = s->indices->only()->sym;
        Symbol *loop_var = new_temp("loop_var");
        ISlot arg[5];
        arg[0].set_selector("_forall_start");
        arg[1] = *eval(iter);
        i_call(iter, arg, 2, islot(loop_var));
        while (1) {
          ISlot valid;
          arg[0].set_selector("_forall_valid");
          arg[1] = *islot(iter);
          arg[2] = *islot(loop_var);
          i_call(iter, arg, 3, &valid);
          check_type(ip, valid, dtBoolean);
          if (!valid.imm->v_bool)
            break;

          arg[0].set_selector("_forall_index");
          arg[1] = *islot(iter);
          arg[2] = *islot(loop_var);
          i_call(iter, arg, 3, islot(indice));
          
          interpreter(s->innerStmt);

          arg[0].set_selector("_forall_next");
          arg[1] = *islot(iter);
          arg[2] = *islot(loop_var);
          i_call(iter, arg, 3, islot(loop_var));
        }
        break;
      }
      case STMT_COND: {
        S(CondStmt);
        ISlot slot;
        interpreter(s->condExpr, &slot);
        check_type(ip, slot, dtBoolean);
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
        interpreter(s->caseExpr, islot(s->caseExpr));
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
interpreter(AList<Stmt> *stmts, ISlot *val) {
  if (!stmts->head) return;
  interpreter((Stmt*)stmts->head->next, val);
}

#define UNRESOLVED(_x) _x = new UnresolvedSymbol(#_x)

static void
initialize() {
  env.clear();
  env.push();
}

void runInterpreter(void) {
  if (!run_interpreter)
    return;
  initialize();
  forv_Vec(ModuleSymbol, mod, allModules)
    interpreter(mod->stmts);
  exit(1);
}
