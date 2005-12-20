#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "alist.h"
#include "driver.h"
#include "filesToAST.h"
#include "runInterpreter.h"
#include "symbol.h"
#include "symtab.h"
#include "map.h"
#include "../traversals/view.h"

class IObject;
class IThread;

enum ISlotKind {
  EMPTY_ISLOT,
  UNINITIALIZED_ISLOT,
  SELECTOR_ISLOT,
  SYMBOL_ISLOT,
  CLOSURE_ISLOT,
  OBJECT_ISLOT,
  IMMEDIATE_ISLOT
};

class ISlot : public gc { public:
  ISlotKind     kind;
  union {
    IObject *object;
    Immediate *imm;
    char *selector;
    Symbol *symbol;
  };
  void set_selector(char *s) { 
    kind = SELECTOR_ISLOT;
    selector = s;
  }
  void set_symbol(Symbol *s) { 
    kind = SYMBOL_ISLOT;
    symbol = s;
  }
  ISlot &operator=(ISlot &s) {
    kind = s.kind;
    object = s.object; // representitive of the union
    return *this;
  }
  ISlot(Symbol *s) : kind(SYMBOL_ISLOT) { symbol = s; }
  ISlot() : kind(EMPTY_ISLOT) {}
};

class IObject : public BaseAST { public:
  Type  *type;
  int   nslots;
  ISlot slot[1];        // nslots for real length
};

typedef MapElem<BaseAST *, ISlot*> MapElemBaseASTISlot;

struct IFrame : public gc { public:
  IThread *thread;
  IFrame *parent;
  FnSymbol *function;
  int single_stepping;

  Map<BaseAST *, ISlot *> env;
  Vec<Stmt *> stmtStack;
  Vec<int> stageStack;
  Vec<Expr *> exprStack;
  Vec<ISlot *> valStack;
  Stmt *stmt;
  int stage;
  Expr *expr;
  BaseAST *ip;

  ISlot *islot(BaseAST *ast);
  void icall(int nargs);
  void icall(FnSymbol *fn, int nargs);
  void igoto(Stmt *s);
  void reset();
  void init(FnSymbol *fn);
  void init(Stmt *s);
  void init(AList<Stmt> *s);
  void init(BaseAST *s);
  int run(int timeslice = 0);
  
  IFrame(IThread *t);
};

enum IThreadState { ITHREAD_RUNNING, ITHREAD_RUNNABLE, ITHREAD_STOPPED };

struct IThread : public gc { public:
  IThreadState state;
  IFrame *frame;
  
  Vec<BaseAST *> todo;

  void add(BaseAST *s) { todo.add(s); }
  void clear() { todo.clear(); }

  int run(int timeslice = 0);
  
  IThread();
};

enum { NO_STEP = 0, SINGLE_STEP = 1, NEXT_STEP = 2 };
static int single_step = NO_STEP;
volatile static int interrupted = 0;

static Vec<IThread *> threads;
static int cur_thread = -1;
static Vec<char *> break_functions;
static Map<int, BaseAST*> known_ids;

static void runProgram();

IThread::IThread() : state(ITHREAD_STOPPED) {
  threads.add(this);
}

IFrame::IFrame(IThread *t) : thread(t), parent(0), function(0), single_stepping(NO_STEP), 
                             stmt(0), stage(0), expr(0), ip(0) 
{
}

ISlot *
IFrame::islot(BaseAST *ast) {
  ISlot *s = env.get(ast);
  if (!s)
    env.put(ast, (s = new ISlot));
  return s;
}

