#include "AstLogger.h"



AstLogger::AstLogger() {

}

AstLogger::~AstLogger() {
}

bool AstLogger::visitEnter(AggregateType* node) {
  return true;
}

void AstLogger::visitExit(AggregateType* node) {
}

bool AstLogger::visitEnter(EnumType* node) {
  return true;
}

void AstLogger::visitExit(EnumType* node) {
}

void AstLogger::visit(PrimitiveType* node) {
}

bool AstLogger::visitEnter(ArgSymbol* node) {
  return true;
}

void AstLogger::visitExit(ArgSymbol* node) {
}

void AstLogger::visit(EnumSymbol* node) {
}

bool AstLogger::visitEnter(FnSymbol* node) {
  return true;
}

void AstLogger::visitExit(FnSymbol* node) {
}

void AstLogger::visit(LabelSymbol* node) {
}

bool AstLogger::visitEnter(ModuleSymbol* node) {
  return true;
}

void AstLogger::visitExit(ModuleSymbol* node) {
}

bool AstLogger::visitEnter(TypeSymbol* node) {
  return true;
}

void AstLogger::visitExit(TypeSymbol* node) {
}

void AstLogger::visit(VarSymbol* node) {
}

bool AstLogger::visitEnter(CallExpr* node) {
  return true;
}

void AstLogger::visitExit(CallExpr* node) {
}

bool AstLogger::visitEnter(DefExpr* node) {
  return true;
}

void AstLogger::visitExit(DefExpr* node) {
}

bool AstLogger::visitEnter(NamedExpr* node) {
  return true;
}

void AstLogger::visitExit(NamedExpr* node) {
}

void AstLogger::visit(SymExpr* node) {
}

void AstLogger::visit(UnresolvedSymExpr* node) {
}

bool AstLogger::visitEnter(BlockStmt* node) {
  return true;
}

void AstLogger::visitExit(BlockStmt* node) {
}

bool AstLogger::visitEnter(CondStmt* node) {
  return true;
}

void AstLogger::visitExit(CondStmt* node) {
}

void AstLogger::visit(ExternBlockStmt* node) {
}

bool AstLogger::visitEnter(GotoStmt* node) {
  return true;
}

void AstLogger::visitExit(GotoStmt* node) {
}
