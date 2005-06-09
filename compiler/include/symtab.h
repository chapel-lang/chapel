#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "chplenum.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"


extern ModuleSymbol* commonModule;
extern SymScope* internalScope;
extern bool _dtinteger_IndexType_switch;

class Symboltable {
 public:
  static void init(void);
  static void parseInternalPrelude(void);
  static void parsePrelude(void);
  static void doneParsingPreludes(void);
  static void doneParsingUserFiles(void);
  static bool parsingUserCode(void);

  static void traverse(SymtabTraversal* traversal);
  static void traverseFromScope(SymtabTraversal* traversal,
                                SymScope* scope);

  static void removeScope(SymScope* scope);
  static void pushScope(scopeType type);
  static SymScope* popScope(void);
  static SymScope* getCurrentScope(void);
  static SymScope* setCurrentScope(SymScope* newScope);
  static ModuleSymbol* getCurrentModule(void);
  static ModuleList* getModuleList(moduleSet modules);
  static FnSymbol* getCurrentFn(void);

  static void undefineInScope(Symbol* sym, SymScope* scope);
  static void defineInScope(Symbol* sym, SymScope* scope);
  static void define(Symbol* sym);

  /** SJD: These are the lookup's to use--
      lookupInScope only checks the passed scope
      lookupFromScope checks the passed scope and then moves up the scopes
  **/
  static Symbol* lookupInScope(char* name, SymScope* scope);
  static Symbol* lookupFromScope(char* name, SymScope* scope, 
                                 bool genError = false);

  static Symbol* lookupInCurrentScope(char* name);
  static Symbol* lookup(char* name, bool genError = false);

  static Symbol* lookupInternal(char* name, 
                                scopeType scope = SCOPE_INTERNAL_PRELUDE);
  static TypeSymbol* lookupInternalType(char* name);

  static Expr* startLetExpr(void);
  static Expr* finishLetExpr(Expr* let_expr, AList<DefExpr>* exprs, 
                             Expr* inner_expr);
  static BlockStmt* startCompoundStmt(void);
  static BlockStmt* finishCompoundStmt(BlockStmt* blkstmt, AList<Stmt>* body);
  static ModuleSymbol* startModuleDef(char* name, modType modtype = MOD_USER);
  static DefExpr* finishModuleDef(ModuleSymbol* mod, AList<Stmt>* def);
  static AList<ParamSymbol>* Symboltable::defineParams(paramType tag, 
                                                       AList<Symbol>* syms,
                                                       Type* type, Expr* init);
  static AList<DefExpr>* Symboltable::defineVarDef1(AList<Symbol>* idents,
                                                    Type* type, 
                                                    Expr* init);
  static void Symboltable::defineVarDef2(AList<DefExpr>* stmts, varType vartag, 
                                         consType constag);
  static DefStmt* Symboltable::defineSingleVarDefStmt(char* name, 
                                                      Type* type,
                                                      Expr* init,
                                                      varType vartag,
                                                      consType constag);
  static ForallExpr* startForallExpr(AList<Expr>* domainExpr, 
                                     AList<Expr>* indexExpr = new AList<Expr>());
  static ForallExpr* finishForallExpr(ForallExpr* indexExpr, 
                                      Expr* argExpr = NULL);
  // REPLACED  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init);
  static FnSymbol* startFnDef(FnSymbol* fnsym, bool noparens = false);
  static void continueFnDef(FnSymbol* fnsym, AList<ParamSymbol>* formals, 
                            Type* retType, bool isRef = false, Expr *whereExp = NULL);
  static FnSymbol* finishFnDef(FnSymbol* fnsym, BlockStmt* body, 
                               bool isExtern = true);
  static DefStmt* defineFunction(char* name, AList<ParamSymbol>* formals, 
                                 Type* retType, BlockStmt* body, 
                                 bool isExtern = false);

  static Type* defineStructType(char* name, Type* type,
                                SymScope* scope, AList<Stmt>* def);
  static ForLoopStmt* startForLoop(bool forall, AList<Symbol>* indices, 
                                   Expr* domain);
  static ForLoopStmt* finishForLoop(ForLoopStmt* forstmt, Stmt* body);

  static ForallExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

bool ModuleDefContainsOnlyNestedModules(AList<Stmt>* def);

#endif
