#include "insertWriteFns.h"
#include "symtab.h"

InsertWriteFns::InsertWriteFns(void) {
  skipInternalScopes = true;
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
	  //	  fprintf(stderr, "  ***has a write function!!!\n");
	  return;
	} else {
	  //	  fprintf(stderr, "  has a method: %s\n", method->name);
	}
      }

      SymScope* prevScope = Symboltable::setCurrentScope(classType->classScope);

      FnSymbol* writeFn = Symboltable::startFnDef(new FnSymbol("write", 
							       typeSym),
						  false);
      IOCall* ioexpr = new IOCall(IO_WRITELN, new StringLiteral("write"),
				  new StringLiteral("hiya!"));
      Stmt* body = new ExprStmt(ioexpr);
      DefExpr* def = Symboltable::finishFnDef(writeFn, NULL, dtVoid, body);
      DefStmt* defstmt = new DefStmt(def);
      classType->addDeclarations(defstmt);

      Symboltable::setCurrentScope(prevScope);

      //      Symboltable::dump(stderr);
    }
  }
}
