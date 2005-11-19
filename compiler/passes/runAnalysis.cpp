#include "alist.h"
#include "analysis.h"
#include "driver.h"
#include "filesToAST.h"
#include "runAnalysis.h"
#include "symbol.h"
#include "symtab.h"

bool preAnalysis = true;
bool inAnalysis = false;
bool postAnalysis = false;

void runAnalysis(void) {
  preAnalysis = false;
  inAnalysis = true;
  Vec<AList<Stmt> *> stmts;
  forv_Vec(ModuleSymbol, mod, allModules) {
    stmts.add(mod->stmts);
  }
  AST_to_IF1(stmts);
  // BLC: John, what filename should be passed in for multiple modules?
  // I'm just passing in the first non-internal module's filename
  // JBP: that's fine, it is only used for debug, HTML and low level cg files
  char* firstUserModuleName = NULL;
  forv_Vec(ModuleSymbol, mod, allModules) {
    if (mod->modtype == MOD_USER) {
      firstUserModuleName = mod->filename;
    }
  }
  //driver:do_analysis
  do_analysis(firstUserModuleName);
#if 0
  // test type_is_used
  forv_Sym(s, if1->allsyms) {
    if (s->type_kind) {
      if (s->asymbol->symbol && s->name && 
          dynamic_cast<Type*>(s->asymbol->symbol) &&
          dynamic_cast<TypeSymbol*>(dynamic_cast<Type*>(s->asymbol->symbol)->name))
        printf("%s is_used: %d\n", s->name, 
               type_is_used(dynamic_cast<TypeSymbol*>(dynamic_cast<Type*>(s->asymbol->symbol)->name)));
    }
  }
#endif
  inAnalysis = false;
  postAnalysis = true;
}