#define S(_t) _t *s = (_t *)ip; (void)s; \
  if (trace_level > 0) { \
    printf( #_t "(%d) %s:%d\n", (int)s->id, s->filename?s->filename:"<>", s->lineno); \
    known_ids.put(s->id, s); \
  }

static void
check_type(BaseAST *ast, ISlot *slot, Type *t) {
  if (slot->kind == EMPTY_ISLOT)
    USR_FATAL(ast, "interpreter: accessed empty variable");
  if (slot->kind == UNINITIALIZED_ISLOT)
    USR_FATAL(ast, "interpreter: accessed uninitialized variable");
  return;
}

void
IFrame::icall(FnSymbol *fn, int nargs) {
  if (trace_level) {
    printf("  Calling %s(%d)\n", fn->name, (int)fn->id);
    known_ids.put(fn->id, fn);
  }
  if (break_functions.n) {
    forv_Vec(char *, x, break_functions) {
      if (!strcmp(fn->name, x))
        interrupted = 1;
    }
  }
  if (!ip) {
    function = fn;
    ip = stmt = (Stmt*)fn->body->body->head->next;
  } else {
    IFrame *f = new IFrame(thread);
    f->init((Stmt*)fn->body->body->head->next);
    f->parent = this;
    f->function = fn;
    if (single_step == NEXT_STEP) {
      f->single_stepping = NEXT_STEP;
      single_step = NO_STEP;
    }
    thread->frame = f;
  }
  valStack.n -= nargs;
}

void
IFrame::icall(int nargs) {
  if (valStack.n < nargs)
    INT_FATAL(ip, "not enough arguments for call");
  char *name = 0;
  if (nargs < 1)
    USR_FATAL(ip, "call with no arguments");
  int done = 0;
  do {
    ISlot **args = &valStack.v[valStack.n-nargs];
    if (args[0]->kind == SYMBOL_ISLOT && args[0]->symbol->astType == SYMBOL_FN) {
      icall((FnSymbol*)args[0]->symbol, nargs);
      return;
    } else if (args[0]->kind == SELECTOR_ISLOT) {
      name = args[0]->selector;
      done = 1;
    } else if (args[0]->kind == CLOSURE_ISLOT) {
      USR_FATAL(ip, "closures not handled yet");
    } else
      USR_FATAL(ip, "call to something other than function name or closure");
  } while (!done);
  Vec<FnSymbol *> visible;
  ip->parentScope->getVisibleFunctions(&visible, cannonicalize_string(name));
  if (visible.n != 1)
    USR_FATAL(ip, "unable to resolve function to single function '%s'", name);
  icall(visible.v[0], nargs);
  return;
}

static void
interactive_usage() {
  fprintf(stdout, "chpl interpreter interactive mode commands:\n");
  fprintf(stdout, 
          "  step - single step\n"
          "  trace - trace program\n"
          "  where - show stack\n"
          "  locals - show locals\n"
          "  print - print by id number\n"
          "  nprint - print by id number showing ids\n"
          "  bi - break information\n"
          "  bf - break at a function\n"
          "  bfrm - remove a break function by number\n"
          "  continue - continue execution\n"
          "  run - restart execution\n"
          "  quit/exit - quit the interpreter\n"
          "  help - show commands (show this message)\n"
    );
}

static void 
handle_interrupt(int sig) {
  interrupted = 1;
}

#define STR_EQ(_c, _s) (!strncasecmp(_c, _s, sizeof(_s)-1))

static int
match_cmd(char *ac, char *str) {
  char *c = ac;
  while (*c) {
    if (isspace(*c))
      return 1;
    if (tolower(*c) != *str)
      return 0;
    c++; str++;
  }
  if (ac != c)
    return 1;
  else
    return 0;
}

static void
skip_arg(char *&c) {
  while (*c && !isspace(*c)) c++;
  while (*c && isspace(*c)) c++;
}

static char last_cmd_buffer[512] = "";

static void
show(BaseAST *ip, int stage) {
  printf("    %s(%d)", astTypeName[ip->astType], (int)ip->id); 
  if (stage)
    printf("/%d", stage);
  printf(" %s:%d\n", 
         ip->filename ? ip->filename:"<>", ip->lineno);
  known_ids.put(ip->id, ip);
}

static void
show(IFrame *frame, BaseAST *ip, int stage) {
  printf("    %s(%d)", astTypeName[ip->astType], (int)ip->id);
  if (stage)
    printf("/%d", stage);
  printf(" in %s %s:%d\n", 
         frame->function ? frame->function->name : "<initialization>",
         ip->filename?ip->filename:"<>", ip->lineno);
  known_ids.put(ip->id, ip);
}

static int
check_running(IFrame *frame) {
  if (!frame) {
    printf("    error: no running program\n");
    return 0;
  }
  return 1;
}

static void
cmd_where(IFrame *frame) {
  if (!check_running(frame))
    return;
  Expr *e = frame->expr;
  int stage = frame->stage;
  int istage = frame->stageStack.n;
  int iexpr = frame->exprStack.n;
  while (e) {
    show(e, stage);
    istage--;
    assert(istage >= 0); 
    stage = frame->stageStack.v[istage];
    if (iexpr <= 0) break;
    iexpr--;
    e = frame->exprStack.v[iexpr];
  }
  Stmt *s = frame->stmt;
  int istmt = frame->stmtStack.n;
  while (s) {
    if (!istage)
      show(frame, s, stage);
    else
      show(s, stage);
    if (istmt <= 0) break;
    istmt--;
    istage--;
    assert(istage >= 0); 
    stage = frame->stageStack.v[istage];
    s = frame->stmtStack.v[istage];
  }
  frame = frame->parent;
  while (frame) {
    if (frame->ip) {
      show(frame, frame->ip, frame->stage);
    } else
      printf("    error: bad stack frame\n");
    frame = frame->parent;
  }
}

static void
print(ISlot *islot) {
  switch (islot->kind) {
    default: INT_FATAL("interpreter: bad slot type: %d", islot->kind); break;
    case EMPTY_ISLOT: printf("<empty>"); break;
    case UNINITIALIZED_ISLOT: printf("<uninitialized>"); break;
    case SELECTOR_ISLOT: printf("selector '%s'", islot->selector); break;
    case SYMBOL_ISLOT: 
      printf("symbol: %s ", astTypeName[islot->symbol->astType]);
      islot->symbol->print(stdout); 
      printf("(%d)", (int)islot->symbol->id); 
      known_ids.put(islot->symbol->id, islot->symbol);
      break;
    case CLOSURE_ISLOT: printf("closure: "); break;
    case OBJECT_ISLOT: 
      printf("object: %d", (int)islot->object->id); 
      known_ids.put(islot->object->id, islot->object);
      break;
    case IMMEDIATE_ISLOT: printf("immediate: "); fprint_imm(stdout, *islot->imm); break;
  }
}

static void
cmd_stack(IFrame *frame) {
  if (!check_running(frame))
    return;
  printf("  value stack:\n");
  for (int i = frame->valStack.n-1; i >= 0; i--) {
    printf("    ");
    print(frame->valStack.v[i]);
  }
}

static void
cmd_locals(IFrame *frame) {
  if (!check_running(frame))
    return;
  printf("  local symbols:\n");
  form_Map(MapElemBaseASTISlot, x, frame->env) {
    if (Symbol *s = dynamic_cast<Symbol*>(x->key)) {
      printf("    ");
      s->print(stdout);
      printf("(%d) = ", (int)s->id);
      print(x->value);
      printf("\n");
      known_ids.put(s->id, s);
    }
  }
}

static void
cmd_print(int i, int nprint = 0) {
  static int last_print = 0;
  if (!i)
    i = last_print;
  if (i <= 0) {
    interactive_usage();
    return;
  }
  BaseAST *p = known_ids.get(i);
  if (!p) {
    printf("  unknown id: %d\n", i);
    return;
  }
  last_print = i;
  if (BaseAST *b = dynamic_cast<BaseAST *>(p)) {
    if (!nprint)
      print_view_noline(b);
    else
      nprint_view_noline(b);
    printf("\n");
  }
}

static int
interactive(IFrame *frame) {
  if (frame)
    show(frame, frame->ip, frame->stage);
  while (1) {
    single_step = interrupted = 0;
#ifdef USE_READLINE
    char *c = readline("(chpl) ");
    if (!c)
      exit(0);
    else
      add_history(c);
#else
    fprintf(stdout, "(chpl) ");
    char cmd_buffer[512], *c = cmd_buffer;
    cmd_buffer[0] = 0;
    fgets(cmd_buffer, 511, stdin);
#endif
    while (*c && isspace(*c)) c++;
    if (!*c)
      c = last_cmd_buffer;
    else
      strcpy(last_cmd_buffer, c);
    // Insert commands in priority order.  First partial match
    // will result in command execution. (e.g. q/qu/qui/quit are quit
    if (0) {
    } else if (match_cmd(c, "help") || match_cmd(c, "?")) {
      interactive_usage();
    } else if (match_cmd(c, "quit")) {
      exit(0);
    } else if (match_cmd(c, "continue")) {
      check_running(frame);
      return 0;
    } else if (match_cmd(c, "step")) {
      check_running(frame);
      single_step = SINGLE_STEP;
      return 0;
    } else if (match_cmd(c, "next")) {
      check_running(frame);
      single_step = NEXT_STEP;
      return 0;
    } else if (match_cmd(c, "print")) {
      skip_arg(c);
      cmd_print(atoi(c));
    } else if (match_cmd(c, "nprint")) {
      skip_arg(c);
      cmd_print(atoi(c), 1);
    } else if (match_cmd(c, "where")) {
      cmd_where(frame);
    } else if (match_cmd(c, "stack")) {
      cmd_stack(frame);
    } else if (match_cmd(c, "locals")) {
      cmd_locals(frame);
    } else if (match_cmd(c, "bf")) {
      skip_arg(c);
      char *e = c;
      while (*e && !isspace(*e)) e++;
      *e = 0;
      break_functions.add(dupstr(c));
      printf("  breaking at start of function '%s'\n", c);
    } else if (match_cmd(c, "bfrm")) {
      skip_arg(c);
      int i = atoi(c);
      printf("  removing bf %d\n", i);
      break_functions.remove(i);
    } else if (match_cmd(c, "bi")) {
      printf("  break functions: %d\n", break_functions.n);
      for (int i = 0; i < break_functions.n; i++) {
        char *x = break_functions.v[i];
        printf("    bf %d '%s'\n", i, x);
      }
    } else if (match_cmd(c, "trace")) {
      skip_arg(c);
      if (!*c)
        trace_level = !trace_level;
      else {
        if (match_cmd(c, "true"))
          trace_level = 1;
        else if (match_cmd(c, "false"))
          trace_level = 0;
        else
          trace_level = atoi(c);
      }
      printf("  tracing level set to %d\n", trace_level);
    } else if (match_cmd(c, "run")) {
      if (frame) {
        frame->thread->todo.clear();
        frame->thread->frame = 0;
      }
      runProgram();
      return 1;
    } else if (match_cmd(c, "exit")) {
      exit(0);
    } else {
      if (*c)
        printf("  unknown command\n");
      interactive_usage();
    }
  }
  return 0;
}

int
IThread::run(int atimeslice) {
  int timeslice = atimeslice;
  while (frame || todo.n) {
    if (!frame)
      frame = new IFrame(this);
    if (!frame->ip) {
      if (todo.n) {
        BaseAST *s = todo.v[0];
        todo.remove(0);
        frame->init(s);
      }
    }
    while (frame) {
      timeslice = frame->run(timeslice);
      if (atimeslice && !timeslice)
        return timeslice;
    }
  }
  return timeslice;  
}

void
IFrame::reset() {
  function = 0;
  env.clear();
  stmtStack.clear();
  stageStack.clear();
  exprStack.clear();
  valStack.clear();
  ip = stmt = 0;
  expr = 0;
}

#define PUSH_EXPR(_e) do { \
  Expr *__e = _e;  \
  if (expr && stage) { exprStack.add(expr); } \
  stageStack.add(stage + 1); \
  valStack.add(islot(__e)); \
  stageStack.add(1); exprStack.add(__e); stage = 0; expr = __e; \
} while (0)
#define EVAL_EXPR(_e) do { \
  Expr *__e = _e;  \
  if (expr && stage) { exprStack.add(expr); }  \
  stageStack.add(stage + 1); \
  stageStack.add(1); exprStack.add(__e); stage = 0; expr = __e; \
} while (0)
#define EVAL_STMT(_s) do { stageStack.add(stage + 1); stmtStack.add(stmt); stmt = _s; } while (0)
#define PUSH_SELECTOR(_s) do { ISlot *_slot = new ISlot; _slot->set_selector(_s); valStack.add(_slot); } while (0)
#define PUSH_VAL(_s) valStack.add(islot(_s))
#define PUSH_SYM(_s)  do { ISlot *_slot = new ISlot; _slot->set_symbol(_s); valStack.add(_slot); } while (0)
#define POP_VAL(_s) *islot(_s) = *valStack.pop();
#define CALL(_n) do { icall(_n); return timeslice; } while (0)

