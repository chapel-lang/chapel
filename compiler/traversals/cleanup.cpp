#include "cleanup.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"


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
  void preProcessStmt(Stmt* &stmt);
};

void ApplyWith::preProcessStmt(Stmt* &stmt) {
  if (WithStmt* with = dynamic_cast<WithStmt*>(stmt)) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(with->parentSymbol)) {
      if (ClassType* ctype = dynamic_cast<ClassType*>(symType->type)) {
	Stmt* with_replacement = with->getClass()->definition->copyList(ctype->scope);
	Stmt::replace(stmt, with_replacement);
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
 *** functions.
 ***
 ***/

class InsertThis : public Traversal {
 public:
  void preProcessStmt(Stmt* &stmt);
};

void InsertThis::preProcessStmt(Stmt* &stmt) {
  if (TypeDefStmt* tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ClassType* ctype = dynamic_cast<ClassType*>(tds->type)) {
      Stmt* stmt = ctype->definition;

      while (stmt) {
	Stmt* next = nextLink(Stmt, stmt);
	if (FnDefStmt* method = dynamic_cast<FnDefStmt*>(stmt)) {
	  Symbol* this_insert = new ParamSymbol(PARAM_INOUT, "this", ctype);
	  this_insert->scope = method->fn->paramScope;
	  Symboltable::defineInScope(this_insert, method->fn->paramScope);
	  this_insert = appendLink(this_insert, method->fn->formals);
	  method->fn->formals = this_insert;
	  method->fn->_this = this_insert;
	}
	stmt = next;
      }
    }
  }
}


/******************************************************************************
 *** Resolve Easy
 ***
 *** This traversal resolves unresolved symbols that are easy to
 *** resolve, i.e., functions and members where the base has unknown
 *** type are left to analysis.
 ***
 ***/

class ResolveEasy : public Traversal {
 public:
  Expr* LastExpr;
  ResolveEasy::ResolveEasy(void);
  void preProcessExpr(Expr* &expr);
  void postProcessExpr(Expr* &expr);
  void preProcessSymbol(Symbol* &sym);
};

ResolveEasy::ResolveEasy(void) {
  LastExpr = NULL;
}

void ResolveEasy::preProcessExpr(Expr* &expr) {
  LastExpr = expr;
}

void ResolveEasy::postProcessExpr(Expr* &expr) {
  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
      Symbol* member = Symboltable::lookupInScope(member_access->member->name, class_type->scope);
      member_access->member = member;

//       printf("changing: ");
//       member_access->member->codegen(stdout);
//       printf(" --> ");
//       member->codegen(stdout);
//       printf("\n");

    }
    else {
      INT_FATAL(expr, "Error resolving dot-expression");
    }
  }
}

void ResolveEasy::preProcessSymbol(Symbol* &sym) {
  if (dynamic_cast<UnresolvedSymbol*>(sym)) {
    Symbol* new_sym = Symboltable::lookupFromScope(sym->name, sym->scope);
    if (!dynamic_cast<UnresolvedSymbol*>(new_sym)) {
      sym = new_sym;

//       printf("changing: ");
//       sym->codegen(stdout);
//       printf(" --> ");
//       new_sym->codegen(stdout);
//       printf("\n");

    }
    else {

//       printf("hi ");
//       new_sym->codegen(stdout);
//       printf("\n");

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
  void preProcessExpr(Expr* &expr);
};

void SpecializeParens::preProcessExpr(Expr* &expr) {
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    }
    else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (ClassType* ctype = dynamic_cast<ClassType*>(baseVar->var->type)) {
	paren_replacement = new FnCall(new Variable(ctype->constructor->fn), paren->argList);
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
    }
  }
  if (paren_replacement) {
    Expr::replace(expr, paren_replacement);
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
  void preProcessStmt(Stmt* &stmt);
  void postProcessStmt(Stmt* &stmt);
  void preProcessExpr(Expr* &expr);
};

ApplyThis::ApplyThis(void) {
  CurrentClass = NULL;
}

void ApplyThis::preProcessStmt(Stmt* &stmt) {
  if (TypeDefStmt* tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (ClassType* ctype = dynamic_cast<ClassType*>(tds->type)) {
      CurrentClass = ctype;
    }
  }
}

void ApplyThis::postProcessStmt(Stmt* &stmt) {
  if (TypeDefStmt* tds = dynamic_cast<TypeDefStmt*>(stmt)) {
    if (dynamic_cast<ClassType*>(tds->type)) {
      CurrentClass = NULL;
    }
  }
}

void ApplyThis::preProcessExpr(Expr* &expr) {
  if (CurrentClass) {
    if (Variable* member = dynamic_cast<Variable*>(expr)) {
      if (!strcmp(member->var->name, "this")) {
	return;
      }
      if (Symboltable::lookupInScope(member->var->name, CurrentClass->scope)) {

	/* replacement of expr variable by memberaccess */
	if (FnSymbol* parentFn = dynamic_cast<FnSymbol*>(member->stmt->parentSymbol)) {
	  MemberAccess* repl = new MemberAccess(new Variable(parentFn->formals),
						member->var);
	  Expr::replace(expr, repl);
	}
	else {
	  INT_FATAL(expr, "Statement is not in method in ApplyThis");
	}
      }
    }
  }
}


/******************************************************************************/

void Cleanup::run(ModuleSymbol* moduleList) { 
  ApplyWith* apply_with = new ApplyWith();
  InsertThis* insert_this = new InsertThis();
  ResolveEasy* resolve_easy = new ResolveEasy();
  SpecializeParens* specialize_parens = new SpecializeParens();
  ApplyThis* apply_this = new ApplyThis();
  ModuleSymbol* mod = moduleList;
  while (mod) {
    // TRAVERSE_LS(mod->stmts, this, true);
    TRAVERSE_LS(mod->stmts, apply_with, true);
    TRAVERSE_LS(mod->stmts, insert_this, true);
    TRAVERSE_LS(mod->stmts, resolve_easy, true);
    TRAVERSE_LS(mod->stmts, specialize_parens, true);
    TRAVERSE_LS(mod->stmts, apply_this, true);
    mod = nextLink(ModuleSymbol, mod);
  }
}
