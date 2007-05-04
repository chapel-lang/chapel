#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

void collapseBlocks(FnSymbol* fn);
void scalarReplace(ClassType* ct);
void inlineCall(CallExpr* call);
void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
void localCopyPropagation(FnSymbol* fn);
void globalCopyPropagation(FnSymbol* fn);
void deadVariableElimination(FnSymbol* fn);
void deadExpressionElimination(FnSymbol* fn);

void liveVariableAnalysis(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localID,
                          Vec<SymExpr*>& useSet,
                          Vec<SymExpr*>& defSet,
                          Vec<Vec<bool>*>& OUT);

#endif
