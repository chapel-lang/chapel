#include <stdio.h>
#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "files.h"
#include "mysystem.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

// convert functions that return records into functions that take
// those records as a reference argument and fix calls
void
convertReturnsToArgs() {
  return;
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  // convert calls to pass returned record by reference, handling the
  // case where the returned record is ignored
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (!call->primitive && call->isResolved()) {
        FnSymbol* fn = call->isResolved();
        ClassType* ct = dynamic_cast<ClassType*>(fn->retType);
        if (ct && ct->classTag != CLASS_CLASS) {
          if (!call->parentExpr) {
            VarSymbol* tmp = new VarSymbol("_ignored_ret", ct);
            tmp->isCompilerTemp = true;
            call->parentStmt->insertBefore(new DefExpr(tmp));
            call->insertAtTail(tmp);
          } else {
            CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr);
            if (!move || !move->isPrimitive(PRIMITIVE_MOVE))
              INT_FATAL(call, "Call is not in move");
            call->insertAtTail(move->argList->get(1)->remove());
            move->replace(call->remove());
          }
        }
      }
    }
  }

  // convert functions to take returned record by reference
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      ClassType* ct = dynamic_cast<ClassType*>(fn->retType);
      if (ct && ct->classTag != CLASS_CLASS) {
        ArgSymbol* arg = new ArgSymbol(INTENT_REF, "_ret", ct);
        fn->formals->insertAtTail(arg);
        fn->retType = dtVoid;
        ReturnStmt* ret = dynamic_cast<ReturnStmt*>(fn->body->body->last());
        ret->insertBefore(new CallExpr(PRIMITIVE_MOVE, arg, ret->expr->copy()));
        ret->expr->replace(new SymExpr(gVoid));
      }
    }
  }
}


static int max(int a, int b) {
  return (a >= b) ? a : b;
}

static void setOrder(Map<ClassType*,int>& order, int& maxOrder, ClassType* ct);

static int
getOrder(Map<ClassType*,int>& order, int& maxOrder,
         ClassType* ct, Vec<ClassType*>& visit) {
  if (visit.set_in(ct))
    return 0;
  visit.set_add(ct);
  if (order.get(ct))
    return order.get(ct);
  int i = 0;
  for_fields(field, ct) {
    if (ClassType* fct = dynamic_cast<ClassType*>(field->type)) {
      if (!visit.set_in(fct) && fct->classTag != CLASS_CLASS)
        setOrder(order, maxOrder, fct);
      i = max(i, getOrder(order, maxOrder, fct, visit));
    }
  }
  return i + (ct->classTag != CLASS_CLASS);
}


static void
setOrder(Map<ClassType*,int>& order, int& maxOrder,
         ClassType* ct) {
  if (ct->classTag == CLASS_CLASS || order.get(ct))
    return;
  int i;
  Vec<ClassType*> visit;
  i = getOrder(order, maxOrder, ct, visit);
  order.put(ct, i);
  if (i > maxOrder)
    maxOrder = i;
}


// _MOVE(USE, SYM)
static void handleMUS(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(2)->remove());
  int i = 1;
  forv_Vec(Symbol, sym, syms) {
    if (rhs->parentSymbol) {
      rhs = rhs->copy();
      rhs->var->uses.add(rhs);
    }
    call->parentStmt->insertBefore(
      new CallExpr(PRIMITIVE_MOVE, sym,
        new CallExpr(PRIMITIVE_GET_MEMBER, rhs, ct->fields->get(i)->sym)));
    i++;
  }
  call->parentStmt->remove();
}

// _MOVE(SYM, USE)
static void handleMSU(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1)->remove());
  int i = 1;
  forv_Vec(Symbol, sym, syms) {
    if (lhs->parentSymbol) {
      lhs = lhs->copy();
      lhs->var->uses.add(lhs);
    }
    call->parentStmt->insertBefore(
      new CallExpr(PRIMITIVE_SET_MEMBER, lhs, ct->fields->get(i)->sym, sym));
    i++;
  }
  call->parentStmt->remove();
}

