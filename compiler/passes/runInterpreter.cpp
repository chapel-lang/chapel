#include <signal.h>
#include <ctype.h>
#include "alist.h"
#include "driver.h"
#include "filesToAST.h"
#include "runInterpreter.h"
#include "symbol.h"
#include "symtab.h"
#include "map.h"

class IObject;
class IThread;

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

class IObject : public gc { public:
  Type  *type;
  int   nslots;
  ISlot slot[1];        // nslots for real length
};

struct IFrame : public gc { public:
  IThread *thread;
  IFrame *parent;

  Map<BaseAST *, ISlot *> env;
  Vec<Stmt *> stmtStack;
  Vec<int> stmtStageStack;
  Vec<Expr *> exprStack;
  Vec<ISlot *> valStack;
  Stmt *stmt;
  int stage;
  Expr *expr;
  BaseAST *ip;

  ISlot *islot(BaseAST *ast);
  void icall(int nargs);
  void run(int timeslice);
  
  IFrame(IThread *t, Stmt *s);
};

struct IThread : public gc { public:
  IFrame *frame;
  
  void run();
  IThread(Stmt *s);
};

static int single_step = 0;
volatile static int interrupted = 0;

static Vec<IThread *> threads;

IFrame::IFrame(IThread *t, Stmt *s) : thread(t), parent(0), stmt(s), stage(0), expr(0), ip(s) {
  assert(s && stmt && ip);
}

IThread::IThread(Stmt *s) : frame(0) {
  frame = new IFrame(this, s);
}

ISlot *
IFrame::islot(BaseAST *ast) {
  ISlot *s = env.get(ast);
  if (!s)
    env.put(ast, (s = new ISlot));
  return s;
}

#define S(_t) _t *s = (_t *)ip; (void)s; if (trace_level > 0) printf( #_t " %p\n", s)

static void
check_type(BaseAST *ast, ISlot *slot, Type *t) {
  if (slot->kind == EMPTY_SLOT)
    USR_FATAL(ast, "interpreter: accessed empty variable");
  if (slot->kind == UNINITIALIZED_SLOT)
    USR_FATAL(ast, "interpreter: accessed uninitialized variable");
  return;
}

void
IFrame::icall(int nargs) {
  if (valStack.n < nargs)
    INT_FATAL(ip, "not enough arguments for call");
  char *name = 0;
  ISlot **args = &valStack.v[valStack.n-(nargs + 1)];
  if (nargs < 1)
    USR_FATAL(ip, "call with no arguments");
  int done = 0;
  do {
    if (args[0]->kind == SELECTOR_SLOT) {
      name = args[0]->selector;
      done = 1;
    } else if (args[0]->kind == CLOSURE_SLOT) {
      USR_FATAL(ip, "closures not handled yet");
    } else
      USR_FATAL(ip, "call to something other than function name or closure");
  } while (!done);
  Vec<FnSymbol *> visible;
  ip->parentScope->getVisibleFunctions(&visible, cannonicalize_string(name));
  if (visible.n != 1)
    USR_FATAL(ip, "unable to resolve function to single function '%s'", name);
  return;
}

static void
interactive_usage() {
  fprintf(stdout, "chpl Interpreter Interactive Mode Commands:\n");
  fprintf(stdout, 
          "\tp - print\n"
          "\tc - continue\n"
          "\tq - quit\n"
    );
}

static void 
handle_interrupt(int sig) {
  interrupted = 1;
}

static void
interactive() {
  while (1) {
    interrupted = 0;
    fprintf(stdout, "\n> ");
    char cmd_buffer[512], *c = cmd_buffer;
    cmd_buffer[0] = 0;
    fgets(cmd_buffer, 511, stdin);
    while (*c && isspace(*c)) c++;
    switch (tolower(*c)) {
      default: interactive_usage(); break;
      case 'q': exit(0); break;
      case 'c': return;
      case 'p': break;
    }
  }
}


#define PUSH_EXPR(_e) do { valStack.add(islot(_e)); exprStack.add(_e); } while (0)
#define EVAL_EXPR(_e) do { exprStack.add(_e); } while (0)
#define EVAL_STMT(_s) do { stmtStageStack.add(stage + 1); stmtStack.add(stmt); stmt = _s; } while (0)
#define PUSH_SELECTOR(_s) do { ISlot *_slot = new ISlot; _slot->set_selector(_s); valStack.add(_slot); } while (0)
#define PUSH_VAL(_s) valStack.add(islot(_s))
#define POP_VAL(_s) *islot(_s) = *valStack.pop();
#define CALL(_n) icall(_n)

void
IThread::run() {
  while (frame)
    frame->run(0);
}

