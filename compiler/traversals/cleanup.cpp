#include "cleanup.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "../passes/runAnalysis.h"
#include "../symtab/symtabTraversal.h"


static int all_parsed = 0;

/******************************************************************************
 *** Apply With
 ***
 *** This traversal expands with-statements that appear in class
 *** definitions; it replaces them with the definitions in the
 *** included class.
 ***
 ***/

class ApplyWith : public Traversal {
public:
  void preProcessStmt(Stmt* stmt);
};

void ApplyWith::preProcessStmt(Stmt* stmt) {
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->type)) {
	Stmt* with_declarations = with->getClass()->declarationList->copyList(true);
	class_type->addDeclarations(with_declarations);
	return;
      }
    }
    USR_FATAL(stmt, "With statement is not in a class type definition");
  }
}


/******************************************************************************
 *** Insert This
 ***
 *** This traversal inserts "this" as the first parameter in bound
 *** functions.  It also resolves the class that secondary methods are
 *** bound to.
 ***
 ***/

class InsertThis : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
};

void InsertThis::preProcessStmt(Stmt* stmt) {
  FnDefStmt* def = dynamic_cast<FnDefStmt*>(stmt);

  if (!def) {
    return;
  }

  FnSymbol* fn = dynamic_cast<FnSymbol*>(def->fn);

  if (!fn) {
    return;
  }

  /***
   *** Resolve classBinding
   ***/
  if (dynamic_cast<UnresolvedSymbol*>(fn->classBinding)) {
    Symbol* classBinding = Symboltable::lookup(fn->classBinding->name);
    if (classBinding) {
      if (TypeSymbol* classBindingTypeSymbol =
	  dynamic_cast<TypeSymbol*>(classBinding)) {
	if (ClassType* classBindingType =
	    dynamic_cast<ClassType*>(classBindingTypeSymbol->type)) {
	  fn->classBinding = classBinding;
	  fn->method_type = SECONDARY_METHOD;
	  classBindingType->methods.add(fn);
	  Symboltable::defineInScope(fn, classBindingType->classScope);
	  fn->paramScope->parent = classBindingType->classScope;
	}
	else {
	  USR_FATAL(fn, "Function is not bound to legal class");
	}
      }
      else {
	USR_FATAL(fn, "Function is not bound to legal class");
      }
    }
    else {
      USR_FATAL(fn, "Function is not bound to legal class");
    }
  }
  
  /***
   *** Insert this as first parameter
   ***/
  
  if (TypeSymbol* class_symbol = dynamic_cast<TypeSymbol*>(fn->classBinding)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(class_symbol->type)) {
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(fn->paramScope);
      Symbol* this_insert = new ParamSymbol(PARAM_REF, "this", class_type);
      Symboltable::setCurrentScope(saveScope);
      this_insert = appendLink(this_insert, fn->formals);
      fn->formals = this_insert;
      fn->_this = this_insert;
    }
  }
}


/******************************************************************************
 *** Resolve Easiest
 ***
 *** This traversal resolves unresolved symbols that are easy to
 *** resolve, i.e., top-level, non-functions.
 ***
 ***/

class ResolveEasiest : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* sym);
};

static void resolve_type_helper(Type* &type) {
  if (dynamic_cast<UnresolvedType*>(type)) {
    Symbol* new_type = Symboltable::lookup(type->name->name);
    if (new_type && !new_type->isNull()) {
      if (!dynamic_cast<UnresolvedType*>(new_type->type)) {
	type = new_type->type;
      }
      else {
	resolve_type_helper(new_type->type);
	type = new_type->type;
      }
    }
    else {
      INT_FATAL(type, "Error resolving type");
    }
  }
  if (UserType* user_type = dynamic_cast<UserType*>(type)) {
    resolve_type_helper(user_type->definition);
  }
  if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    resolve_type_helper(array_type->elementType);
  }
}

