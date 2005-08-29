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
   _c != '_'&& _c != '?' && _c != '$')                  \


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
  uses.clear();
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


void SymScope::define(Symbol* sym) {
  Symbol* tmp = Symboltable::lookupInScope(sym->name, this);
  if (tmp) {
    if (tmp == sym) {
      INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
    }
    while (tmp->overload) {
      tmp = tmp->overload;
      if (tmp == sym) {
        INT_FATAL(sym, "Attempt to define symbol %s twice", sym->name);
      }
    }
    tmp->overload = sym;
    sym->setParentScope(tmp->parentScope);
  } else {
    table.put(sym->name, sym);
    sym->setParentScope(this);
    symbols.add(sym);
  }
}


void SymScope::undefine(Symbol* sym) {
  for (int i = 0; i < symbols.n; i++) {
    if (symbols.v[i]) {
      if (symbols.v[i] == sym) {
        symbols.v[i] = sym->overload;
        table.del(sym->name);
        if (symbols.v[i])
          table.put(sym->name, symbols.v[i]);
        sym->overload = NULL;
        return;
      } else {
        Symbol* tmp = symbols.v[i];
        while (tmp->overload) {
          if (tmp->overload == sym) {
            tmp->overload = sym->overload;
            sym->overload = NULL;
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
  case SCOPE_ARG:
    fprintf(outfile, "formal arguments");
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
  Vec<Symbol*> tableOrderSymbols;
  Vec<Symbol*>* psymbols;
  if (tableOrder) {
    table.get_values(tableOrderSymbols);
    psymbols = &tableOrderSymbols;
  } else {
    psymbols = &symbols;
  }
  forv_Vec(Symbol, sym, *psymbols) {
    if (sym) {
      fprintf(outfile, "%s", indent);
      sym->print(outfile);
      fprintf(outfile, " (");
      for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
        fprintf(outfile, "%s", tmp->cname);
        if (tmp->overload) fprintf(outfile, ", ");
      }
      fprintf(outfile, ")\n");
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
  if (type > SCOPE_MODULE) { // Because initFn has modScope
    forv_Vec(Symbol, sym, symbols) {
      for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
        if (!dynamic_cast<TypeSymbol*>(tmp)) {
          tmp->codegenDef(outfile);
        }
      }
    }
  }
}


void SymScope::codegenFunctions(FILE* outfile) {
  forv_Vec(Symbol, sym, symbols) {
    for (Symbol* tmp = sym; tmp; tmp = tmp->overload) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(tmp)) {
        ts->definition->codegenStringToType(codefile);
        ts->definition->codegenConfigVarRoutines(codefile);
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
  int is_setter = (fn->name[0] == '=' && !OPERATOR_CHAR(fn->name[1]) &&
                   fn->name[1] != '\0');
  char *n = if1_cannonicalize_string(if1, fn->name + (is_setter ? 1 : 0));
  Vec<FnSymbol*>* fs = visibleFunctions->get(n);
  if (!fs) fs = new Vec<FnSymbol*>;
  fs->add(fn);
  visibleFunctions->put(n, fs);
}


void SymScope::setVisibleFunctions(Vec<FnSymbol*>* moreVisibleFunctions) {
  forv_Vec(Symbol, symbol, symbols) {
    for (Symbol* sym = symbol; sym; sym = sym->overload) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
        if (fn->typeBinding) {
          if (ClassType* ct = dynamic_cast<ClassType*>(fn->typeBinding->definition)) {
            if (ct->isNominalType()) {
              addVisibleFunctionsHelper(&rootScope->visibleFunctions, fn);
              continue;
            }
          }
        }
        if (fn->fnClass == FN_CONSTRUCTOR) {
          if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
            if (ct->isNominalType()) {
              addVisibleFunctionsHelper(&rootScope->visibleFunctions, fn);
              continue;
            }
          }
        }
        addVisibleFunctionsHelper(&visibleFunctions, fn);
      }
    }
  }
}


void SymScope::getVisibleFunctions(Vec<FnSymbol*>* allVisibleFunctions,
                                   char* name,
                                   bool recursed) {

  // to avoid infinite loop because of cyclic module uses
  static Vec<SymScope*> visited;
  if (!recursed)
    visited.clear();
  if (visited.set_in(this))
    return;
  visited.set_add(this);

  Vec<FnSymbol*>* fs = visibleFunctions.get(name);
  if (fs)
    allVisibleFunctions->append(*fs);
  forv_Vec(ModuleSymbol, module, uses) {
    module->modScope->getVisibleFunctions(allVisibleFunctions, name, true);
  }
  if (parent)
    parent->getVisibleFunctions(allVisibleFunctions, name, true);
}


void SymScope::printVisibleFunctions() {
  for (int i = 0; i < visibleFunctions.n; i++) {
    Vec<FnSymbol *> *fs = visibleFunctions.v[i].value;
    if (fs) {
      forv_Vec(FnSymbol, fn, *fs) {
        if (fn) {
          fn->print(stdout);
          printf(" %ld\n", fn->id);
        }
      }
    }
  }
}


void
getSymbols(SymScope *scope, Vec<Symbol *> &symbols) {
  if (!scope) return;
  forv_Symbol(sym, scope->symbols)
    for (Symbol *s = sym; s; s = s->overload)
      symbols.set_add(s);
  getSymbols(scope->child, symbols);
  getSymbols(scope->sibling, symbols);
}
