#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

// If DEBUG_CP > 0, then the debug code is enabled.  Inside of
// copyPropagation.cpp, one still has to set debug=1 to obtain verbose debug
// output.
// IF DEBUG_CP > 1, then calls to copy propagation from other passes are also
// disabled.
// localCopyPropagation is called from several optimization passes, so DEBUG_CP
// was hoisted into this common include file for convenience.
#define DEBUG_CP 0

#include <map>

class BitVec;

void collapseBlocks(BlockStmt* block);
void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
size_t localCopyPropagation(FnSymbol* fn);
size_t globalCopyPropagation(FnSymbol* fn);
void eliminateSingleAssignmentReference(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                        Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                        Symbol* var);
size_t singleAssignmentRefPropagation(FnSymbol* fn);
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
