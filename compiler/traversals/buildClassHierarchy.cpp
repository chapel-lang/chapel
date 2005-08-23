#include "buildClassHierarchy.h"
#include "expr.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"


static void addClassToHierarchy(ClassType* ct,
                                Vec<ClassType*>* alreadySeen) {
  forv_Vec(ClassType, at, *alreadySeen) {
    if (at == ct) {
      USR_FATAL(ct, "Cyclic class hierarchy detected");
    }
  }

  for_alist(Expr, expr, ct->inherits) {
    SymExpr* symExpr = dynamic_cast<SymExpr*>(expr);
    if (!symExpr) {
      USR_FATAL_CONT(symExpr, "Possible temporary restriction follows:");
      USR_FATAL(symExpr, "Inheritance is only supported from simple classes");
    }
    Symbol* symbol = Symboltable::lookup(symExpr->var->name);
    TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(symbol);
    if (!typeSymbol)
      USR_FATAL(expr, "Illegal to inherit from something other than a class");
    ClassType* pt = dynamic_cast<ClassType*>(typeSymbol->definition);
    if (!pt)
      USR_FATAL(expr, "Illegal to inherit from something other than a class");
    if (pt->classTag == CLASS_RECORD || pt->classTag == CLASS_UNION)
      USR_FATAL(expr, "Illegal to inherit from record or union");
    if (ct->classTag == CLASS_RECORD || ct->classTag == CLASS_UNION)
      USR_FATAL(expr, "Illegal for record or union to inherit");
    if (pt->inherits) {
      alreadySeen->add(ct);
      addClassToHierarchy(pt, alreadySeen);
    }
    ct->dispatchParents.add(pt);
    ct->addDeclarations(pt->declarationList->copy(true), ct->declarationList->first());
  }
  if (ct->dispatchParents.n == 0 && ct != dtObject)
    ct->dispatchParents.add(dtObject);
  ct->inherits = NULL;
}


void BuildClassHierarchy::postProcessExpr(Expr* expr) {
  DefExpr* defExpr = dynamic_cast<DefExpr*>(expr);
  if (!defExpr)
    return;

  TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(defExpr->sym);
  if (!typeSymbol)
    return;

  ClassType* classType = dynamic_cast<ClassType*>(typeSymbol->definition);
  if (!classType)
    return;

  if (classType->inherits) {
    Vec<ClassType*> alreadySeen;
    alreadySeen.clear();
    addClassToHierarchy(classType, &alreadySeen);
  }
}
