#ifndef SCOPE_RESOLVE_H
#define SCOPE_RESOLVE_H


void build_constructors(ClassType* ct);
void add_root_type(ClassType* ct);

#ifdef HAVE_LLVM
// this one is defined in externCResolve.cpp
void convertDeclToChpl(ModuleSymbol* module, const char* name, Vec<Expr*> & addedToAST);
#endif

// this one is defined in util/clangUtil.cpp
void cleanupExternC(void);

#endif
