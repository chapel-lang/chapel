#include <stdlib.h>
#include <stdio.h>
#include "link.h"
#include "misc.h"
#include "baseAST.h"
#include "stmt.h"
#include "expr.h"
#include "../traversals/fixup.h"

ILink::ILink(void) :
  prev(nilILink),
  next(nilILink),
  back(NULL)
{}


void ILink::traverse(ILink* &_this, Traversal* traversal, bool atTop) {
  if (isNull()) return;

  if (BaseAST* ast = dynamic_cast<BaseAST*>(_this)) {
    ast->traverse(ast, traversal, atTop);
  }
}


bool ILink::isNull(void) {
  return (this == nilILink);
}


void ILink::print(FILE* outfile) {
  if (isNull()) {
    INT_FATAL("Calling print() on a Null AST");
  } else {
    INT_FATAL("print() is unimplemented for a subclass of ILink");
  }
}


void ILink::println(FILE* outfile) {
  this->print(outfile);
  fprintf(outfile, "\n");
}


void ILink::printList(FILE* outfile, char* separator) {
  ILink* ptr;

  if (!isNull()) {
    print(outfile);
    ptr = next;
    while (!ptr->isNull()) {
      fprintf(outfile, "%s", separator);
      ptr->print(outfile);
      ptr = ptr->next;
    }
  }
}

void ILink::codegen(FILE* outfile) {
  if (isNull()) {
    INT_FATAL("Calling codegen() on a Null AST");
  } else {
    INT_FATAL("codegen() is unimplemented for a subclass of ILink");
  }
}


void ILink::codegenList(FILE* outfile, char* separator) {
  ILink* ptr;

  if (!isNull()) {
    codegen(outfile);
    ptr = next;
    while (!ptr->isNull()) {
      fprintf(outfile, "%s", separator);
      ptr->codegen(outfile);
      ptr = ptr->next;
    }
  }
}


void ILink::add(ILink* newlink) {
  newlink->next = next;
  newlink->prev = this;
  next->prev = newlink;
  next = newlink;
}


void ILink::append(ILink* newlink) {
  ILink* lastlink;

  if (newlink->isNull()) {
    return;
  }
  lastlink = this;
  while (!lastlink->next->isNull()) {
    lastlink = lastlink->next;
  }
  lastlink->next = newlink;
  newlink->prev = lastlink;
}


void ILink::filter(bool filter(ILink*), ILink** truelinks, 
		   ILink** falselinks) {
  ILink* link = this;
  ILink* nextlink;
  *truelinks = nilILink;
  *falselinks = nilILink;

  while (!link->isNull()) {
    nextlink = link->next;
    link->next = nilILink;
    if (!nextlink->isNull()) {
      nextlink->prev = nilILink;
    }
    if (filter(link)) {
      *truelinks = appendLink(*truelinks, link);
    } else {
      *falselinks = appendLink(*falselinks, link);
    }
    link = nextlink;
  }
}


static ILink* find_start_of_list(ILink* list) {
  ILink* first = list;
  while (first->prev && !first->prev->isNull()) {
    first = first->prev;
  }
  return first;
}


static ILink* find_end_of_list(ILink* list) {
  ILink* last = list;
  while (last->next && !last->next->isNull()) {
    last = last->next;
  }
  return last;
}


static void call_fixup(ILink* link) {
  Fixup* fixup = new Fixup();
  fixup->setArgs("");

  if (Expr* expr = dynamic_cast<Expr*>(link)) {
    fixup->stmtParent.add(expr->stmt->parentSymbol);
    TRAVERSE(expr->stmt, fixup, true);
  }
  else if (Stmt* stmt = dynamic_cast<Stmt*>(link)) {
    fixup->stmtParent.add(stmt->parentSymbol);
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(stmt->parentSymbol)) {
      TRAVERSE_LS(fn->body, fixup, true);
    }
    else if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(stmt->parentSymbol)) {
      TRAVERSE_LS(mod->stmts, fixup, true);
    }
    else if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(stmt->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(type->type)) {
	TRAVERSE(class_type, fixup, true);
      }
    }
  }
  else {
    INT_FATAL(link, "Symbol and Types not supported for any of the following:\n"
                        "  insertBefore, insertAfter, replace\n");
  }
}


void ILink::insertBefore(ILink* new_link) {
  ILink* first = find_start_of_list(new_link);
  ILink* last = find_end_of_list(new_link);

  if (first != new_link) {
    INT_FATAL(this, "Illegal insertBefore, new_link is not head of list");
  }

  first->prev = prev;

  *back = first;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = first;
    }
  */

  prev = last;
  last->next = this;

  call_fixup(this);
}


void ILink::insertAfter(ILink* new_link) {
  ILink* first = find_start_of_list(new_link);
  ILink* last = find_end_of_list(new_link);

  if (first != new_link) {
    INT_FATAL(this, "Illegal insertAfter, new_link is not head of list");
  }

  last->next = next;
  if (next && !next->isNull()) {
    next->prev = last;
  }
  next = first;
  first->prev = this;

  call_fixup(this);
}


ILink* ILink::extract(void) {
  if (next && !next->isNull()) {
    next->prev = prev;
    next->back = back;
  }
  *back = next;
  /* NOT NECESSARY BECAUSE OF PRECEDING LINE
    if (prev && !prev->isNull()) {
      prev->next = next;
    }
  */

  next = nilILink;
  prev = nilILink;
  back = NULL;

  return this;
}


void ILink::replace(ILink* old_link, ILink* new_link) {
  ILink* first = find_start_of_list(new_link);
  ILink* last = find_end_of_list(new_link);

  if (first != new_link) {
    INT_FATAL(old_link, "Illegal replace, new_link is not head of list");
  }

  first->prev = old_link->prev;
  if (old_link->prev && !old_link->prev->isNull()) {
    old_link->prev->next = first;
  }
  last->next = old_link->next;
  if (old_link->next && !old_link->next->isNull()) {
    old_link->next->prev = last;
  }

  *old_link->back = new_link;

  /* Wipe out old links--it's been replaced */
  old_link->prev = nilILink;
  old_link->next = nilILink;

  call_fixup(old_link);
}
