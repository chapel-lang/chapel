/*
 * Copyright 2004-2015 Cray Inc.
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

// oldCollectors.h
//
// It is desired to deprecate the collect...() functions below in favor of
// their STL-based counter parts that still appear in astutil.h
// 
// Please do not include this .h file nor use these functions in new code.
//

#ifdef _OLDCOLLECTORS_H
#error Multiple inclusion
#else
#define _OLDCOLLECTORS_H

void collectFnCalls(BaseAST* ast, Vec<CallExpr*>& calls);
void collect_asts(BaseAST* ast, Vec<BaseAST*>& asts);
void collect_asts_postorder(BaseAST*, Vec<BaseAST*>& asts);
void collect_top_asts(BaseAST* ast, Vec<BaseAST*>& asts);
void collect_stmts(BaseAST* ast, Vec<Expr*>& stmts);
void collectDefExprs(BaseAST* ast, Vec<DefExpr*>& defExprs);
void collectCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs);
void collectMyCallExprs(BaseAST* ast, Vec<CallExpr*>& callExprs, FnSymbol* fn);
void collectGotoStmts(BaseAST* ast, Vec<GotoStmt*>& gotoStmts);
void collectSymExprs(BaseAST* ast, Vec<SymExpr*>& symExprs);
void collectMySymExprs(Symbol* me, Vec<SymExpr*>& symExprs);
void collectSymbols(BaseAST* ast, Vec<Symbol*>& symbols);


#endif