// _SET(USE, SYM, SYM)
static void handleSUSS(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* setter = dynamic_cast<SymExpr*>(call->get(2));
  int i = 0;
  for_fields(field, ct) {
    if (!strcmp(setter->var->name, field->name)) {
      call->parentStmt->insertBefore(
        new CallExpr(PRIMITIVE_MOVE, syms.v[i], call->get(3)->remove()));
      call->parentStmt->remove();
      break;
    }
    i++;
  }
}

// _SET(SYM, USE, SYM)
static void handleSSUS(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* _this = dynamic_cast<SymExpr*>(call->get(1));
  SymExpr* rhs = dynamic_cast<SymExpr*>(call->get(3));
  _this->remove();
  rhs->remove();
  int i = 1;
  forv_Vec(Symbol, sym, syms) {
    if (_this->parentSymbol) {
      _this = _this->copy();
      _this->var->uses.add(_this);
    }
    if (rhs->parentSymbol) {
      rhs = rhs->copy();
      rhs->var->uses.add(rhs);
    }
    call->parentStmt->insertBefore(
      new CallExpr(PRIMITIVE_SET_MEMBER, _this, sym,
        new CallExpr(PRIMITIVE_GET_MEMBER, rhs, ct->fields->get(i)->sym)));
    i++;
  }
  call->parentStmt->remove();
}

// _SET(*, *, USE)
static void handleSSSU(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* _this = dynamic_cast<SymExpr*>(call->get(1));
  SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(2));
  _this->remove();
  lhs->remove();
  int i = 1;
  forv_Vec(Symbol, sym, syms) {
    if (_this->parentSymbol) {
      _this = _this->copy();
      _this->var->uses.add(_this);
    }
    if (lhs->parentSymbol) {
      lhs = lhs->copy();
      lhs->var->uses.add(lhs);
    }
    call->parentStmt->insertBefore(
      new CallExpr(PRIMITIVE_SET_MEMBER, _this,
        new CallExpr(PRIMITIVE_GET_MEMBER, lhs, ct->fields->get(i)->sym), sym));
    i++;
  }
  call->parentStmt->remove();
}

// _GET(USE, SYM)
static void handleGUS(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  SymExpr* getter = dynamic_cast<SymExpr*>(call->get(2));
  int i = 0;
  for_fields(field, ct) {
    if (!strcmp(getter->var->name, field->name)) {
      call->replace(new SymExpr(syms.v[i]));
      break;
    }
    i++;
  }
}

// _GET(SYM, USE) in _MOVE(SYM, _GET(SYM, USE))
static void handleGSU(ClassType* ct, Vec<Symbol*>& syms, CallExpr* call) {
  CallExpr* move = dynamic_cast<CallExpr*>(call->parentExpr);
  if (!move || !move->isPrimitive(PRIMITIVE_MOVE))
    INT_FATAL(call, "unexpected case in handleGSU");
  SymExpr* lhs = dynamic_cast<SymExpr*>(move->get(1)->remove());
  SymExpr* _this = dynamic_cast<SymExpr*>(call->get(1)->remove());
  int i = 1;
  forv_Vec(Symbol, sym, syms) {
    if (lhs->parentSymbol) {
      lhs = lhs->copy();
      lhs->var->uses.add(lhs);
    }
    if (_this->parentSymbol) {
      _this = _this->copy();
      _this->var->uses.add(_this);
    }
    move->parentStmt->insertBefore(
      new CallExpr(PRIMITIVE_MOVE,
        new CallExpr(PRIMITIVE_GET_MEMBER, lhs, ct->fields->get(i)->sym),
        new CallExpr(PRIMITIVE_GET_MEMBER, _this, sym)));
    i++;
  }
  call->parentStmt->remove();
}

