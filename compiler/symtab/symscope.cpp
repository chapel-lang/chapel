#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symscope.h"
#include "symtab.h"
#include "symtabTraversal.h"
#include "if1.h"
#include "../passes/filesToAST.h"
#include "files.h"

#define OPERATOR_CHAR(_c) \
(((_c > ' ' && _c < '0') || (_c > '9' && _c < 'A') || \
  (_c > 'Z' && _c < 'a') || (_c > 'z')) &&            \
   _c != '_'&& _c != '?' && _c != '$')                \


SymScope::SymScope(scopeType init_type) :
  type(init_type),
  lookupCache(NULL),
  stmtContext(NULL),
  symContext(NULL),
  exprContext(NULL),
  parent(NULL),
  child(NULL),
  sibling(NULL)
{
  symbols.clear();
  visibleFunctions.clear();
}


void SymScope::setContext(Stmt* stmt, Symbol* sym, Expr* expr) {
  stmtContext = stmt;
  symContext = sym;
  exprContext = expr;
}


void SymScope::traverse(SymtabTraversal* traversal) {
  SymScope* prevScope = Symboltable::setCurrentScope(this);

  traversal->preProcessScope(this);

  forv_Vec(Symbol, sym, symbols) {
    Symbol* overload = sym;
    while (overload) {
      Symbol* next = overload->overload;
      traversal->processSymbol(overload);
      overload = next;
    }
  }

  traversal->postProcessScope(this);

  Symboltable::setCurrentScope(prevScope);
}


bool SymScope::isEmpty(void) {
  return symbols.n == 0;
}


void SymScope::insert(Symbol* sym) {
  table.put(sym->name, sym);
  sym->setParentScope(this);
  symbols.add(sym);
}


void SymScope::remove(Symbol* sym) {
  for (int i = 0; i < symbols.n; i++) {
    if (symbols.v[i]) {
      if (symbols.v[i] == sym) {
        symbols.v[i] = sym->overload;
        return;
      } else {
        Symbol* tmp = symbols.v[i];
        while (tmp->overload) {
          if (tmp->overload == sym) {
            tmp->overload = sym->overload;
            return;
          }
          tmp = tmp->overload;
        }
      }
    }
  }
  INT_FATAL(sym, "Symbol not found in scope from which deleted");
}


ModuleSymbol* SymScope::getModule() {
  if (type <= SCOPE_MODULE) {
    return dynamic_cast<ModuleSymbol*>(symContext);
  }

  if (!parent) {
    INT_FATAL("Scope not in module");
  }

  return parent->getModule();
}


Symbol* SymScope::findEnclosingSymContext() {
  if (symContext) {
    return symContext;
  }
  else if (parent == NULL) {
    return NULL;
  }
  else {
    return parent->findEnclosingSymContext();
  }
}


Stmt* SymScope::findEnclosingStmtContext() {
  if (stmtContext) {
    return stmtContext;
  }
  else if (parent == NULL) {
    return NULL;
  }
  else {
    return parent->findEnclosingStmtContext();
  }
}


Expr* SymScope::findEnclosingExprContext() {
  if (exprContext) {
    return exprContext;
  }
  else if (parent == NULL) {
    return NULL;
  }
  else {
    return parent->findEnclosingExprContext();
  }
}


int SymScope::parentLength(void) {
  if (!parent) {
    return 0;
  } else {
    return parent->parentLength() + 1;
  }
}


char* SymScope::indentStr(void) {
  static char* spaces = "                                                     "
                        "                                                     ";
  int printLevel = parentLength();
  int maxspaces = strlen(spaces);
  int offset = maxspaces-(2*printLevel);
  if (offset < 0) {
    offset = 0;
  }

  return spaces + offset;
}


static bool printEmpty = false;


void SymScope::printHeader(FILE* outfile) {
  char* indent = indentStr();
  fprintf(outfile, "%s======================================================\n",
          indent);
  fprintf(outfile, "%sSCOPE: ", indent);
  switch (type) {
  case SCOPE_INTRINSIC:
    fprintf(outfile, "intrinsic");
    break;
  case SCOPE_PRELUDE:
    fprintf(outfile, "prelude");
    break;
  case SCOPE_MODULE:
    fprintf(outfile, "module");
    break;
  case SCOPE_PARAM:
    fprintf(outfile, "parameters");
    break;
  case SCOPE_FUNCTION:
    fprintf(outfile, "function");
    break;
  case SCOPE_LOCAL:
    fprintf(outfile, "local");
    break;
  case SCOPE_FORLOOP:
    fprintf(outfile, "for loop");
    break;
  case SCOPE_FORALLEXPR:
    fprintf(outfile, "forall expression");
    break;
  case SCOPE_LETEXPR:
    fprintf(outfile, "let expression");
    break;
  case SCOPE_CLASS:
    fprintf(outfile, "class");
    break;
  case SCOPE_POSTPARSE:
    fprintf(outfile, "post parsing");
    break;
  }
  BaseAST* scopeLoc = NULL;
  if (symContext) {
    fprintf(outfile, " ");
    symContext->print(outfile);
    scopeLoc = symContext;
  } else if (exprContext) {
    scopeLoc = exprContext;
  } else if (stmtContext) {
    scopeLoc = stmtContext;
  }
  if (scopeLoc) {
    fprintf(outfile, " (%s)", scopeLoc->stringLoc());
  }
  fprintf(outfile, "\n");

  fprintf(outfile, "%s------------------------------------------------------\n",
          indent);
}


