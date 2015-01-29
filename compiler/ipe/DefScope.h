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

#ifndef _DEF_SCOPE_H_
#define _DEF_SCOPE_H_

#include <vector>

class DefExpr;
class ModuleSymbol;
class UnresolvedSymExpr;
class VisibleSymbol;

class DefScope
{
  // Class state
public:
  static DefScope*                createRoot();
  static DefScope*                extend(const DefScope* parent);

private:
  static std::vector<DefScope*>   mScopes;

  // Instance state
public:
                                 ~DefScope();

  int                             depthGet()                                            const;
  void                            depthSet(int depth);

  void                            addDefinition (DefExpr*      expr);

  void                            extendByModule(ModuleSymbol* module);

  void                            visibleSymbols(UnresolvedSymExpr*          unres,
                                                 std::vector<VisibleSymbol>& functions) const;

private:
                                  DefScope();
                                  DefScope(const DefScope* parent);

  const DefScope*                 mParent;
  int                             mDepth;

  std::vector<DefExpr*>           mDefinitions;
};

#endif