static void
makeScalarReplacements(ClassType* ct, Vec<Symbol*>& syms, DefExpr* def) {
  ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def->sym);
  VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
  for_fields(field, ct) {
    char* name = stringcat(def->sym->name, "_", field->name);
    if (var) {
      VarSymbol* sym = new VarSymbol(name, field->type);
      sym->isCompilerTemp = def->sym->isCompilerTemp;
      syms.add(sym);
      if (def->parentStmt)
        def->parentStmt->insertBefore(new DefExpr(sym));
      else
        def->insertBefore(new DefExpr(sym));
    } else if (arg) {
      ArgSymbol* sym = new ArgSymbol(arg->intent, name, field->type);
      if (!strcmp(arg->name, "this"))
        arg->intent = INTENT_REF;
      syms.add(sym);
      def->insertBefore(new DefExpr(sym));
    }
  }
  if (def->parentStmt)
    def->parentStmt->remove();
  else
    def->remove();
}

static void
scalarReplaceSyms(Vec<Symbol*>& symbols) {
  forv_Vec(Symbol, symbol, symbols) {
    ClassType* ct = dynamic_cast<ClassType*>(symbol->type);
    DefExpr* def = symbol->defPoint;
    Vec<Symbol*> syms;
    makeScalarReplacements(ct, syms, def);
    forv_Vec(SymExpr, use, def->sym->uses) {
      if (!use->parentSymbol)
        continue;
      CallExpr* call = dynamic_cast<CallExpr*>(use->parentExpr);
      if (!call)
        INT_FATAL(use, "unexpected use");
      if (!call->primitive) { // handle function call
        forv_Vec(Symbol, sym, syms)
          use->insertBefore(new SymExpr(sym));
        use->remove();
        continue;
      }
      if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (use == call->get(1)) {
          if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
            if (rhs && rhs->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
              call->parentStmt->remove();
            } else {
              INT_FATAL("unexpected use");
            }
          } else if (dynamic_cast<SymExpr*>(call->get(2))) {
            handleMUS(ct, syms, call);
          } else {
            INT_FATAL("unexpected use");
          }
        } else if (use == call->get(2)) {
          handleMSU(ct, syms, call);
        } else {
          INT_FATAL("unexpected use");
        }
      } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
        if (call->get(1) == use) {
          handleSUSS(ct, syms, call);
        } else if (call->get(2) == use) {
          handleSSUS(ct, syms, call);
        } else if (call->get(3) == use) {
          handleSSSU(ct, syms, call);
        } else {
          INT_FATAL(use, "unexpected use");
        }
      } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        if (call->get(1) == use) {
          handleGUS(ct, syms, call);
        } else if (call->get(2) == use) {
          handleGSU(ct, syms, call);
        } else {
          INT_FATAL(use, "unexpected use");
        }
      } else {
        INT_FATAL(use, "unexpected use");
      }
    }
  }
}


static void
scalarReplace(Vec<ClassType*>& typeSet) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  compute_sym_uses();

  Vec<Symbol*> fields, vars, args;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      ClassType* ct = dynamic_cast<ClassType*>(def->sym->type);
      if (!typeSet.set_in(ct))
        continue;
      ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def->sym);
      VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
      if (var && !var->defPoint->parentStmt)
        fields.add(var);
      else if (var)
        vars.add(var);
      else if (arg)
        args.add(arg);
    }
  }
  Vec<Symbol*> syms;
  syms.append(fields);
  syms.append(vars);
  syms.append(args);
  scalarReplaceSyms(syms);
}


void
destructureRecords() {
  if (no_scalar_replacement)
    return;

  Vec<BaseAST*> asts;
  collect_asts(&asts);

  // order types that should be scalar replaced
  //  e.g. replace ((int,int),string) before (int,int)
  Map<ClassType*,int> order;
  int maxOrder = 0;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym))
        if (ClassType* ct = dynamic_cast<ClassType*>(ts->type))
          setOrder(order, maxOrder, ct);
  }

  // iteratively call scalar replace, respecting type ordering
  Vec<ClassType*> keys;
  order.get_keys(keys);
  for (int i = maxOrder; i >= 1; i--) {
    Vec<ClassType*> typeSet;
    bool nonEmpty = false;
    forv_Vec(ClassType, key, keys) {
      if (order.get(key) == i && key->symbol->hasPragma("destructure")) {
        nonEmpty = true;
        typeSet.set_add(key);
      }
    }
    if (nonEmpty)
      scalarReplace(typeSet);
  }

  // inline constructors for scalar replaced types
  forv_Vec(ClassType, key, keys) {
    key->defaultConstructor->addPragma("inline");
  }
}


