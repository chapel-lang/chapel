#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif
#include "pass.h"
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
  SELECTOR_ISLOT,
  SYMBOL_ISLOT,
  CLOSURE_ISLOT,
  OBJECT_ISLOT,
  IMMEDIATE_ISLOT
};

char *slotKindName[] = {
  "Empty",
  "Selector",
  "Symbol",
  "Closure",
  "Object",
  "Immediate"
};

class ISlot : public gc { public:
  ISlotKind     kind;
  union {
    IObject *object;
    Immediate *imm;
    char *selector;
    Symbol *symbol;
  };
  void set_selector(char *s) { kind = SELECTOR_ISLOT; selector = s; }
  void set_symbol(Symbol *s) { kind = SYMBOL_ISLOT; symbol = s; }
  void set_immediate(Immediate *i) { kind = IMMEDIATE_ISLOT; imm = i; }
  ISlot &operator=(ISlot &s) {
    kind = s.kind;
    object = s.object; // representitive of the union
    return *this;
  }
  ISlot(Symbol *s) : kind(SYMBOL_ISLOT) { symbol = s; }
  ISlot(Immediate *i) : kind(IMMEDIATE_ISLOT) { imm = i; }
  ISlot(char *sel) : kind(SELECTOR_ISLOT) { selector = sel; }
  ISlot(ISlot &s) { kind = s.kind; object = s.object; }
  ISlot() : kind(EMPTY_ISLOT) {}
};

class IObject : public BaseAST { public:
  ClassType *type;
  Vec<BaseAST *> alloc_context;
  Map<BaseAST *, ISlot *> member;
  Vec<int> dim;
  Vec<ISlot *> array;
  
  void print(int fnprint = 0);

  IObject() : BaseAST(OBJECT), type(0) {}
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
  ISlot *return_slot;

  ISlot *islot(BaseAST *ast);
  void icall(int nargs, ISlot *ret_slot = 0);
  void icall(FnSymbol *fn, int nargs = 0, int extra_args = 0);
  int igoto(Stmt *s);
  void iprimitive(CallExpr *s);
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

enum PrimOps {
  PRIM_NONE, PRIM_INIT, PRIM_ALLOC, PRIM_FOPEN, PRIM_FCLOSE,
  PRIM_STRERROR, PRIM_FPRINTF, PRIM_FSCANF, PRIM_ARRAY_INDEX,
  PRIM_ARRAY_SET, PRIM_UNARY_MINUS, PRIM_UNARY_PLUS,
  PRIM_UNARY_BNOT, PRIM_UNARY_NOT, PRIM_ADD,
  PRIM_SUBTRACT, PRIM_MULT, PRIM_DIV, PRIM_MOD, PRIM_EQUAL,
  PRIM_NOTEQUAL, PRIM_ELSSOREQUAL, PRIM_GREATEROREQUAL, PRIM_LESS,
  PRIM_GREATER, PRIM_AND, PRIM_OR, PRIM_XOR, PRIM_LAND,
  PRIM_LOR, PRIM_EXP, PRIM_PTR_EQ, PRIM_PTR_NEQ, PRIM_CAST,
  PRIM_TO_STRING, PRIM_COPY_STRING, PRIM_STRING_INDEX, PRIM_STRING_CONCAT,
  PRIM_STRING_EQUAL, PRIM_STRING_SELECT, PRIM_STRING_STRIDED_SELECT,
  PRIM_STRING_LENGTH, PRIM_DONE 
};

class InterpreterOp : public gc { public:
  char *name;
  PrimOps kind;
  
