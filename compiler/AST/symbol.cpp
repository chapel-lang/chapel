#include <typeinfo>
#include "analysis.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"


Symbol::Symbol(astType_t astType, char* init_name, Type* init_type) :
  BaseAST(astType),
  name(init_name),
  type(init_type),
  scope(Symboltable::getCurrentScope()),
  asymbol(0)
{}


Symbol* Symbol::copy(void) {
  INT_FATAL(this, "Symbol::copy() not anticipated to be needed");
  return nilSymbol;
}


Symbol* Symbol::copyList(void) {
  Symbol* newSymbolList = nilSymbol;
  Symbol* oldSymbol = this;

  while (oldSymbol) {
    newSymbolList = appendLink(newSymbolList, oldSymbol->copy());

    oldSymbol = nextLink(Symbol, oldSymbol);
  }
  
  return newSymbolList;
}


bool Symbol::isNull(void) {
  return (this == nilSymbol);
}


void Symbol::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }

  // explore Symbol and components
  if (traversal->processTop || !atTop) {
    traversal->preProcessSymbol(this);
  }
  if (atTop || traversal->exploreChildSymbols) {
    traverseSymbol(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessSymbol(this);
  }
}


void Symbol::traverseSymbol(Traversal* traversal) {
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

void Symbol::codegen(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printDef(FILE* outfile) {
  print(outfile);
}


void Symbol::printDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  printDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr) {
    fprintf(outfile, "%s", separator);
    ptr->printDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


void Symbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  type->codegenSafeInit(outfile);
}  


void Symbol::codegenDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  codegenDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr) {
    fprintf(outfile, "%s", separator);
    ptr->codegenDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


UnresolvedSymbol::UnresolvedSymbol(char* init_name) :
  Symbol(SYMBOL_UNRESOLVED, init_name)
{}


void UnresolvedSymbol::codegen(FILE* outfile) {
  //  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
  Symbol::codegen(outfile);
}


Symbol* UnresolvedSymbol::copy(void) {
  return new UnresolvedSymbol(copystring(name));
}


VarSymbol::VarSymbol(char* init_name, Type* init_type, varType init_varClass, 
		     bool init_isConst) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  isConst(init_isConst)
{}


Symbol* VarSymbol::copy(void) {
  INT_FATAL(this, "VarSymbol::copy() not implemented yet");
  return nilSymbol;
}


bool VarSymbol::isNull(void) {
  return (this == nilVarSymbol);
}


void VarSymbol::printDef(FILE* outfile) {
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


static char* paramTypeNames[NUM_PARAM_TYPES] = {
  "in",
  "inout",
  "out",
  "const"
};


ParamSymbol::ParamSymbol(paramType init_usage, char* init_name, 
			 Type* init_type) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  usage(init_usage)
{}


Symbol* ParamSymbol::copy(void) {
  return new ParamSymbol(usage, copystring(name), type->copy());
}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[usage]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


void ParamSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(SYMBOL_TYPE, init_name, init_definition)
{}


Symbol* TypeSymbol::copy(void) {
  INT_FATAL(this, "TypeSymbol::copy() not implemented yet");
  return nilSymbol;
}


FnSymbol::FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
		   Stmt* init_body, bool init_exportMe, 
		   FnSymbol* init_parentFn) :
  Symbol(SYMBOL_FN, init_name, init_retType),
  exportMe(init_exportMe),
  formals(init_formals),
  body(init_body),
  parentFn(init_parentFn)
{}


FnSymbol::FnSymbol(char* init_name, FnSymbol* init_parentFn) :
  Symbol(SYMBOL_FN, init_name, nilType),
  formals(nilSymbol),
  body(nilStmt),
  parentFn(init_parentFn)
{}


void FnSymbol::finishDef(Symbol* init_formals, Type* init_retType,
			 Stmt* init_body, bool init_exportMe) {
  formals = init_formals;
  type = init_retType;
  body = init_body;
  exportMe = init_exportMe;

  if (strcmp(name, "main") == 0 && scope->type == SCOPE_MODULE && 
      formals->isNull()) {
    if (mainFn->isNull()) {
      mainFn = this;
      exportMe = true;
    } else {
      USR_FATAL(this, "main multiply defined -- first occurrence at %s\n",
		mainFn->stringLoc());
    }
  }
}


Symbol* FnSymbol::copy(void) {
  INT_FATAL(this, "ReduceSymbol::copy() not implemented yet");
  return nilSymbol;
}


bool FnSymbol::isNull(void) {
  return (this == nilFnSymbol);
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (!exportMe) {
    fprintf(outfile, "static ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (formals->isNull()) {
    fprintf(outfile, "void");
  } else {
    formals->codegenDefList(outfile, ", ");
  }
  fprintf(outfile, ")");
}

FnSymbol* FnSymbol::mainFn;

void FnSymbol::init(void) {
  mainFn = nilFnSymbol;
}



EnumSymbol::EnumSymbol(char* init_name, Expr* init_init, int init_val) :
  Symbol(SYMBOL_ENUM, init_name),
  init(init_init),
  val(init_val)
{}


Symbol* EnumSymbol::copy(void) {
  return new EnumSymbol(copystring(name), init->copy(), val);
}


void EnumSymbol::set_values(void) {
  EnumSymbol* tmp = this;
  int tally = 0;

  while (tmp) {
    if (tmp->init) {
      tally = tmp->init->intVal();
    }
    tmp->val = tally++;
    Symboltable::define(tmp);
    tmp = nextLink(EnumSymbol, tmp);
  }
}


ModuleSymbol::ModuleSymbol(char* init_name) :
  Symbol(SYMBOL_MODULE, init_name),
  initFn(nilFnSymbol)
{}


void ModuleSymbol::codegen(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openCFiles(filename, &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);
  fprintf(codefile, "\n");

  stmts->codegenList(codefile, "\n");

  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}
