/*
 * Copyright 2004-2020 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ASTUTIL_H_
#define _ASTUTIL_H_

#include "baseAST.h"
#include "alist.h"

#include <vector>
#include <set>

class Type;
class FnSymbol;
class VarSymbol;
class ArgSymbol;
class BlockStmt;
class CallExpr;
class SymExpr;
class Expr;

// return vec of CallExprs of FnSymbols (no primitives)
void collectFnCalls(BaseAST* ast, std::vector<CallExpr*>& calls);
// specialized helper for IBBs
void collectTreeBoundGotosAndIteratorBreakBlocks(BaseAST* ast,
                                                 std::vector<GotoStmt*>& GOTOs,
                                                 std::vector<CondStmt*>& IBBs);

// collect Stmts and Exprs in the AST and return them in vectors
void collect_asts(BaseAST* ast, std::vector<BaseAST*>& asts);
void collect_asts_postorder(BaseAST*, std::vector<BaseAST*>& asts);
void collect_top_asts(BaseAST* ast, std::vector<BaseAST*>& asts);
void collectExprs(BaseAST* ast, std::vector<Expr*>& exprs);
void collect_stmts(BaseAST* ast, std::vector<Expr*>& stmts);
void collectDefExprs(BaseAST* ast, std::vector<DefExpr*>& defExprs);
void collectForallStmts(BaseAST* ast, std::vector<ForallStmt*>& forallStmts);
void collectCallExprs(BaseAST* ast, std::vector<CallExpr*>& callExprs);
void collectMyCallExprs(BaseAST* ast,
                        std::vector<CallExpr*>& callExprs,
                        FnSymbol* fn);
void collectGotoStmts(BaseAST* ast, std::vector<GotoStmt*>& gotoStmts);
void collectSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs);
void collectSymExprsFor(BaseAST* ast, Symbol* sym, std::vector<SymExpr*>& symExprs);
void collectSymExprsFor(BaseAST* ast, const Symbol* sym1, const Symbol* sym2,
                        std::vector<SymExpr*>& symExprs);
void collectLcnSymExprs(BaseAST* ast, std::vector<SymExpr*>& symExprs);
void collectSymbols(BaseAST* ast, std::vector<Symbol*>& symbols);

// returns true if the AST contains a SymExpr pointing to sym
bool containsSymExprFor(BaseAST* ast, Symbol* sym);

// utility routines for clearing and resetting lineno and filename
void reset_ast_loc(BaseAST* destNode, astlocT astloc);
void reset_ast_loc(BaseAST* destNode, BaseAST* sourceNode);

void compute_call_sites();
void computeNonvirtualCallSites(FnSymbol* fn);
void computeAllCallSites(FnSymbol* fn);

//
// collect set of symbols and vector of SymExpr; can be used to
// compute defMaps and useMaps below (these are computed when
// computing the defMaps and useMaps; precomputing them can help if
// you need to use them elsewhere too)
//
void collectSymbolSetSymExprVec(BaseAST* ast,
                                Vec<Symbol*>& symSet,
                                Vec<SymExpr*>& symExprs);

//
// collect set of symbols
//
void collectSymbolSet(BaseAST* ast, Vec<Symbol*>& symSet);
void collectSymbolSet(BaseAST* ast, std::set<Symbol*>& symSet);


//
// Checks if a callExpr is one of the op= primitives
// Note, this does not check if a callExpr is an
// op= function call (such as before inlining)
//
bool isOpEqualPrim(CallExpr* call);

bool isMoveOrAssign(CallExpr* call);

bool isDerefMove(CallExpr* call);

//
// Checks if a callExpr is a relational operator (<, <=, >, >=, ==, !=)
//
bool isRelationalOperator(CallExpr* call);

//
// Return value & 1 is true if se is a def
// Return value & 2 is true if se is a use
//
int isDefAndOrUse(SymExpr* se);

//
// build defMap and useMap such that defMap is a map from symbols to
// their defs and useMap is a map from symbols to their uses; these
// vectors are built differently depending on the other arguments
//

// builds the vectors for every variable/argument in 'symSet'
void buildDefUseMaps(Vec<Symbol*>& symSet,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap);

// builds the vectors for every variable/argument in the entire
// program
void buildDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap);

// builds the vectors for every variable/argument in 'fn' and looks
// for uses and defs only in 'fn'
void buildDefUseMaps(FnSymbol* fn,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap);

// builds the vectors for every variable declaration in the given block
// and looks for uses and defs within the same block (scope).
void buildDefUseMaps(BlockStmt* block,
                     Map<Symbol*,Vec<SymExpr*>*>& defMap,
                     Map<Symbol*,Vec<SymExpr*>*>& useMap);


//
// add a def to a defMap or a use to a useMap
//
void addDef(Map<Symbol*,Vec<SymExpr*>*>& defMap, SymExpr* def);
void addUse(Map<Symbol*,Vec<SymExpr*>*>& useMap, SymExpr* use);


//
// free memory consumed by defMap and useMap
//
void freeDefUseMaps(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                    Map<Symbol*,Vec<SymExpr*>*>& useMap);

//
// stylized loops over defs and uses: to loop over the defs/uses
// (SymExprs) stored in the map defMap/useMap for a symbol sym, use
// for_defs/for_uses and the resulting declared variable def/use will
// contain the defs/uses
//
#define for_defs(def, defMap, sym)                \
  for_uses(def, defMap, sym)

#define for_uses(use, useMap, sym)                \
  if (Vec<SymExpr*>* macro_vec = useMap.get(sym)) \
    forv_Vec(SymExpr, use, *macro_vec)

//
// build useSet and defSet for a vector of symbols 'syms' where the
// uses and defs are restricted to 'fn' such that the set useSet
// contains all of the uses of the symbols in 'syms' that occur in
// 'fn' and the set defSet contains all of the defs of the symbols in
// 'syms' that occur in 'fn'
//
void buildDefUseSets(Vec<Symbol*>& syms,
                     FnSymbol* fn,
                     Vec<SymExpr*>& defSet,
                     Vec<SymExpr*>& useSet);

// replace symbol use with another
void subSymbol(BaseAST* ast, Symbol* oldSym, Symbol* newSym);

void remove_help(BaseAST* ast, int trace_flag);
void parent_insert_help(BaseAST* parent, Expr* ast);
void sibling_insert_help(BaseAST* sibling, BaseAST* ast);
void insert_help(BaseAST* ast, Expr* parentExpr, Symbol* parentSymbol);

ArgSymbol* actual_to_formal( Expr *a);
Expr* formal_to_actual(CallExpr* call, Symbol* formal);

bool isTypeExpr(Expr* expr);
bool givesType(Symbol* sym);

Symbol* getSvecSymbol(CallExpr* call);
void collectUsedFnSymbols(BaseAST* ast, std::set<FnSymbol*>& fnSymbols);

void convertToQualifiedRefs();

#endif