void
IFrame::init(FnSymbol *fn) {
  reset();
  PUSH_SYM(chpl_main);
  icall(1);
}

void
IFrame::init(Stmt *s) {
  reset();
  ip = stmt = s;
}

void
IFrame::init(AList<Stmt> *s) {
  reset();
  ip = stmt = (Stmt*)s->head->next;
}

void
IFrame::init(BaseAST *s) {
  if (FnSymbol *x = dynamic_cast<FnSymbol*>(s)) {
    init(x);
  } else if (Stmt *x = dynamic_cast<Stmt*>(s)) {
    init(x);
  } else if (AList<Stmt> *x = dynamic_cast<AList<Stmt>*>(s)) {
    init(x);
  } else {
    INT_FATAL(ip, "interpreter: bad astType: %d", s->astType);
  }
}

void
IFrame::igoto(Stmt *s) {
  Vec<Stmt *> parents;
  Stmt *ss = s;
  while (ss->parentStmt) {
    parents.add(ss->parentStmt);
    ss = ss->parentStmt;
  }
  parents.reverse();
  if (parents.n > stmtStack.n)
    USR_FATAL("interpreter: goto target nested below source");
  for (int i = 0; i < parents.n; i++)
    if (parents.v[i] != stmtStack.v[i])
      USR_FATAL("interpreter: illegal goto target");
  ss = stmt;
  Expr *defexpr = 0;
  while (ss) {
    if (ss->astType == STMT_EXPR) {
      if (ExprStmt *x = dynamic_cast<ExprStmt *>(ss))
        if (x->expr->astType == EXPR_DEF)
          defexpr = x->expr;
    }
    if (defexpr && ss == s)
      USR_FATAL("interpreter: illegal goto over variable definition DefExpr(%d)", defexpr->id);
    ss = (Stmt*)ss->next;
  }
  stage = 0;
  stmt = s;
  ip = expr = 0;
  valStack.clear();
  stageStack.n = parents.n;
  stmtStack.n = parents.n;
}

