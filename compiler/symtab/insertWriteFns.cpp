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
                                    ClassType* classType, 
                                    ParamSymbol* thisArg) {
  char* fieldName = "__uninitialized";
  if (field) {
    fieldName = field->name;
  }
  char* id_tag = glomstrings(4, "_", classType->symbol->name, "_union_id_",
                             fieldName);
  Expr* argExpr = new Variable(thisArg);
  argExpr->append(new Variable(Symboltable::lookup(id_tag, true)));
  Symbol* fnSym = Symboltable::lookupInternal("_UNION_CHECK_QUIET");
  FnCall* check_function = new FnCall(new Variable(fnSym), argExpr);
  Stmt* condStmts = NULL;
  if (field) {
    condStmts = addIOStmt(condStmts, glomstrings(2, fieldName, " = "));
    condStmts = addIOStmt(condStmts, thisArg, field);
  } else {
    condStmts = addIOStmt(condStmts, "uninitialized");
  }
  Stmt* thenClause = new BlockStmt(condStmts);
  CondStmt* newCondStmt = new CondStmt(check_function, thenClause, NULL);
  if (prevStmt) {
    prevStmt->addElseStmt(newCondStmt);
  }
  return newCondStmt;
}


static Stmt* createUnionBody(Stmt* bodyStmts, ClassType* classType, 
                             ParamSymbol* thisArg) {
  CondStmt* topStmt = createUnionFieldIO(NULL, NULL, classType, thisArg);
  CondStmt* fieldIOStmt = topStmt;
  forv_Vec(VarSymbol, field, classType->fields) {
    fieldIOStmt = createUnionFieldIO(fieldIOStmt, field, classType, thisArg);
  }
  bodyStmts->append(topStmt);
  return bodyStmts;
}


static Stmt* createClassRecordBody(Stmt* bodyStmts, ClassType* classType,
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


static void createWriteFn(ClassType* classType) {
  SymScope* prevScope = Symboltable::setCurrentScope(classType->classScope);
  
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

  if (dynamic_cast<UnionType*>(classType)) {
    bodyStmts = createUnionBody(bodyStmts, classType, thisArg);
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

    ClassType* classType = dynamic_cast<ClassType*>(type);
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