  InterpreterOp(char *n, PrimOps k) : name(n), kind(k) {}
};

#define _EXTERN
#define _INIT = NULL
#include "interpreter_ops.h"

enum { NO_STEP = 0, SINGLE_STEP = 1, NEXT_STEP = 2 };

volatile static int interrupted = 0;
static int single_step = NO_STEP;
static Vec<IThread *> threads;
static int cur_thread = -1;
static Vec<int> break_ids;
static Map<int, BaseAST*> known_ids;

static void runProgram();
static void error_interactive(IFrame *frame);

IThread::IThread() : state(ITHREAD_STOPPED) {
  threads.add(this);
}

IFrame::IFrame(IThread *t) : thread(t), parent(0), function(0), single_stepping(NO_STEP), 
                             stmt(0), stage(0), expr(0), ip(0), return_slot(0) 
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
check_kind(BaseAST *ast, ISlot *slot, int kind) {
  if (slot->kind != kind) {
    USR_FATAL(ast, "interpreter: unexpected kind of slot %s: expected %s", 
              slotKindName[slot->kind], slotKindName[kind]);
  }
}

static void
check_type(BaseAST *ast, ISlot *slot, Type *t) {
  if (slot->kind == EMPTY_ISLOT) {
    USR_FATAL(ast, "interpreter: accessed empty variable");
  }
  if (slot->kind == IMMEDIATE_ISLOT) {
    switch (slot->imm->const_kind) {
      default: 
    Lerror:
        USR_FATAL(ast, "interpreter: bad immediate type");
      case IF1_NUM_KIND_UINT:
        if (t == dtBoolean && slot->imm->num_index == IF1_INT_TYPE_1) break;
        goto Lerror;
      case IF1_NUM_KIND_INT:
        if (t == dtInteger && slot->imm->num_index == IF1_INT_TYPE_64) break;
        goto Lerror;
      case IF1_NUM_KIND_FLOAT:
        if (t == dtFloat && slot->imm->num_index == IF1_FLOAT_TYPE_64) break;
        goto Lerror;
      case IF1_NUM_KIND_COMPLEX:
        if (t == dtFloat && slot->imm->num_index == IF1_FLOAT_TYPE_64) break;
        goto Lerror;
      case IF1_CONST_KIND_STRING:
        if (t == dtString) break;
        goto Lerror;
    }
  }
  return;
}

void
IFrame::icall(FnSymbol *fn, int nargs, int extra_args) {
  if (trace_level) {
    printf("  Calling %s(%d)\n", fn->name, (int)fn->id);
    known_ids.put(fn->id, fn);
  }
  if (break_ids.in(fn->id)) {
    printf("  break at function id %d\n", (int)fn->id);
    interrupted = 1;
  }
  ISlot **args = &valStack.v[valStack.n-nargs];
  if (!ip) {
    assert(!nargs);
    function = fn;
    ip = stmt = (Stmt*)fn->body->body->head->next;
  } else {
    IFrame *f = new IFrame(thread);
    f->init((Stmt*)fn->body->body->head->next);
    f->parent = this;
    f->function = fn;
    for (int i = 0; i < nargs; i++) {
      DefExpr *def = fn->formals->get(i + 1); // FORTRAN-style
      ISlot *slot = args[i], *arg_slot = new ISlot;
      *arg_slot = *slot;
      f->env.put(def->sym, arg_slot);
    }
    if (single_step == NEXT_STEP) {
      f->single_stepping = NEXT_STEP;
      single_step = NO_STEP;
    }
    thread->frame = f;
  }
  valStack.n -= (nargs + extra_args);
}

static void
user_error(IFrame *frame, char *fmt, ...) {
  BaseAST *ip = frame->ip;
  va_list args;
  int lineno = 0;
  char *filename = NULL;
  if (ip) {
    filename = ip->filename;
    lineno = ip->lineno;
  }
  printf("error: ");

  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
  
  printf("\n");

  if (filename || lineno) {
    if (filename)
      printf("at %s", filename);
    if (lineno) {
      if (filename)
        printf(":");
      else
        printf("at line ");
      printf("%d", lineno);
    }
    printf(" ");
  }

  printf("\n");

  if (run_interpreter > 1)
    error_interactive(frame);
  else {
    INT_FATAL("interpreter terminated");
  }
}

void
IFrame::icall(int nargs, ISlot *ret_slot) {
  if (!ret_slot)
    return_slot = islot(ip);
  else
    return_slot = ret_slot;
  if (valStack.n < nargs)
    INT_FATAL(ip, "not enough arguments for call");
  char *name = 0;
  if (nargs < 1)
    INT_FATAL(ip, "call with no arguments");
  int done = 0, extra_args = 0;
  do {
    ISlot **args = &valStack.v[valStack.n-nargs];
    if (args[0]->kind == SYMBOL_ISLOT && args[0]->symbol->astType == SYMBOL_FN) {
      icall((FnSymbol*)args[0]->symbol, nargs - 1, 1);
      return;
    } else if (args[0]->kind == SELECTOR_ISLOT) {
      name = args[0]->selector;
      done = 1;
      nargs--;
      extra_args = 1;
    } else if (args[0]->kind == CLOSURE_ISLOT) {
      INT_FATAL(ip, "closures not handled yet");
    } else {
      user_error(this, "call to something other than function name or closure");
      return;
    }
  } while (!done);
  Vec<FnSymbol *> visible;
  ip->parentScope->getVisibleFunctions(&visible, cannonicalize_string(name));
  if (visible.n != 1) {
    user_error(this, "unable to resolve call '%s' to a single function", name);
    return;
  }
  icall(visible.v[0], nargs, extra_args);
  return;
}

static void
interactive_usage() {
  fprintf(stdout, "chpl interpreter interactive mode commands:\n");
  fprintf(stdout, 
          "  step - single step\n"
          "  next - single step skipping over function calls\n"
          "  trace - trace program\n"
          "  where - show the expression/statement stack\n"
          "  stack - show the value stack\n"
          "  locals - show locals\n"
          "  print - print by id number or a local by name\n"
          "  nprint - print showing ids\n"
          "  info - information about breakpoints\n"
          "  bi - break at an id\n"
          "  birm - remove a break by id\n"
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
show(BaseAST *ip, int stage, int nospaces = 0) {
  if (!nospaces)
    printf("    ");
  printf("%s(%d)", astTypeName[ip->astType], (int)ip->id); 
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
  printf(" in %s(%d) %s:%d\n", 
         frame->function ? frame->function->name : "<initialization>",
         (int)(frame->function ? frame->function->id : 0),
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
print(ISlot *islot, int fnprint = 0) {
  switch (islot->kind) {
    default: INT_FATAL("interpreter: bad slot type: %d", islot->kind); break;
    case EMPTY_ISLOT: printf("<empty>"); break;
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
nprint(BaseAST *a) {
  if (IObject *o = dynamic_cast<IObject*>(a)) {
    o->print(1);
  } else
    nprint_view_noline(a);
}

static void
print(BaseAST *a, int fnprint = 0) {
  if (fnprint) {
    nprint(a);
    return;
  }
  if (IObject *o = dynamic_cast<IObject*>(a)) {
    o->print(0);
  } else
    print_view_noline(a);
}

void
IObject::print(int fnprint) {
  printf("Object: %d\n", (int)id);
  printf("  Type: "); 
  ::print(type, fnprint); 
  printf("\n");
  printf("  Members:\n");
  form_Map(MapElemBaseASTISlot, x, member) {
    if (Symbol *s = dynamic_cast<Symbol*>(x->key)) {
      printf("    ");
      s->print(stdout);
      printf("(%d) = ", (int)s->id);
      ::print(x->value, fnprint);
      printf("\n");
      known_ids.put(s->id, s);
    }
  }
  if (dim.n) {
    printf("  Array Dimension(s): ");
    for (int i = 0; i < dim.n; i++)
      printf("%d ", dim.v[i]);
    printf("\n");
    printf("  Array Element(s): %d\n", array.n);
  }
  printf("  Allocation Context:\n");
  forv_BaseAST(x, alloc_context)
    show(x, 0);
}

static void
cmd_stack(IFrame *frame) {
  if (!check_running(frame))
    return;
  printf("  value stack:\n");
  for (int i = frame->valStack.n-1; i >= 0; i--) {
    printf("    ");
    print(frame->valStack.v[i]);
    printf("\n");
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

static BaseAST *
get_known_id(int i) {
  BaseAST *p = known_ids.get(i);
  if (p)
    return p;
  Accum<BaseAST*> asts;
  forv_Vec(ModuleSymbol, mod, allModules)
    collect_ast_children(mod, asts, 1);
  forv_BaseAST(x, asts.asvec)
    known_ids.put(x->id, x);
  return known_ids.get(i);
}

static BaseAST *last_print = 0;

static void
cmd_print(IFrame *frame, char *c, int fnprint = 0) {
  skip_arg(c);
  BaseAST *p = NULL;
  if (!*c) {
    if (last_print)
      p = last_print;
    else {
      printf("  no previous print\n");
      return;
    }
  } else if (isdigit(*c)) {
    int i = atoi(c);
    if (i <= 0) {
      interactive_usage();
      return;
    }
    p = get_known_id(i);
    if (!p) {
      printf("  unknown id: %d\n", i);
      return;
    }
  } else {
    char *e = c;
    while (*e && !isspace(*e)) e++;
    *e = 0;
    form_Map(MapElemBaseASTISlot, x, frame->env) {
      if (Symbol *s = dynamic_cast<Symbol*>(x->key)) {
        if (s->name && !strcmp(s->name, c)) {
          p = s;
          goto Lfound;
        }
      }
    }
    printf("  unknown local: %s\n", c);
    return;
  Lfound:;
  }
  last_print = p;
  if (!fnprint)
    print(p);
  else
    nprint(p);
  printf("\n ");
  p->print(stdout);
  printf(" ");
  ISlot *ss = frame->env.get(p);
  if (ss) {
    printf("= ");
    print(ss);
    printf("\n");
  } else
    printf("\n");
}

static int 
compar_int(const void *ai, const void *aj) {
  int i = *(int*)ai;
  int j = *(int*)aj;
  if (i > j)
    return 1;
  else if (i < j)
    return -1;
  return 0;
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
      cmd_print(frame, c);
    } else if (match_cmd(c, "nprint")) {
      cmd_print(frame, c, 1);
    } else if (match_cmd(c, "where")) {
      cmd_where(frame);
    } else if (match_cmd(c, "stack")) {
      cmd_stack(frame);
    } else if (match_cmd(c, "locals")) {
      cmd_locals(frame);
    } else if (match_cmd(c, "bi")) {
      skip_arg(c);
      int i = atoi(c);
      if (i) {
        BaseAST *a = get_known_id(i);
        if (a) {
          break_ids.set_add(i);
          printf("  breaking at ");
          show(a, 0, 1);
        } else
          printf("  unable to break at unknown id %d", i);
      } else 
        printf("  please provide a valid id\n");
    } else if (match_cmd(c, "birm")) {
      skip_arg(c);
      int i = atoi(c);
      Vec<int> ids;
      ids.move(break_ids);
      int found = 0;
      for (int z = 0; z < ids.n; z++) {
        if (ids.v[z]) {
          if (i == ids.v[z]) {
            printf("  removing bi %d\n", i);
            found = 1;
          } else
            break_ids.set_add(ids.v[z]);
        }
      }
      if (!found)
        printf("  bi %d not found\n", i);
    } else if (match_cmd(c, "info")) {
      printf("  break ids:\n");
      Vec<int> ids;
      ids.copy(break_ids);
      ids.set_to_vec();
      qsort(ids.v, ids.n, sizeof(ids.v[0]), compar_int);
      for (int i = 0; i < ids.n; i++) {
        printf("    bi %d\n", ids.v[i]);
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

static void
error_interactive(IFrame *frame) {
  BaseAST *ip = frame->ip;
  while (1) {
    interactive(frame);
    if (frame->thread->frame && frame->ip == ip) 
      printf("  unable to continue from error\n");
    else
      break;
  }
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
  assert(__e); \
  if (expr && stage) { exprStack.add(expr); } \
  stageStack.add(stage + 1); \
  valStack.add(islot(__e)); \
  stageStack.add(1); exprStack.add(__e); stage = 0; expr = __e; \
} while (0)
#define EVAL_EXPR(_e) do { \
  Expr *__e = _e;  \
  assert(__e); \
  if (expr && stage) { exprStack.add(expr); }  \
  stageStack.add(stage + 1); \
  stageStack.add(1); exprStack.add(__e); stage = 0; expr = __e; \
} while (0)
#define EVAL_STMT(_s) do { stageStack.add(stage + 1); stmtStack.add(stmt); stmt = _s; } while (0)
#define PUSH_SELECTOR(_s) do { ISlot *_slot = new ISlot; _slot->set_selector(_s); valStack.add(_slot); } while (0)
#define PUSH_VAL(_s) valStack.add(islot(_s))
#define PUSH_SYM(_s)  do { ISlot *_slot = new ISlot; _slot->set_symbol(_s); valStack.add(_slot); } while (0)
#define POP_VAL(_s) do { *islot(_s) = *valStack.pop(); } while (0)
#define CALL(_n) do { icall(_n); return timeslice; } while (0)
#define CALL_RET(_n, _s) do { icall(_n, _s); return timeslice; } while (0)
#define CALL_PUSH(_n) do { ISlot *_slot = new ISlot; valStack.add(_slot); icall(_n, _slot); return timeslice; } while (0)

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

int
IFrame::igoto(Stmt *s) {
  Vec<Stmt *> parents;
  Stmt *ss = s;
  while (ss->parentStmt) {
    parents.add(ss->parentStmt);
    ss = ss->parentStmt;
  }
  parents.reverse();
  if (parents.n > stmtStack.n) {
    user_error(this, "goto target nested below source");
    return 1;
  }
  for (int i = 0; i < parents.n; i++)
    if (parents.v[i] != stmtStack.v[i]) {
      user_error(this, "goto target crosses nesting levels");
      return 1;
    }
  ss = stmt;
  Expr *defexpr = 0;
  while (ss) {
    if (ss->astType == STMT_EXPR) {
      if (ExprStmt *x = (ExprStmt *)(ss))
        if (x->expr->astType == EXPR_DEF)
          defexpr = x->expr;
    }
    if (defexpr && ss == s) {
      user_error(this, "goto over variable definition DefExpr(%d)", defexpr->id);
      return 1;
    }
    ss = (Stmt*)ss->next;
  }
  stage = 0;
  stmt = s;
  ip = expr = 0;
  valStack.clear();
  stageStack.n = parents.n;
  stmtStack.n = parents.n;
  return 0;
}

#define AT_LEAST -
static void
check_prim_args(CallExpr *s, int nargs) {
  int args = s->argList->length();
  int bad = 0;
  if (nargs < 0) {
    if (AT_LEAST nargs > args)
      bad = 1;
  } else
    bad = args != nargs;
  if (bad) {
    INT_FATAL(s, "interpreter: incorrect number of arguments (%d) to primitive '%s': expected %d", 
              args, s->primitive->interpreterOp->name, nargs);
  }
}

static void
get_context(IFrame *frame, Vec<BaseAST *> &context) {
  while (frame) {
    if (frame->expr) context.add(frame->expr);
    forv_Expr(e, frame->exprStack) context.add(frame->expr);
    if (frame->stmt) context.add(frame->stmt);
    forv_Stmt(e, frame->stmtStack) context.add(frame->stmt);
    frame = frame->parent;
  }
}

static TypeSymbol *
check_TypeSymbol(BaseAST *s, ISlot *slot) { 
  check_kind(s, slot, SYMBOL_ISLOT);
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(slot->symbol))
    return ts;
  USR_FATAL(s, "interpreter: non-TypeSymbol argument to primitive");
  return NULL;
}

void
IFrame::iprimitive(CallExpr *s) {
  int len = s->argList->length();
  if (!s->primitive->interpreterOp) {
    // INT_FATAL(ip, "interpreter: bad astType: %d", ip->astType);
    valStack.n -= len;
    islot(s)->kind = EMPTY_ISLOT;
    return;
  }
  ISlot **arg = &valStack.v[valStack.n-len];
  ISlot result;
  switch (s->primitive->interpreterOp->kind) {

    case PRIM_NONE:
      INT_FATAL(ip, "interpreter: prim type: %d", s->primitive->interpreterOp->kind);

    case PRIM_INIT: {
      check_prim_args(s, 1);
      if (arg[0]->kind != SYMBOL_ISLOT) {
        INT_FATAL(ip, "interpreter: non-symbol argument to INIT primitive: %d", arg[0]->kind);
      }
      TypeSymbol *ts = dynamic_cast<TypeSymbol*>(arg[0]->symbol);
      if (!ts) {
        INT_FATAL(ip, "interpreter: non-TypeSymbol argument to INIT primitive: %s", 
                  astTypeName[arg[0]->symbol->astType]);
      }
      Symbol *dv = ts->definition->defaultValue;
      if (dv) {
        if (VarSymbol *vs = dynamic_cast<VarSymbol*>(dv)) {
          if (vs->immediate) {
            result.set_immediate(vs->immediate);
            goto Lok;
          }
        }
        result.set_symbol(dv);
      Lok:;
      } else if (ts->definition->defaultConstructor) {
        return_slot = islot(s);
        icall(ts->definition->defaultConstructor);
        return;
      }
      break;
    }

    case PRIM_ALLOC: {
      check_prim_args(s, 1);
      TypeSymbol *ts = check_TypeSymbol(s, arg[0]);
      if (ClassType *ct = dynamic_cast<ClassType*>(ts->definition)) {
        result.kind = OBJECT_ISLOT;
        result.object = new IObject;
        result.object->type = ct;
        if (ct->isGeneric || ct->genericSymbols.n) {
          USR_FATAL(s, "interpreter: attempted ALLOC of generic ClassType");
        }
        forv_Symbol(s, ct->fields)
          result.object->member.put(s, new ISlot);
        get_context(this, result.object->alloc_context);
      } else {
        USR_FATAL(s, "interpreter: non-ClassType definition of TypeSymbol argument to ALLOC primitive");
      }
      break;
    }
      
    case PRIM_FOPEN:
    case PRIM_FCLOSE:
    case PRIM_STRERROR:
    case PRIM_FPRINTF:
    case PRIM_FSCANF:
      user_error(this, "unhandled primitive: %s", s->primitive->name);
      return;
    case PRIM_ARRAY_INDEX: {
      check_prim_args(s, AT_LEAST 2);
      check_kind(s, arg[0], OBJECT_ISLOT);
      IObject *a = arg[0]->object;
      check_prim_args(s, 1 + a->dim.n);
      int mult = 1, index = 0;
      for (int i = 0; i < a->dim.n; i++) {
        check_type(s, arg[1 + i], dtInteger);
        mult *= a->dim.v[i];
        index = arg[1 + i]->imm->v_int64 + mult * index;
      }
      assert(index < a->array.n);
      result = *a->array.v[index];
      break;
    }
    case PRIM_ARRAY_SET: {
      check_prim_args(s, AT_LEAST 3);
      check_kind(s, arg[0], OBJECT_ISLOT);
      IObject *a = arg[0]->object;
      check_prim_args(s, 2 + a->dim.n);
      int mult = 1, index = 0;
      for (int i = 0; i < a->dim.n; i++) {
        check_type(s, arg[1 + i], dtInteger);
        mult *= a->dim.v[i];
        index = arg[1 + i]->imm->v_int64 + mult * index;
      }
      assert(index < a->array.n);
      *a->array.v[index] = *arg[a->dim.n + 1];
      result = *arg[a->dim.n + 1];
      break;
    }
    case PRIM_CAST: {
      check_prim_args(s, 2);
      TypeSymbol *ts = check_TypeSymbol(s, arg[0]);
      (void)ts;
      switch (arg[1]->kind) {
        default:
          USR_FATAL(ip, "bad slot argument to CAST primitive: %s", 
                    slotKindName[arg[1]->kind]);
        case OBJECT_ISLOT:
          break;
        case IMMEDIATE_ISLOT:
          break;
      }
      break;
    }
    case PRIM_UNARY_MINUS:
    case PRIM_UNARY_PLUS:
    case PRIM_UNARY_BNOT:
    case PRIM_UNARY_NOT:
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
    case PRIM_DIV:
    case PRIM_MOD:
    case PRIM_EQUAL:
    case PRIM_NOTEQUAL:
    case PRIM_ELSSOREQUAL:
    case PRIM_GREATEROREQUAL:
    case PRIM_LESS:
    case PRIM_GREATER:
    case PRIM_AND:
    case PRIM_OR:
    case PRIM_XOR:
    case PRIM_LAND:
    case PRIM_LOR:
    case PRIM_EXP:
    case PRIM_PTR_EQ:
    case PRIM_PTR_NEQ:
    case PRIM_TO_STRING:
    case PRIM_COPY_STRING:
    case PRIM_STRING_INDEX:
    case PRIM_STRING_CONCAT:
    case PRIM_STRING_EQUAL:
    case PRIM_STRING_SELECT:
    case PRIM_STRING_STRIDED_SELECT:
    case PRIM_STRING_LENGTH:
    case PRIM_DONE: {
      user_error(this, "interpreter terminated: %s", s->primitive->name);
      return;
    }
      
  }
  valStack.n -= len;
  *islot(s) = result;
}

int
IFrame::run(int timeslice) {
  if (expr)
    goto LnextExpr;
  while (1) {
  LgotoLabel:
    if (timeslice && !--timeslice)
      return timeslice;
    if (break_ids.in(ip->id)) {
      printf("  break at id %d\n", (int)ip->id);
      interrupted = 1;
    }
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
            EVAL_EXPR(s->expr);
            break;
          case 1: {
            stage = 0;
            thread->frame = parent;
            if (thread->frame->single_stepping == NEXT_STEP)
              single_step = NEXT_STEP;
            if (parent)
              *parent->return_slot = *islot(s->expr);
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
            CALL_RET(2, islot(loop_var));
            break;
          case 2:
            PUSH_SELECTOR("_forall_valid");
            PUSH_VAL(iter);
            PUSH_VAL(loop_var);
            CALL_PUSH(3);
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
            CALL_RET(3, islot(indice));
            break;
          }
          case 4:
            EVAL_STMT(s->innerStmt);
            break;
          case 5:
            stage = 2;
            PUSH_SELECTOR("_forall_next");
            PUSH_VAL(iter);
            PUSH_VAL(loop_var);
            CALL_RET(3, islot(loop_var));
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
        if (igoto(s->label->defPoint->parentStmt))
          return timeslice;
        goto LgotoLabel;
      }
      case EXPR_SYM: {
        S(SymExpr);
        ISlot *x = env.get(s->var);
        if (!x) {
          switch (s->var->astType) {
            case SYMBOL_UNRESOLVED:
              env.put(s->var, (x = new ISlot(s->var->name)));
              break;
            case SYMBOL_FN: 
            case SYMBOL_TYPE:
              env.put(s->var, (x = new ISlot(s->var)));
              break;
            case SYMBOL_VAR: {
              VarSymbol *v = (VarSymbol*)s->var;
              if (v->immediate) {
                env.put(s->var, (x = new ISlot(v->immediate)));
                break;
              }
            }
              // fall through  
            default:
              USR_FATAL(ip, "unknown variable in SymExpr '%s'", s->var->name ? s->var->name : "");
              break;
          }
        }
        assert(x);
        *islot(s) = *x;
        break;
      }
      case EXPR_DEF: {
        S(DefExpr);
        ISlot *slot = new ISlot;
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
              env.put(expr, islot(s->elseExpr));
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
                if (!s->primitive)
                  PUSH_EXPR(s->baseExpr);
                break;
              case OP_MOVE:
                if (s->argList->length() != 2) {
                  INT_FATAL("illegal number of arguments for MOVE %d\n", s->argList->length());
                }
                stage = 2;
                break;
            }
            break;
          }
          default:
            if (stage - 1 <= s->argList->length()) {
              PUSH_EXPR(s->argList->get(stage - 1));
            } else {
              stage = 0;
              if (s->primitive)
                iprimitive(s);
              else if (s->opTag == OP_MOVE) {
                Expr *a = s->argList->get(1);
                if (a->astType == EXPR_SYM)
                  POP_VAL(((SymExpr*)a)->var);
                else {
                  INT_FATAL("target of MOVE not an SymExpr, astType = %d\n", s->argList->get(1)->astType);
                }
              } else
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
      } else if (!ip) {
        assert(!expr);
        ip = stmt;
      }
      assert((!expr || expr == ip) && (expr || ip == stmt));
      assert(stageStack.n == exprStack.n + stmtStack.n + (expr ? 1 : 0));
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
  if (run_interpreter > 1)
    interrupted = 1;
  do {
    runProgram();
    chpl_interpreter();
    printf("  program terminated\n");
    while (!threads.n) 
      interactive(0);
  } while (run_interpreter > 1);
  exit(0);
}

void 
init_interpreter() {
  init_interpreter_op = new InterpreterOp("init", PRIM_INIT);
  alloc_interpreter_op = new InterpreterOp("alloc", PRIM_ALLOC);
  fopen_interpreter_op = new InterpreterOp("fopen", PRIM_FOPEN);
  fclose_interpreter_op = new InterpreterOp("fclose", PRIM_FCLOSE);
  strerror_interpreter_op = new InterpreterOp("strerror", PRIM_STRERROR);
  fprintf_interpreter_op = new InterpreterOp("fprintf", PRIM_FPRINTF);
  fscanf_interpreter_op = new InterpreterOp("fscanf", PRIM_FSCANF);
  array_index_interpreter_op = new InterpreterOp("array_index", PRIM_ARRAY_INDEX);
  array_set_interpreter_op = new InterpreterOp("array_set", PRIM_ARRAY_SET);
  unary_minus_interpreter_op = new InterpreterOp("unary_minus", PRIM_UNARY_MINUS);
  unary_plus_interpreter_op = new InterpreterOp("unary_plus", PRIM_UNARY_PLUS);
  unary_bnot_interpreter_op = new InterpreterOp("unary_bnot", PRIM_UNARY_BNOT);
  unary_not_interpreter_op = new InterpreterOp("unary_not", PRIM_UNARY_NOT);
  add_interpreter_op = new InterpreterOp("add", PRIM_ADD);
  subtract_interpreter_op = new InterpreterOp("subtract", PRIM_SUBTRACT);
  mult_interpreter_op = new InterpreterOp("mult", PRIM_MULT);
  div_interpreter_op = new InterpreterOp("div", PRIM_DIV);
  mod_interpreter_op = new InterpreterOp("mod", PRIM_MOD);
  equal_interpreter_op = new InterpreterOp("equal", PRIM_EQUAL);
  notequal_interpreter_op = new InterpreterOp("notequal", PRIM_NOTEQUAL);
  elssorequal_interpreter_op = new InterpreterOp("elssorequal", PRIM_ELSSOREQUAL);
  greaterorequal_interpreter_op = new InterpreterOp("greaterorequal", PRIM_GREATEROREQUAL);
  less_interpreter_op = new InterpreterOp("less", PRIM_LESS);
  greater_interpreter_op = new InterpreterOp("greater", PRIM_GREATER);
  and_interpreter_op = new InterpreterOp("and", PRIM_AND);
  or_interpreter_op = new InterpreterOp("or", PRIM_OR);
  xor_interpreter_op = new InterpreterOp("xor", PRIM_XOR);
  land_interpreter_op = new InterpreterOp("land", PRIM_LAND);
  lor_interpreter_op = new InterpreterOp("lor", PRIM_LOR);
  exp_interpreter_op = new InterpreterOp("exp", PRIM_EXP);
  ptr_eq_interpreter_op = new InterpreterOp("ptr_eq", PRIM_PTR_EQ);
  ptr_neq_interpreter_op = new InterpreterOp("ptr_neq", PRIM_PTR_NEQ);
  cast_interpreter_op = new InterpreterOp("cast", PRIM_CAST);
  to_string_interpreter_op = new InterpreterOp("to_string", PRIM_TO_STRING);
  copy_string_interpreter_op = new InterpreterOp("copy_string", PRIM_COPY_STRING);
  string_index_interpreter_op = new InterpreterOp("string_index", PRIM_STRING_INDEX);
  string_concat_interpreter_op = new InterpreterOp("string_concat", PRIM_STRING_CONCAT);
  string_equal_interpreter_op = new InterpreterOp("string_equal", PRIM_STRING_EQUAL);
  string_select_interpreter_op = new InterpreterOp("string_select", PRIM_STRING_SELECT);
  string_strided_select_interpreter_op = new InterpreterOp("string_strided_select", PRIM_STRING_STRIDED_SELECT);
  string_length_interpreter_op = new InterpreterOp("string_length", PRIM_STRING_LENGTH);
  done_interpreter_op = new InterpreterOp("done", PRIM_DONE);
}
