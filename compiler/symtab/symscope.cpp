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
  stmtContext(NULL),
  symContext(NULL),
  exprContext(NULL),
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
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(link->pSym)) {
      while (fn->overload) {
	fn = fn->overload;
	traversal->processSymbol(fn);
      }
    }
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
    if (scope->type == SCOPE_INTERNAL_PRELUDE) {
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


/************* NOT USING, BUT LEAVING  --SJD 2/2/05
void SymScope::remove(Symbol* sym) {


  if (firstSym == lastSym) {
    if (firstSym->pSym != sym) {
      INT_FATAL(sym, "Symbol not found in scope from which deleted");
    }
    else {
      firstSym = NULL;
      lastSym = NULL;
    }
  }
  else {
    SymLink* tmp = firstSym;
    while (tmp) {
      if (tmp->pSym == sym) {
	if (tmp == firstSym) {
	  firstSym = dynamic_cast<SymLink*>(tmp->next);
	  firstSym->prev = NULL;
	}
	else if (tmp == lastSym) {
	  lastSym = dynamic_cast<SymLink*>(tmp->prev);
	  lastSym->next = NULL;
	}
	else {
	  tmp->prev->next = tmp->next;
	  tmp->next->prev = tmp->prev;
	}
      }
      else if (tmp == lastSym) {
	INT_FATAL(sym, "Symbol not found in scope from which deleted");
      }
      else {
	tmp = nextLink(SymLink, tmp);
      }
    }
  }
}
************************/


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
  Loc* scopeLoc = NULL;
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

void SymScope::codegen(FILE* outfile, char* separator) {
  // SJD UGH HACK  --  ORDER IS A PAIN
  // I'm going to codegen EnumSymbols first, then the rest
  // This is for unions so that I codegen the IDs first.


  for (SymLink* tmp = firstSym;
       tmp;
       tmp = nextLink(SymLink, tmp)) {
    if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(tmp->pSym)) {
      if (dynamic_cast<EnumType*>(type_sym->type)) {
	tmp->pSym->codegenDef(outfile);
      }
    }
  }
  for (SymLink* tmp = firstSym;
       tmp;
       tmp = nextLink(SymLink, tmp)) {
    if (dynamic_cast<FnSymbol*>(tmp->pSym)) {
      tmp->pSym->codegenDefList(outfile, "\n");
    } else if (TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(tmp->pSym)) {
      if (!dynamic_cast<EnumType*>(type_sym->type)) {
	tmp->pSym->codegenDef(outfile);
      }
    } else {
      tmp->pSym->codegenDef(outfile);
    }
  }
}