#define STRSUB(x)                               \
  *ch = '\0';                                   \
  sym->cname = stringcat(sym->cname, x, ch+1);  \
  ch = sym->cname

static void legalizeCName(Symbol* sym) {
  for (char* ch = sym->cname; *ch != '\0'; ch++) {
    switch (*ch) {
    case '>': STRSUB("_GREATER_"); break;
    case '<': STRSUB("_LESS_"); break;
    case '=': STRSUB("_EQUAL_"); break;
    case '*': STRSUB("_ASTERISK_"); break;
    case '/': STRSUB("_SLASH_"); break;
    case '%': STRSUB("_PERCENT_"); break;
    case '+': STRSUB("_PLUS_"); break;
    case '-': STRSUB("_HYPHEN_"); break;
    case '^': STRSUB("_CARET_"); break;
    case '&': STRSUB("_AMPERSAND_"); break;
    case '|': STRSUB("_BAR_"); break;
    case '!': STRSUB("_EXCLAMATION_"); break;
    case '#': STRSUB("_POUND_"); break;
    case '?': STRSUB("_QUESTION_"); break;
    case '~': STRSUB("_TILDA_"); break;
    default: break;
    }
  }
}

static void codegen_header(void) {
  ChainHashMap<char*, StringHashFns, int> cnames;
  Vec<TypeSymbol*> typeSymbols;
  Vec<FnSymbol*> fnSymbols;
  Vec<VarSymbol*> varSymbols;

  chpl_main->addPragma("rename _chpl_main");

  cnames.put("abs", 1);
  cnames.put("_init", 1);
  cnames.put("stdin", 1);
  cnames.put("close", 1);
  cnames.put("fwrite", 1);
  cnames.put("fread", 1);
  cnames.put("main", 1);
  cnames.put("open", 1);
  cnames.put("printf", 1);
  cnames.put("quad", 1);
  cnames.put("read", 1);
  cnames.put("sleep", 1);
  cnames.put("stderr", 1);
  cnames.put("stdout", 1);
  cnames.put("write", 1);
  cnames.put("y1", 1); // this is ridiculous...

  // Remove functions not used (misses mutual recursion)
  compute_call_sites();
  Vec<FnSymbol*> all_functions;
  collect_functions(&all_functions);
  forv_Vec(FnSymbol, fn, all_functions) {
    if (fn->calledBy->n == 0 && chpl_main != fn)
      fn->defPoint->parentStmt->remove();
  }

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    Symbol* sym = dynamic_cast<Symbol*>(ast);
    if (!sym)
      continue;

    if (sym->name == sym->cname)
      sym->cname = stringcpy(sym->name);

    if (char* pragma = sym->hasPragmaPrefix("rename"))
      sym->cname = stringcpy(pragma+7);

    if (VarSymbol* vs = dynamic_cast<VarSymbol*>(ast))
      if (vs->immediate)
        continue;

    if (sym->parentScope == rootScope)
      continue;

    if (sym->hasPragma("no codegen"))
      continue;

    legalizeCName(sym);

    // mangle symbol that is neither field nor formal if the symbol's
    // name has already been encountered
    if (!dynamic_cast<ArgSymbol*>(sym) &&
        !dynamic_cast<UnresolvedSymbol*>(sym) &&
        !dynamic_cast<ClassType*>(sym->parentScope->astParent) &&
        cnames.get(sym->cname))
      sym->cname = stringcat("_", intstring(sym->id), "_", sym->cname);

    cnames.put(sym->cname, 1);

    if (TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(sym)) {
      typeSymbols.add(typeSymbol);
    } else if (FnSymbol* fnSymbol = dynamic_cast<FnSymbol*>(sym)) {
      fnSymbols.add(fnSymbol);
    } else if (VarSymbol* varSymbol = dynamic_cast<VarSymbol*>(sym)) {
      if (dynamic_cast<ModuleSymbol*>(varSymbol->parentScope->astParent))
        varSymbols.add(varSymbol);
    }
  }

  FILE* outfile = openCFile("_chpl_header.h");
  fprintf(outfile, "#include \"stdchpl.h\"\n");
  fprintf(outfile, "\n");

  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    typeSymbol->codegenPrototype(outfile);
  }

  // codegen enumerated types
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (dynamic_cast<EnumType*>(typeSymbol->type))
      typeSymbol->codegenDef(outfile);
  }

  // order records/unions topologically
  //   (int, int) before (int, (int, int))
  Map<ClassType*,int> order;
  int maxOrder = 0;
  forv_Vec(TypeSymbol, ts, typeSymbols) {
    if (ClassType* ct = dynamic_cast<ClassType*>(ts->type))
      setOrder(order, maxOrder, ct);
  }

  // debug
