#include "AstLogger.h"



AstLogger::AstLogger() {

}

AstLogger::~AstLogger() {
}

bool AstLogger::enterAggrType(AggregateType* node) {
  return true;
}

void AstLogger::exitAggrType(AggregateType* node) {
}

bool AstLogger::enterEnumType(EnumType* node) {
  return true;
}

void AstLogger::exitEnumType(EnumType* node) {
}

void AstLogger::visitPrimType(PrimitiveType* node) {
}

bool AstLogger::enterArgSym(ArgSymbol* node) {
  return true;
}

void AstLogger::exitArgSym(ArgSymbol* node) {
}

void AstLogger::visitEnumSym(EnumSymbol* node) {
}

bool AstLogger::enterFnSym(FnSymbol* node) {
  return true;
}

void AstLogger::exitFnSym(FnSymbol* node) {
}

void AstLogger::visitLabelSym(LabelSymbol* node) {
}

bool AstLogger::enterModSym(ModuleSymbol* node) {
  return true;
}

void AstLogger::exitModSym(ModuleSymbol* node) {
}

bool AstLogger::enterTypeSym(TypeSymbol* node) {
  return true;
}

void AstLogger::exitTypeSym(TypeSymbol* node) {
}

void AstLogger::visitVarSym(VarSymbol* node) {
}

bool AstLogger::enterCallExpr(CallExpr* node) {
  return true;
}

void AstLogger::exitCallExpr(CallExpr* node) {
}

bool AstLogger::enterDefExpr(DefExpr* node) {
  return true;
}

void AstLogger::exitDefExpr(DefExpr* node) {
}

bool AstLogger::enterNamedExpr(NamedExpr* node) {
  return true;
}

void AstLogger::exitNamedExpr(NamedExpr* node) {
}

void AstLogger::visitSymExpr(SymExpr* node) {
}

void AstLogger::visitUsymExpr(UnresolvedSymExpr* node) {
}

bool AstLogger::enterBlockStmt(BlockStmt* node) {
  return true;
}

void AstLogger::exitBlockStmt(BlockStmt* node) {
}

bool AstLogger::enterCondStmt(CondStmt* node) {
  return true;
}

void AstLogger::exitCondStmt(CondStmt* node) {
}

void AstLogger::visitEblockStmt(ExternBlockStmt* node) {
}

bool AstLogger::enterGotoStmt(GotoStmt* node) {
  return true;
}

void AstLogger::exitGotoStmt(GotoStmt* node) {
}