void ResolveEasiest::preProcessExpr(Expr* expr) {
  if (CastExpr* cast_expr = dynamic_cast<CastExpr*>(expr)) {
    resolve_type_helper(cast_expr->newType);
  }
  if (Variable* variable_expr = dynamic_cast<Variable*>(expr)) {
    if (dynamic_cast<UnresolvedSymbol*>(variable_expr->var)) {
      Symbol* new_symbol = Symboltable::lookup(variable_expr->var->name);
      if (new_symbol) {
	if (!dynamic_cast<FnSymbol*>(new_symbol)) {
	  variable_expr->var = new_symbol;
	}
	else if (RunAnalysis::runCount > 0) {
	  if (!strcmp(new_symbol->cname, variable_expr->var->cname)) {
	    variable_expr->var = new_symbol;
	  }
	  else {
	    INT_FATAL(expr, "Unable to resolve function in Cleanup");
	    /** DETAILS: Post-analysis, we should be able to resolve
		functions.  In the case of copied symbols, the cname
		is set to the cloned or overloaded function so we can
		continue to get the right one. **/
	  }
	}
      }
      else {
	if (strcmp(variable_expr->var->name, "__primitive")) {
	  USR_FATAL(expr, "Unable to resolve token '%s'",
		    variable_expr->var->name);
	}
      }
    }
  }
}

void ResolveEasiest::preProcessSymbol(Symbol* sym) {
  resolve_type_helper(sym->type);
  FnSymbol* fnSym = dynamic_cast<FnSymbol*>(sym);
  if (fnSym) {
    resolve_type_helper(fnSym->retType);
  }
}


/******************************************************************************
 *** Rename Overloaded
 ***
 *** This traversal mangles the c name for overloaded functions.
 ***
 ***/

class RenameOverloaded : public Traversal {
 public:
  void preProcessStmt(Stmt* stmt);
};

void RenameOverloaded::preProcessStmt(Stmt* stmt) {
  static int uid = 1; // Unique ID for user-overloaded functions

  if (FnDefStmt* fndef = dynamic_cast<FnDefStmt*>(stmt)) {
    FnSymbol* fn = fndef->fn;
    if (fn->overload) {
      FnSymbol* tmp = fn;

      while (tmp) {
	if (!strstr(tmp->cname, "_user_overload_")) {
	  tmp->cname =
	    glomstrings(3, tmp->cname, "_user_overload_", intstring(uid++));
	}
	tmp = tmp->overload;
      }
    }
  }
}


/******************************************************************************
 *** Resolve Easy
 ***
 *** This traversal resolves unresolved symbols that are easy to
 *** resolve, i.e., dot-expressions where the base expression is of
 *** known class type.
 ***
 ***/

class ResolveEasy : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

void ResolveEasy::preProcessExpr(Expr* expr) {
  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
      Symbol* member = Symboltable::lookupInScope(member_access->member->name, class_type->classScope);
      if (member) {
	member_access->member = member;
      }
      else {
	INT_FATAL(expr, "Error resolving dot-expression");
      }
    }
    else {
      INT_FATAL(expr, "Error resolving dot-expression");
    }
  }
}


/******************************************************************************
 *** SpecializeParens
 ***
 *** This traversal changes all ParenOpExpr that are arrays to
 *** ArrayRef and changes all ParenOpExpr that are write, writeln, and
 *** read to IOCall.  Also, this traversal corrects constructors.
 ***
 ***/

class SpecializeParens : public Traversal {
 public:
  void preProcessExpr(Expr* expr);
};

void SpecializeParens::preProcessExpr(Expr* expr) {
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    }
    else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (ClassType* ctype = dynamic_cast<ClassType*>(baseVar->var->type)) {
	if (FnDefStmt* constructor = 
	    dynamic_cast<FnDefStmt*>(ctype->constructor)) {
	  paren_replacement = new FnCall(new Variable(constructor->fn),
					 paren->argList);
	}
	else {
	  INT_FATAL(expr, "constructor is not a FnDefStmt");
	}
      }
      else if (strcmp(baseVar->var->name, "write") == 0) {
	paren_replacement = new IOCall(IO_WRITE, paren->baseExpr, paren->argList);
      }
      else if (strcmp(baseVar->var->name, "writeln") == 0) {
	paren_replacement = new IOCall(IO_WRITELN, paren->baseExpr, paren->argList);
      }
      else if (strcmp(baseVar->var->name, "read") == 0) {
	paren_replacement = new IOCall(IO_READ, paren->baseExpr, paren->argList);
      }
      else if (dynamic_cast<FnSymbol*>(baseVar->var)) {
	paren_replacement = new FnCall(baseVar, paren->argList);
      }
    }
  }
  if (paren_replacement) {
    expr->replace(paren_replacement);
  }
}


