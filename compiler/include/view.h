#ifndef _VIEW_H_
#define _VIEW_H_

#include "baseAST.h"
#include "vec.h"

// These are used by the compiler itself
BaseAST*    aid(int id);

void        list_view_noline(BaseAST* ast);
void        nprint_view(BaseAST* ast);

// These are not used by the compiler but are available for use in GDB
//
// The most commonly used functions are referenced in
// $CHPL_HOME/compiler/etc/gdb.commands and appear to be
//
//    aid
//    print_view
//    iprint_view
//    nprint_view
//    list_view
//    viewFlags
//    stringLoc

void        print_view(BaseAST* ast);
void        print_view_noline(BaseAST* ast);

void        iprint_view(int id);

void        nprint_view(int id);
void        nprint_view_noline(BaseAST* ast);

void        mark_view(BaseAST* ast, int id);

void        list_view(int id);
void        list_view(BaseAST* ast);

void        viewFlags(int id);

void        map_view(SymbolMap* map);
void        map_view(SymbolMap& map);

void        vec_view(Vec<Symbol*,   VEC_INTEGRAL_SIZE>* v);
void        vec_view(Vec<Symbol*,   VEC_INTEGRAL_SIZE>& v);
void        vec_view(Vec<FnSymbol*, VEC_INTEGRAL_SIZE>* v);
void        vec_view(Vec<FnSymbol*, VEC_INTEGRAL_SIZE>& v);

void        fnsWithName(const char* name);
void        fnsWithName(const char* name, Vec<FnSymbol*>& fnVec);

void        whocalls(int id);
void        whocalls(BaseAST* ast);

// NB these return the same static buffer
const char* stringLoc(int id);
const char* stringLoc(BaseAST* ast);

const char* shortLoc(int id);
const char* shortLoc(BaseAST* ast);

const char* debugLoc(int id);
const char* debugLoc(BaseAST* ast);

#endif
