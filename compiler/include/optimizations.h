#ifndef _OPTIMIZATIONS_H_
#define _OPTIMIZATIONS_H_

void scalarReplace(TypeSymbol* ts);
void compressUnnecessaryScopes(FnSymbol* fn);
void removeUnnecessaryGotos(FnSymbol* fn);
void removeUnusedLabels(FnSymbol* fn);
void localCopyPropagation(FnSymbol* fn);
void deadVariableElimination(FnSymbol* fn);
void deadExpressionElimination(FnSymbol* fn);

#endif
