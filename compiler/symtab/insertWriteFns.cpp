#include "insertWriteFns.h"
#include "stringutil.h"
#include "symtab.h"

InsertWriteFns::InsertWriteFns(void) {
  whichModules = MODULES_COMMON_AND_USER;
}


static void createWriteFn(StructuralType* classType) {
  SymScope* newScope =
    classType->structScope->findEnclosingScopeLessType(SCOPE_MODULE);
  SymScope* saveScope =
    Symboltable::setCurrentScope(newScope);
  FnSymbol* writeFn = Symboltable::startFnDef(new FnSymbol("write", 
                                                           classType->symbol),
                                              false);
  writeFn->cname = glomstrings(3, "_", classType->symbol->name, "_write");
  ParamSymbol* thisArg = Symboltable::defineParams(PARAM_REF, 
                                                   new Symbol(SYMBOL, "_this"),
                                                   classType, NULL);
  
  Stmt* bodyStmts = classType->buildIOBodyStmts(thisArg);
  Stmt* body = new BlockStmt(bodyStmts);
  
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(writeFn, thisArg, dtVoid, body));
  DefStmt* defstmt = new DefStmt(def);
  classType->addDeclarations(defstmt);
  //   Symboltable::dump(stderr);
  Symboltable::setCurrentScope(saveScope);
}


void InsertWriteFns::processSymbol(Symbol* sym) {
  TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(sym);
  if (typeSym != NULL && !typeSym->isDead) {
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
