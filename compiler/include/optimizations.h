#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

void collapseBlocks(FnSymbol* fn);
void inlineCall(CallExpr* call);
void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
void localCopyPropagation(FnSymbol* fn);
void localReferencePropagation(FnSymbol* fn);
void globalCopyPropagation(FnSymbol* fn);
void deadVariableElimination(FnSymbol* fn);
void deadExpressionElimination(FnSymbol* fn);
void deadCodeElimination(FnSymbol* fn);

void liveVariableAnalysis(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localID,
                          Vec<SymExpr*>& useSet,
                          Vec<SymExpr*>& defSet,
                          Vec<Vec<bool>*>& OUT);

void reachingDefinitionsAnalysis(FnSymbol* fn,
                                 Vec<SymExpr*>& defs,
                                 Map<SymExpr*,int>& defMap,
                                 Vec<SymExpr*>& useSet,
                                 Vec<SymExpr*>& defSet,
                                 Vec<Vec<bool>*>& IN);

void
buildDefUseChains(FnSymbol* fn,
                  Map<SymExpr*,Vec<SymExpr*>*>& DU,
                  Map<SymExpr*,Vec<SymExpr*>*>& UD);

void
freeDefUseChains(Map<SymExpr*,Vec<SymExpr*>*>& DU,
                 Map<SymExpr*,Vec<SymExpr*>*>& UD);

#endif
