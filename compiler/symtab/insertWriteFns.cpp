#include "insertWriteFns.h"
#include "stringutil.h"
#include "symtab.h"

InsertWriteFns::InsertWriteFns(void) {
  whichModules = MODULES_USER;
}


static Stmt* addIOStmt(Stmt* ioStmtList, Expr* argExpr) {
  IOCall* ioExpr = new IOCall(IO_WRITE, new StringLiteral("write"), argExpr);
  Stmt* ioStmt = new ExprStmt(ioExpr);
  if (ioStmtList == NULL) {
    ioStmtList = ioStmt;
  } else {
    ioStmtList->append(ioStmt);
  }
  return ioStmtList;
}


static Stmt* addIOStmt(Stmt* ioStmtList, ParamSymbol* _this, VarSymbol* field) {
  return addIOStmt(ioStmtList, new MemberAccess(new Variable(_this), field));
}


static Stmt* addIOStmt(Stmt* ioStmtList, char* str) {
  return addIOStmt(ioStmtList, new StringLiteral(str));
}


static CondStmt* createUnionFieldIO(CondStmt* prevStmt, VarSymbol* field, 
                                    UnionType* unionType, 
                                    ParamSymbol* thisArg) {
  FnCall* checkFn = unionType->buildSafeUnionAccessCall(UNION_CHECK_QUIET,
                                                        new Variable(thisArg),
                                                        field);
  Stmt* condStmts = NULL;
  if (field) {
    condStmts = addIOStmt(condStmts, glomstrings(2, field->name, " = "));
    condStmts = addIOStmt(condStmts, thisArg, field);
  } else {
    condStmts = addIOStmt(condStmts, "uninitialized");
  }
  Stmt* thenClause = new BlockStmt(condStmts);
  CondStmt* newCondStmt = new CondStmt(checkFn, thenClause, NULL);
  if (prevStmt) {
    prevStmt->addElseStmt(newCondStmt);
  }
  return newCondStmt;
}


static Stmt* createUnionBody(Stmt* bodyStmts, UnionType* unionType, 
                             ParamSymbol* thisArg) {
  CondStmt* topStmt = createUnionFieldIO(NULL, NULL, unionType, thisArg);
  CondStmt* fieldIOStmt = topStmt;
  forv_Vec(VarSymbol, field, unionType->fields) {
    fieldIOStmt = createUnionFieldIO(fieldIOStmt, field, unionType, thisArg);
  }
  bodyStmts->append(topStmt);
  return bodyStmts;
}


static Stmt* createClassRecordBody(Stmt* bodyStmts, StructuralType* classType,
                                   ParamSymbol* thisArg) {
  bool firstField = true;

  forv_Vec(VarSymbol, field, classType->fields) {
    if (!firstField) {
      bodyStmts = addIOStmt(bodyStmts, ", ");
    } else {
      firstField = false;
    }
    bodyStmts = addIOStmt(bodyStmts, glomstrings(2, field->name, " = "));
    bodyStmts = addIOStmt(bodyStmts, thisArg, field);
  }
  return bodyStmts;
}


static void createWriteFn(StructuralType* classType) {
  SymScope* prevScope = Symboltable::setCurrentScope(classType->structScope);
  
  FnSymbol* writeFn = Symboltable::startFnDef(new FnSymbol("write", 
                                                           classType->symbol),
                                              false);
  writeFn->cname = glomstrings(3, "_", classType->symbol->name, "_write");
  ParamSymbol* thisArg = Symboltable::defineParams(PARAM_REF, 
                                                   new Symbol(SYMBOL, "_this"),
                                                   classType, NULL);
  
  bool useParens = classType->value;
  Stmt* bodyStmts = NULL;

  if (useParens) {
    bodyStmts = addIOStmt(bodyStmts, "(");
  } else {
    bodyStmts = addIOStmt(bodyStmts, "{");
  }

  UnionType* unionType = dynamic_cast<UnionType*>(classType);
  if (unionType) {
    bodyStmts = createUnionBody(bodyStmts, unionType, thisArg);
  } else {
    bodyStmts = createClassRecordBody(bodyStmts, classType, thisArg);
  }

  if (useParens) {
    bodyStmts = addIOStmt(bodyStmts, ")");
  } else {
    bodyStmts = addIOStmt(bodyStmts, "}");
  }

  Stmt* body = new BlockStmt(bodyStmts);
  
  DefExpr* def = Symboltable::finishFnDef(writeFn, thisArg, dtVoid, body);
  DefStmt* defstmt = new DefStmt(def);
  classType->addDeclarations(defstmt);
  
  Symboltable::setCurrentScope(prevScope);
  //   Symboltable::dump(stderr);
}


void InsertWriteFns::processSymbol(Symbol* sym) {
  TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym);
  if (typeSym != NULL) {
    Type* type = typeSym->type;

    StructuralType* classType = dynamic_cast<StructuralType*>(type);
    if (classType != NULL) {
      //      fprintf(stderr, "Found a class function: %s\n", typeSym->name);
      forv_Symbol(method, classType->methods) {
        if (strcmp(method->name, "write") == 0) {
          //      fprintf(stderr, "  ***has a write function!!!\n");
          return;
        }
      }

      createWriteFn(classType);
    }
  }
}
