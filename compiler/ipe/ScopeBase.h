/************************************ | *************************************
*                                                                           *
* Maintain lexical scoping information for AST definitions to support name  *
* resolution for the IPE.                                                   *
*                                                                           *
* It is important to note that these scopes are similar to but distinct     *
* from the dynamic environments that will be maintained by the IPE's        *
* runtime.                                                                  *
*                                                                           *
************************************* | ************************************/

#ifndef _SCOPE_BASE_H_
#define _SCOPE_BASE_H_

#include <cstdio>
#include <vector>

class BlockStmt;
class Expr;
class ModuleSymbol;
class ScopeModule;
class Symbol;
class UnresolvedSymExpr;
class VisibleSymbol;

class ScopeBase
{
public:
                                  ScopeBase(const ScopeBase* parent);
                                 ~ScopeBase();

  void                            extend(BlockStmt* block);
  void                            describe(bool recursive = false)                      const;

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& symbols)   const;

protected:
  void                            addSym(Symbol*       symbol);
  void                            addUse(ModuleSymbol* module);


private:
                                  ScopeBase();

  virtual void                    describeHeader(FILE* fp)                              const = 0;

  void                            extend(BlockStmt*                  block,
                                         std::vector<ModuleSymbol*>& visited);

  void                            addUse(Expr*                       expr);

  void                            addUse(ModuleSymbol*               module,
                                         std::vector<ModuleSymbol*>& visited);


  bool                            isConflicted(const Symbol*       sym)                 const;
  bool                            isConflicted(const ModuleSymbol* mod)                 const;

  bool                            isVisited(ModuleSymbol*               module,
                                            std::vector<ModuleSymbol*>& visited);

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& symbols,
                                                 int                         distance)  const;

  ModuleSymbol*                   moduleByName(Expr* expr)                              const;

  const ScopeBase*                mParent;
  std::vector<ScopeModule*>       mUsedScopes;
  std::vector<Symbol*>            mSymbols;
};

#endif