void
IFrame::run(int timeslice) {
  assert(ip && stmt);
  while (1) {
  LgotoLabel:
    if (timeslice && !--timeslice)
      return;
    if (interrupted || single_step)
      interactive();
    switch (ip->astType) {
      default: INT_FATAL(ip, "interpreter: bad astType: %d", ip->astType);
      case STMT: break;
      case STMT_EXPR: {
        S(ExprStmt);
        EVAL_EXPR(s->expr);
        break;
      }
      case STMT_RETURN: {
        S(ReturnStmt);
        switch (stage++) {
          case 0: 
            PUSH_EXPR(s->expr);
            break;
          case 1: {
            stage = 0;
            ISlot *slot = valStack.pop();
            thread->frame = parent;
            if (parent)
              parent->valStack.add(slot);
            return;
          }
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
        }
        break;
      }
      case STMT_BLOCK: {
        S(BlockStmt);
        EVAL_STMT((Stmt*)s->body->head->next);
        break;
      }
      case STMT_WHILELOOP: {
        S(WhileLoopStmt);
        switch (stage) {
          case 0:
            stage = 1;
            if (!s->isWhileDo) 
              EVAL_STMT(s->block);
            break;
          case 1:
            stage = 2;
            EVAL_EXPR(s->condition);
            break;
          case 2: {
            ISlot *cond = islot(s->condition);
            check_type(ip, cond, dtBoolean);
            if (!cond->imm->v_bool)
              stage = 0;
            else {
              stage = 1;
              EVAL_STMT(s->block);
            }
            break;
          }
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
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
        BaseAST *loop_var = s;
        switch (stage++) {
          case 0: 
            EVAL_EXPR(iter); 
            break;
          case 1:
            PUSH_SELECTOR("_forall_start");
            PUSH_VAL(iter);
            CALL(2);
            break;
          case 2:
            POP_VAL(loop_var);
            PUSH_SELECTOR("_forall_valid");
            PUSH_VAL(iter);
            PUSH_VAL(loop_var);
            CALL(3);
            break;
          case 3: {
            ISlot *valid = valStack.pop();
            check_type(ip, valid, dtBoolean);
            if (!valid->imm->v_bool) {
              stage = 0;
              break;
            }
            PUSH_SELECTOR("_forall_index");
            PUSH_VAL(iter);
            PUSH_VAL(loop_var);
            CALL(3);
            break;
          }
          case 4:
            POP_VAL(indice);
            EVAL_STMT(s->innerStmt);
            break;
          case 5:
            PUSH_SELECTOR("_forall_next");
            PUSH_VAL(iter);
            PUSH_VAL(loop_var);
            CALL(3);
            break;
          case 6:
            stage = 2;
            POP_VAL(loop_var);
            break;
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
        }
        break;
      }
      case STMT_COND: {
        S(CondStmt);
        switch (stage++) {
          case 0:
            PUSH_EXPR(s->condExpr);
            break;
          case 1: {
            stage = 0;
            ISlot *cond = valStack.pop();
            check_type(ip, cond, dtBoolean);
            if (cond->imm->v_bool)
              EVAL_STMT(s->thenStmt);
            else
              EVAL_STMT(s->elseStmt);
            break;
          }
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
        }
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
        switch (stage++) {
          case 0:
            EVAL_EXPR(s->caseExpr);
            break;
          case 1:
            stage = 0;
            EVAL_STMT((Stmt*)s->whenStmts->head->next);
            break;
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
        }
        break;
      }
      case STMT_LABEL: break;
      case STMT_GOTO: {
        S(GotoStmt);
        stage = 0;
        valStack.clear();
        stmt = s->label->defPoint->parentStmt;
        goto LgotoLabel;
      }
      case EXPR_SYM: {
        S(SymExpr);
        ISlot *x = env.get(s->var);
        if (!x)
          USR_FATAL(ip, "unknown variable in SymExpr'%s'", s->var->name ? s->var->name : "");
        env.put(s, x);
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
        switch (stage++) {
          case 0:
            PUSH_EXPR(s->condExpr);
            break;
          case 1: {
            stage = 0;
            ISlot *cond = valStack.pop();
            check_type(ip, cond, dtBoolean);
            if (cond->imm->v_bool) {
              EVAL_EXPR(s->thenExpr);
              env.put(expr, islot(s->thenExpr));
            } else {
              EVAL_EXPR(s->elseExpr);
              env.put(expr, islot(s->thenExpr));
            }
            break;
	  }
          default: INT_FATAL(ip, "interpreter: bad stage %d for astType: %d", stage, ip->astType); break;
        }
        break;
      }
      case EXPR_CALL: {
        S(CallExpr);
        switch (stage++) {
          case 0: {
            switch (s->opTag) {
              default: 
                INT_FATAL("unhandled CallExpr::opTag: %d\n", s->opTag); 
                break;
              case OP_NONE:
                PUSH_EXPR(s->baseExpr);
                break;
            }
            break;
          }
          default:
            if (stage <= s->argList->length()) {
              PUSH_EXPR(s->argList->get(stage));
            } else {
              CALL(s->argList->length());
            }
            break;
        }
      }
      case EXPR_CAST:
      case EXPR_MEMBERACCESS:
      case EXPR_REDUCE:
      case EXPR_NAMED:
      case EXPR_IMPORT:
        break;
    }
    ip = expr = exprStack.pop();
    if (!expr) {
      if (!stage) {
        ip = stmt = (Stmt*)stmt->next;
        valStack.clear();
        while (!stmt) {
          stmt = stmtStack.pop();
          stage = stmtStageStack.pop() - 1;
          if (!stmt) {
            thread->frame = parent;
            return;
          }
          assert(stage >= 0);
          ip = stmt = (Stmt*)stmt->next;
        }
      }
    }
  }
}

static void
initialize() {
  signal(SIGINT, handle_interrupt);
}

void 
runInterpreter(void) {
  if (!run_interpreter)
    return;
  if (run_interpreter > 1)
    interrupted = 1;
  initialize();
  forv_Vec(ModuleSymbol, mod, allModules)
    (new IThread((Stmt*)mod->stmts->head->next))->run();
  exit(0);
}