int
IFrame::run(int timeslice) {
  if (expr)
    goto LnextExpr;
  while (1) {
  LgotoLabel:
    if (timeslice && !--timeslice)
      return timeslice;
    if (interrupted)
      if (interactive(this))
        return 0;
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
            if (thread->frame->single_stepping == NEXT_STEP)
              single_step = NEXT_STEP;
            if (parent)
              parent->valStack.add(slot);
            return timeslice;
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
        igoto(s->label->defPoint->parentStmt);
        goto LgotoLabel;
      }
      case EXPR_SYM: {
        S(SymExpr);
        ISlot *x = env.get(s->var);
        if (!x) {
          switch (s->var->astType) {
            case SYMBOL_UNRESOLVED: {
              x = new ISlot();
              x->set_selector(s->var->name);
              env.put(s->var, x);
              break;
            }
            case SYMBOL_FN: 
            case SYMBOL_TYPE: 
              env.put(s->var, (x = new ISlot(s->var)));
              break;
            default:
              USR_FATAL(ip, "unknown variable in SymExpr '%s'", s->var->name ? s->var->name : "");
              break;
          }
        }
        assert(x);
        ISlot *e = env.get(s);  
        if (e)
          *e = *x;
        else
          env.put(s, x);
        break;
      }
      case EXPR_DEF: {
        S(DefExpr);
        ISlot *slot = new ISlot;
        slot->kind = EMPTY_ISLOT;
        env.put(s->sym, slot);
        switch (s->sym->astType) {
          default: break;
          case SYMBOL_UNRESOLVED:
          case SYMBOL_MODULE:
          case SYMBOL_TYPE:
          case SYMBOL_FN:
          case SYMBOL_ENUM:
          case SYMBOL_LABEL:
            slot->set_symbol(s->sym);
            break;
        }
        if (trace_level) {
          printf("  %s(%d)\n", !s->sym->name ? "" : s->sym->name, (int)s->id);
          known_ids.put(s->id, s);
        }
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
            if (stage - 1 <= s->argList->length()) {
              PUSH_EXPR(s->argList->get(stage - 1));
            } else {
              stage = 0;
              CALL(s->argList->length() + 1);
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
  LnextExpr:
    if (!stage) {
      if (expr) {
        stage = stageStack.pop() - 1;
        if (exprStack.n)
          ip = expr = exprStack.pop();
        else
          ip = expr = 0;
      }
      if (!expr && !stage) {
        ip = stmt = (Stmt*)stmt->next;
        valStack.clear();
        while (!stmt) {
          stmt = stmtStack.pop();
          stage = stageStack.pop() - 1;
          if (!stmt) {
            thread->frame = parent;
            return timeslice;
          }
          assert(stage >= 0);
          ip = stmt = (Stmt*)stmt->next;
        }
      }
    }
    if (single_step)
      interrupted = 1;
  }
}

static void
initialize() {
  signal(SIGINT, handle_interrupt);
}

static void
runProgram() {
  if (run_interpreter > 1)
    interrupted = 1;
  threads.clear();
  cur_thread = -1;
  IThread *t = new IThread;
  forv_Vec(ModuleSymbol, mod, allModules)
    t->add((Stmt*)mod->stmts->head->next);
  t->add(chpl_main);
  t->state = ITHREAD_RUNNABLE;
}

static void
chpl_interpreter() {
  while (threads.n) {
    cur_thread = (cur_thread + 1) % threads.n;
    IThread *t = threads.v[cur_thread];
    t->run(0);
    if (!t->frame && cur_thread >= 0)
      threads.remove(cur_thread);
  }
}

void 
runInterpreter(void) {
  if (!run_interpreter)
    return;
  initialize();
  do {
    runProgram();
    chpl_interpreter();
    printf("  program terminated\n");
    while (!threads.n) 
      interactive(0);
  } while (run_interpreter > 1);
  exit(0);
}
