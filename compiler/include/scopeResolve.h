#ifndef SCOPE_RESOLVE_H
#define SCOPE_RESOLVE_H

class AggregateType;


void build_constructors(AggregateType* ct);
void add_root_type(AggregateType* ct);

#ifdef HAVE_LLVM

#include "vec.h"

class Expr;
class ModuleSymbol;

// this one is defined in externCResolve.cpp
void convertDeclToChpl(ModuleSymbol* module, const char* name, Vec<Expr*>& addedToAST);
#endif

// this one is defined in util/clangUtil.cpp
void cleanupExternC();

#endif