void SymScope::printSymbols(FILE* outfile, bool tableOrder) {
  char* indent = indentStr();

  if (tableOrder) {
    int i;
    Vec<Symbol*> symlist;

    table.get_values(symlist);
    for (i=0; i<symlist.n; i++) {
      fprintf(outfile, "%s", indent);
      symlist.v[i]->print(outfile);
      fprintf(outfile, "\n");
    }
  } else {
    if (!isEmpty()) {
      fprintf(outfile, "%s", indent);
      forv_Vec(Symbol, sym, symbols) {
        sym->print(outfile);
        fprintf(outfile, "L\n%s", indent);
      }
      fprintf(outfile, "\n");
    }
  }
}


void SymScope::printFooter(FILE* outfile) {
  char* indent = indentStr();
  fprintf(outfile, "%s======================================================\n",
          indent);
}


void SymScope::print(FILE* outfile, bool tableOrder) {
  if (!isEmpty() && !printEmpty) {
    printHeader(outfile);
    printSymbols(outfile, tableOrder);
    printFooter(outfile);
  }
}

void SymScope::codegen(FILE* outfile, char* separator) {
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (!dynamic_cast<TypeSymbol*>(tmp)) {
        tmp->codegenDef(outfile);
      }
    }
  }
}


void SymScope::codegenFunctions(FILE* outfile) {
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (dynamic_cast<TypeSymbol*>(tmp)) {
        tmp->type->codegenStringToType(codefile);
        tmp->type->codegenConfigVarRoutines(codefile);
      }
    }
  }
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (dynamic_cast<FnSymbol*>(tmp)) {
        tmp->codegenDef(outfile);
      }
    }
  }
}


// hack to avoid static on commonModule until module use is supported
bool SymScope::commonModuleIsFirst() {
  if (type == SCOPE_MODULE) {
    return commonModule->modScope == this;
  } else {
    return parent->commonModuleIsFirst();
  }
}


static void
addVisibleFunctionsHelper(Map<char*,Vec<FnSymbol*>*>* visibleFunctions,
                          FnSymbol* fn) {
  int is_setter = (fn->name[0] == '=' && !OPERATOR_CHAR(fn->name[1]));
  char *n = if1_cannonicalize_string(if1, fn->name + (is_setter ? 1 : 0));
  Vec<FnSymbol*>* fs = visibleFunctions->get(n);
  if (!fs) fs = new Vec<FnSymbol*>;
  fs->add(fn);
  visibleFunctions->put(n, fs);
}


static void
addVisibleFunctions(Map<char*,Vec<FnSymbol*>*>* visibleFunctions,
                    Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (!fn->typeBinding) {
      addVisibleFunctionsHelper(visibleFunctions, fn);
    }
  }
  if (sym->overload) {
    addVisibleFunctions(visibleFunctions, sym->overload);
  }
  if (ForwardingSymbol* forward = dynamic_cast<ForwardingSymbol*>(sym)) {
    addVisibleFunctions(visibleFunctions, forward->forward);
  }
}


void SymScope::setVisibleFunctions(Vec<FnSymbol*>* moreVisibleFunctions) {
  visibleFunctions.clear();

  forv_Vec(Symbol, sym, symbols) {
    if (sym) {
      if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym->getSymbol())) {
        if (!dynamic_cast<ClassType*>(typeSym->type)) {
          forv_Vec(FnSymbol, method, typeSym->type->methods) {
            while (method) {
              addVisibleFunctionsHelper(&visibleFunctions, method);
              method = dynamic_cast<FnSymbol*>(method->overload);
            }
          }
          FnSymbol* constructor = typeSym->type->defaultConstructor;
          while (constructor) {
            addVisibleFunctionsHelper(&visibleFunctions, constructor);
            constructor = dynamic_cast<FnSymbol*>(constructor->overload);
          }
        }
      } else {
        addVisibleFunctions(&visibleFunctions, sym);
      }
    }
  }

  if (type == SCOPE_INTRINSIC) {
    //
    // Include class methods and constructors in intrinsic
    //
    forv_Vec(FnSymbol, fn, *moreVisibleFunctions) {
      char *n = if1_cannonicalize_string(if1, fn->name);
      Vec<FnSymbol*> *fs = visibleFunctions.get(n);
      if (!fs) fs = new Vec<FnSymbol*>;
      fs->add(fn);
      visibleFunctions.put(n, fs);
    }
  } else if (parent) {
    //
    // Include parent scope's visible functions
    //
    for (int i = 0; i < parent->visibleFunctions.n; i++) {
      Vec<FnSymbol *> *fs = visibleFunctions.get(parent->visibleFunctions.v[i].key);
      if (!fs)
        fs = parent->visibleFunctions.v[i].value;
      else
        fs->append(*parent->visibleFunctions.v[i].value);
      visibleFunctions.put(parent->visibleFunctions.v[i].key, fs);
    }
  }
}


void SymScope::printVisibleFunctions() {
  for (int i = 0; i < visibleFunctions.n; i++) {
    Vec<FnSymbol *> *fs = visibleFunctions.v[i].value;
    if (fs) {
      forv_Vec(FnSymbol, fn, *fs) {
        if (fn) {
          fn->print(stdout);
          printf("\n");
        }
      }
    }
  }
}
