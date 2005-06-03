#include "insertThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void InsertThisParameters::preProcessStmt(Stmt* stmt) {
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  FnSymbol* fn = def_stmt->fnDef();

  if (!fn) {
    /*** mangle type names in class types ***/
    if (def_stmt->definesTypes()) {
      for (DefExpr* defExpr = def_stmt->defExprls->first();
           defExpr;
           defExpr = def_stmt->defExprls->next()) {
        TypeSymbol* type_sym = dynamic_cast<TypeSymbol*>(defExpr->sym);
        if (StructuralType* classType = dynamic_cast<StructuralType*>(type_sym->type)) {
          forv_Vec(TypeSymbol, type, classType->types) {
            type->cname = glomstrings(4, "_", classType->symbol->cname, "_", type->cname);
          }
        }
      }
    }
    return;
  }

  /***
   *** Resolve typeBinding
   ***/
  if (dynamic_cast<UnresolvedSymbol*>(fn->typeBinding)) {
    Symbol* typeBindingSymbol = Symboltable::lookup(fn->typeBinding->name);
    if (dynamic_cast<TypeSymbol*>(typeBindingSymbol)) {
      Type* typeBinding = typeBindingSymbol->type;
      fn->typeBinding = typeBindingSymbol;
      if (!fn->isConstructor) {
        fn->method_type = SECONDARY_METHOD;
      }
      typeBinding->methods.add(fn);
    } else {
      USR_FATAL(fn, "Function is not bound to type");
    }
  }
  
  /***
   *** Mangle code-generation name and insert this as first parameter
   ***/
  
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(fn->typeBinding)) {
    fn->cname = glomstrings(4, "_", typeSym->cname, "_", fn->cname);
    if (fn->isConstructor) {
      SymScope* saveScope = Symboltable::setCurrentScope(fn->body->body->only()->parentScope);
      DefStmt* this_decl = Symboltable::defineSingleVarDefStmt(copystring("this"),
                                                               typeSym->type,
                                                               NULL,
                                                               VAR_NORMAL,
                                                               VAR_VAR);
      fn->_this = this_decl->varDef();
      fn->retType = typeSym->type;
      dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;
      fn->body->body->insertAtHead(this_decl);
      if (dynamic_cast<ClassType*>(typeSym->type)) {
        char* description = glomstrings(2, "instance of class ", typeSym->name);
        AList<Expr>* alloc_args = new AList<Expr>(new IntLiteral("1", 1));
        alloc_args->insertAtTail(new SizeofExpr(new Variable(fn->_this)));
        alloc_args->insertAtTail(new StringLiteral(description));
        Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_malloc");
        Expr* alloc_call = new FnCall(new Variable(alloc_sym), alloc_args);
        Expr* alloc_lhs = new Variable(fn->_this);
        Expr* alloc_rhs = new CastLikeExpr(new Variable(fn->_this), alloc_call);
        Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
        Stmt* alloc_stmt = new ExprStmt(alloc_expr);
        this_decl->insertAfter(alloc_stmt);
      }
      fn->body->body->insertAtTail(new ReturnStmt(new Variable(fn->_this)));
      Symboltable::setCurrentScope(saveScope);

      // fix type variables, associate by name
      if (StructuralType* structType = dynamic_cast<StructuralType*>(typeSym->type)) {
        for (ParamSymbol* arg = fn->formals->first(); arg; arg = fn->formals->next()) {
          if (arg->isGeneric) {
            forv_Vec(TypeSymbol, tmp, structType->types) {
              if (VariableType* variableType = dynamic_cast<VariableType*>(tmp->type)) {
                if (!strcmp(tmp->name, arg->name)) {
                  arg->type = variableType->type;
                  arg->isGeneric = true;
                  arg->typeVariable = tmp;
                }
              }
            }
          }
        }
      }
    } else {
      SymScope* saveScope = Symboltable::setCurrentScope(fn->paramScope);
      ParamSymbol* this_insert = new ParamSymbol(PARAM_REF, "this", typeSym->type);
      this_insert->setDefPoint(def_stmt->defExprls->only());
      Symboltable::setCurrentScope(saveScope);
      fn->formals->insertAtHead(this_insert);
      fn->_this = this_insert;
    }
  }

  /***
   *** Move function out of class scope, into module scope
   ***/

  if (fn->defPoint->parentSymbol == fn->typeBinding) {
    Stmt* insertPoint = fn->typeBinding->defPoint->parentStmt;
    while (!dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol)) {
      insertPoint = insertPoint->parentSymbol->defPoint->parentStmt;
    }
    ModuleSymbol* moduleSymbol = dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol);
    SymScope* saveScope =
      Symboltable::setCurrentScope(moduleSymbol->modScope);
    DefStmt* defStmt = dynamic_cast<DefStmt*>(fn->defPoint->parentStmt->copy(true));
    FnSymbol* newFn = defStmt->fnDef();
    newFn->cname = copystring(fn->cname);
    insertPoint->insertBefore(defStmt);
    fn->defPoint->parentStmt->remove();
    fn->parentScope->remove(fn);
    Symboltable::removeScope(fn->paramScope);
    StructuralType* structuralType = dynamic_cast<StructuralType*>(fn->typeBinding->type);
    for (int i = 0; i < structuralType->methods.n; i++) {
      if (structuralType->methods.v[i] == fn) {
        structuralType->methods.v[i] = newFn;
      }
    }
    Symboltable::setCurrentScope(saveScope);
  }
}
