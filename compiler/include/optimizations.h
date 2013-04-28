#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

#include <map>

class BitVec;

void collapseBlocks(BlockStmt* block);
void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
void localCopyPropagation(FnSymbol* fn);
void localReferencePropagation(FnSymbol* fn);
void globalCopyPropagation(FnSymbol* fn);
void eliminateSingleAssignmentReference(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                        Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                        Symbol* var);
void singleAssignmentRefPropagation(FnSymbol* fn);
void deadVariableElimination(FnSymbol* fn);
void deadExpressionElimination(FnSymbol* fn);
void deadCodeElimination(FnSymbol* fn);

void liveVariableAnalysis(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localID,
                          Vec<SymExpr*>& useSet,
                          Vec<SymExpr*>& defSet,
                          std::vector<BitVec*>& OUT);

void
buildDefUseChains(FnSymbol* fn,
                  std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                  std::map<SymExpr*,Vec<SymExpr*>*>& UD);

void
freeDefUseChains(std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                 std::map<SymExpr*,Vec<SymExpr*>*>& UD);

void
remoteValueForwarding(Vec<FnSymbol*>& fns);

void
narrowWideReferences();

// Debug functions from narrowWideReferences.cpp
void printNode(BaseAST*);
void printNode(int id);

#endif