//   for (int i = 0; i < order.n; i++) {
//     if (order.v[i].key && order.v[i].value) {
//       printf("%d: %s\n", order.v[i].value, order.v[i].key->symbol->name);
//     }
//   }
//   printf("%d\n", maxOrder);

  // codegen records/unions in topological order
  Vec<ClassType*> keys;
  order.get_keys(keys);
  for (int i = 1; i <= maxOrder; i++) {
    forv_Vec(ClassType, key, keys) {
      if (order.get(key) == i)
        key->symbol->codegenDef(outfile);
    }
  }

  // codegen remaining types
  forv_Vec(TypeSymbol, typeSymbol, typeSymbols) {
    if (ClassType* ct = dynamic_cast<ClassType*>(typeSymbol->type))
      if (ct->classTag != CLASS_CLASS)
        continue;
    if (dynamic_cast<EnumType*>(typeSymbol->type))
      continue;
    typeSymbol->codegenDef(outfile);
  }

  forv_Vec(FnSymbol, fnSymbol, fnSymbols) {
    fnSymbol->codegenPrototype(outfile);
  }
  forv_Vec(VarSymbol, varSymbol, varSymbols) {
    varSymbol->codegenDef(outfile);
  }
  closeCFile(outfile);
}


static void
codegen_config() {
  fileinfo outfileinfo;
  char* rtconfigFile = "rtconfig";
  openCFiles(rtconfigFile, &outfileinfo);
  fprintf(codefile, "#include \"stdchpl.h\"\n\n");
  fprintf(codefile, "void CreateConfigVarTable(void) {\n");
  fprintf(codefile, "initConfigVarTable();\n");

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      codefile = outfileinfo.fptr;
      VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym);
      if (var && var->varClass == VAR_CONFIG) {
        fprintf(codefile, "installConfigVar(\"%s\", \"", var->name);
        fprintf(codefile, var->type->symbol->name);
        fprintf(codefile, "\", \"%s\");\n", var->getModule()->name);
      }
    }
  }

  codefile = outfileinfo.fptr;
  fprintf(codefile, "if (askedToParseArgs()) {\n");
  fprintf(codefile, "  parseConfigArgs();\n");
  fprintf(codefile, "}\n");

  fprintf(codefile, "if (askedToPrintHelpMessage()) {\n");
  fprintf(codefile, "  printHelpTable();\n");
  fprintf(codefile, "  printConfigVarTable();\n");
  fprintf(codefile, "}\n");
  
  fprintf(codefile, "}\n");
  closeCFiles(&outfileinfo);
}


void codegen(void) {
  if (no_codegen)
    return;

  codegen_header();

  openMakefile(allModules.v[0]->filename, system_dir);

  codegen_config();

  forv_Vec(ModuleSymbol, currentModule, allModules) {
    if (!currentModule->hasPragma("no codegen")) {
      mysystem(stringcat("# codegen-ing module", currentModule->name),
               "generating comment for --print-commands option");
      currentModule->codegenDef();
    }
  }

  closeMakefile();
  makeBinary();
}
