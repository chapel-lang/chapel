#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symlink.h"
#include "symscope.h"
#include "symtab.h"
#include "symtabTraversal.h"


SymScope::SymScope(scopeType init_type, int init_level) :
  type(init_type),
  level(init_level),
  stmtContext(nilStmt),
  symContext(nilSymbol),
  exprContext(nilExpr),
  parent(NULL),
  child(NULL),
  sibling(NULL),
  firstSym(NULL),
  lastSym(NULL)
{}


void SymScope::setContext(Stmt* stmt, Symbol* sym, Expr* expr) {
  stmtContext = stmt;
  symContext = sym;
  exprContext = expr;
}


void SymScope::traverse(SymtabTraversal* traversal) {
  SymScope* prevScope = Symboltable::setCurrentScope(this);

  traversal->preProcessScope(this);

  SymLink* link = firstSym;
  while (link) {
    traversal->processSymbol(link->pSym);

    link = nextLink(SymLink, link);
  }

  traversal->postProcessScope(this);

  Symboltable::setCurrentScope(prevScope);
}


bool SymScope::isEmpty(void) {
  return (firstSym == NULL);
}


bool SymScope::isInternal(void) {
  SymScope* scope = this;

  while (scope != NULL) {
    if (scope->type == SCOPE_INTERNAL_PRELUDE || 
	scope->type == SCOPE_PRELUDE) {
      return true;
    }
    scope = scope->parent;
  }
  return false;
}


void SymScope::insert(Symbol* sym) {
  table.put(sym->name, sym);
  sym->setParentScope(this);

  SymLink* newLink = new SymLink(sym);

  if (firstSym == NULL) {
    firstSym = newLink;
    lastSym = firstSym;
  } else {
    lastSym->append(newLink);
    lastSym = newLink;
  }
}


SymScope* SymScope::findEnclosingScopeType(scopeType t) {
  if (type == t) {
    return this;
  } else {
    if (parent == NULL) {
      INT_FATAL("can't find scope");
    }
    return parent->findEnclosingScopeType(t);
  }
}


char* SymScope::indentStr(void) {
  static char* spaces = "                                                     "
                        "                          ";
  int printLevel;
  switch (type) {
  case SCOPE_INTRINSIC:
  case SCOPE_POSTPARSE:
    printLevel = 0;
    break;
  case SCOPE_INTERNAL_PRELUDE:
  case SCOPE_PRELUDE:
    printLevel = 1;
    break;
  default:
    printLevel = level+1;
  }

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

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "======================================================\n");

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "SCOPE: ");
  switch (type) {
  case SCOPE_INTRINSIC:
    fprintf(outfile, "intrinsic");
    break;
  case SCOPE_INTERNAL_PRELUDE:
    fprintf(outfile, "internal prelude");
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
  case SCOPE_CLASS:
    fprintf(outfile, "class");
    break;
  case SCOPE_POSTPARSE:
    fprintf(outfile, "post parsing");
    break;
  }
  Loc* scopeLoc = NULL;
  if (!symContext->isNull()) {
    fprintf(outfile, " ");
    symContext->print(outfile);
    scopeLoc = symContext;
  } else if (!exprContext->isNull()) {
    scopeLoc = exprContext;
  } else if (!stmtContext->isNull()) {
    scopeLoc = stmtContext;
  }
  if (scopeLoc) {
    fprintf(outfile, " (%s)", scopeLoc->stringLoc());
  }
  fprintf(outfile, "\n");

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "------------------------------------------------------\n");
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
    if (firstSym != NULL) {
      fprintf(outfile, "%s", indent);
      firstSym->printList(outfile, glomstrings(2, "\n", indent));
      fprintf(outfile, "\n");
    }
  }
}


void SymScope::printFooter(FILE* outfile) {
  char* indent = indentStr();

  fprintf(outfile, "%s", indent);
  fprintf(outfile, "======================================================\n");
}


void SymScope::print(FILE* outfile, bool tableOrder) {
  if (!isEmpty() && !printEmpty) {
    printHeader(outfile);
    printSymbols(outfile, tableOrder);
    printFooter(outfile);
  }
}
