#include "insertThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

void InsertThisParameters::preProcessExpr(Expr* expr) {
  DefExpr* defExpr = dynamic_cast<DefExpr*>(expr);

  if (!defExpr) {
    return;
  }

  /***
   *** Mangle type names in class types
   ***/
  if (dynamic_cast<TypeSymbol*>(defExpr->sym)) {
    if (ClassType* classType = dynamic_cast<ClassType*>(defExpr->sym->type)) {
      forv_Vec(TypeSymbol, type, classType->types) {
        type->cname = glomstrings(4, "_", classType->symbol->cname, "_", type->cname);
      }
    }
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym);

  if (!fn) {
    return;
  }

  /***
   *** Resolve typeBinding
   ***/
  if (fn->typeBinding && fn->typeBinding->isUnresolved) {
    Symbol* typeBindingSymbol = Symboltable::lookup(fn->typeBinding->name);
    assert(!typeBindingSymbol->isUnresolved);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(typeBindingSymbol)) {
      Type* typeBinding = ts->definition;
      fn->typeBinding = ts;
      if (fn->fnClass != FN_CONSTRUCTOR) {
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
    if (fn->fnClass == FN_CONSTRUCTOR) {
      SymScope* saveScope = Symboltable::setCurrentScope(fn->body->body->first()->parentScope);
      fn->body->body->reset(); // reset iteration
      DefExpr* this_decl = Symboltable::defineSingleVarDef(copystring("this"),
                                                           typeSym->definition,
                                                           NULL,
                                                           VAR_NORMAL,
                                                           VAR_VAR);
      fn->_this = dynamic_cast<VarSymbol*>(this_decl->sym);
      fn->retType = typeSym->definition;
      dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;
      fn->body->body->insertAtHead(new ExprStmt(this_decl));
      char* description = glomstrings(2, "instance of class ", typeSym->name);
      Expr* alloc_rhs = new CallExpr(Symboltable::lookupInternal("_chpl_alloc"),
                                     new Variable(typeSym),
                                     new StringLiteral(description));
      Expr* alloc_lhs = new Variable(fn->_this);
      Expr* alloc_expr = new CallExpr(OP_GETSNORM, alloc_lhs, alloc_rhs);
      Stmt* alloc_stmt = new ExprStmt(alloc_expr);
      this_decl->parentStmt->insertAfter(alloc_stmt);
      fn->body->body->insertAtTail(new ReturnStmt(new Variable(fn->_this)));
      Symboltable::setCurrentScope(saveScope);

      // fix type variables, associate by name
      if (ClassType* structType = dynamic_cast<ClassType*>(typeSym->definition)) {
        for_alist(DefExpr, arg, fn->formals) {
          if (dynamic_cast<ParamSymbol*>(arg->sym)->isGeneric) {
            forv_Vec(TypeSymbol, tmp, structType->types) {
              if (VariableType* variableType = dynamic_cast<VariableType*>(tmp->definition)) {
                if (!strcmp(tmp->name, arg->sym->name)) {
                  arg->sym->type = variableType->type;
                  dynamic_cast<ParamSymbol*>(arg->sym)->isGeneric = true;
                  dynamic_cast<ParamSymbol*>(arg->sym)->typeVariable = tmp;
                }
              }
            }
          }
        }
      }
    } else {
      SymScope* saveScope = Symboltable::setCurrentScope(fn->paramScope);
      ParamSymbol* this_insert = new ParamSymbol(PARAM_REF, "this", typeSym->definition);
      Symboltable::setCurrentScope(saveScope);
      fn->formals->insertAtHead(new DefExpr(this_insert));
      fn->_this = this_insert;
      bool isThisMethod = !strcmp(fn->name, "this");
      if (applyGettersSetters) {
        SymScope* saveScope = Symboltable::setCurrentScope(fn->paramScope);
        if (!isThisMethod) {
          TypeSymbol *methodTypeSymbol = dynamic_cast<TypeSymbol*>(Symboltable::lookupInternal("_methodTokenType"));
          ParamSymbol* token_dummy = new ParamSymbol(PARAM_REF, "_methodTokenDummy",
                                                     methodTypeSymbol->definition);
          fn->formals->insertAtHead(new DefExpr(token_dummy));
        }
        if (fn->isSetter) {
          TypeSymbol *setterTypeSymbol = dynamic_cast<TypeSymbol*>(Symboltable::lookupInternal("_setterTokenType"));
          ParamSymbol* setter_dummy = new ParamSymbol(PARAM_REF, "_setterTokenDummy", 
                                                      setterTypeSymbol->definition);
          int len = fn->formals->length();
          fn->formals->get(len)->insertBefore(new DefExpr(setter_dummy));
        }
        Symboltable::setCurrentScope(saveScope);
      }
    }
  }
}