/******************************************************************************
 *** Apply This
 ***
 *** This traversal changes all references to variables and functions
 *** defined in a class within the definitions of bound functions.
 *** They are changed to MemberAccess objects where the base is "this"
 *** and the member is the variable or function.
 ***
 ***/

class ApplyThis : public Traversal {
 public:
  ClassType* CurrentClass;
  ApplyThis::ApplyThis(void);
  void preProcessStmt(Stmt* stmt);
  void postProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
};

ApplyThis::ApplyThis(void) {
  CurrentClass = NULL;
}

void ApplyThis::preProcessStmt(Stmt* stmt) {
  if (FnDefStmt* fds = dynamic_cast<FnDefStmt*>(stmt)) {
    if (!fds->fn->classBinding->isNull()) {
      CurrentClass = dynamic_cast<ClassType*>(fds->fn->classBinding->type);
    }
  }
}

void ApplyThis::postProcessStmt(Stmt* stmt) {
  if (FnDefStmt* fds = dynamic_cast<FnDefStmt*>(stmt)) {
    if (!fds->fn->classBinding->isNull()) {
      CurrentClass = NULL;
    }
  }
}

void ApplyThis::preProcessExpr(Expr* expr) {
  if (CurrentClass) {
    if (Variable* member = dynamic_cast<Variable*>(expr)) {
      if (Symboltable::lookupInScope(member->var->name, 
				     CurrentClass->classScope)) {

	/* replacement of expr variable by memberaccess */
	if (FnSymbol* parentFn =
	    dynamic_cast<FnSymbol*>(member->stmt->parentSymbol)) {
	  if (member->var == parentFn->_this) {
	    return;
	  }
	  MemberAccess* repl =
	    new MemberAccess(new Variable(parentFn->_this), member->var);
	  repl->lineno = expr->lineno;
	  repl->filename = expr->filename;
	  expr->replace(repl);
	}
	else {
	  INT_FATAL(expr, "Statement is not in method in ApplyThis");
	}
      }
    }
  }
}


/*****************************************************************************/

void Cleanup::run(ModuleSymbol* moduleList) {
  ApplyWith* apply_with = new ApplyWith();
  InsertThis* insert_this = new InsertThis();
  ResolveEasiest* resolve_easiest = new ResolveEasiest();
  RenameOverloaded* rename_overloaded = new RenameOverloaded();
  ResolveEasy* resolve_easy = new ResolveEasy();
  SpecializeParens* specialize_parens = new SpecializeParens();
  ApplyThis* apply_this = new ApplyThis();
  ModuleSymbol* mod = moduleList;

  all_parsed = 1;  // First call to Cleanup::run and we're done parsing.
  while (mod) {
    mod->startTraversal(apply_with);
    mod->startTraversal(insert_this);
    mod->startTraversal(resolve_easiest);
    mod->startTraversal(rename_overloaded);
    mod->startTraversal(resolve_easy);
    mod->startTraversal(specialize_parens);
    mod->startTraversal(apply_this);
    mod = nextLink(ModuleSymbol, mod);
  }
}

void call_cleanup(BaseAST* ast) {
  if (all_parsed) { // Don't want to resolve if not all parsed
    TRAVERSE(ast, new ResolveEasiest(), true);
    TRAVERSE(ast, new ResolveEasy(), true);
    TRAVERSE(ast, new SpecializeParens(), true);
  }
}

void call_cleanup_ls(BaseAST* ast) {
  if (all_parsed) { // Don't want to resolve if not all parsed
    TRAVERSE_LS(ast, new ResolveEasiest(), true);
    TRAVERSE_LS(ast, new ResolveEasy(), true);
    TRAVERSE_LS(ast, new SpecializeParens(), true);
  }
}
