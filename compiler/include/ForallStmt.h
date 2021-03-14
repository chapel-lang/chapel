/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _FORALL_STMT_H_
#define _FORALL_STMT_H_

#include "stmt.h"

enum ForallAutoLocalAccessCloneType {
  NOT_CLONE,
  NO_OPTIMIZATION,
  STATIC_ONLY,
  STATIC_AND_DYNAMIC
};

class ForallOptimizationInfo {
  public:
    bool infoGathered;

    std::vector<Symbol *> iterSym;
    std::vector<Expr *> dotDomIterExpr;
    std::vector<Symbol *> dotDomIterSym;
    std::vector<Symbol *> dotDomIterSymDom;

    std::vector<CallExpr *> iterCall;  // refers to the original CallExpr
    std::vector<Symbol *> iterCallTmp; // this is the symbol to use for checks

    // even if there are multiple indices we store them in a vector
    std::vector< std::vector<Symbol *> > multiDIndices;

    // calls in the loop that are candidates for optimization
    std::vector< std::pair<CallExpr *, int> > staticCandidates;
    std::vector< std::pair<CallExpr *, int> > dynamicCandidates;

    // the static check control symbol added for symbol
    std::map<Symbol *, Symbol *> staticCheckSymForSymMap;
    
    // the dynamic check call added for symbol
    std::map<Symbol *, CallExpr *> dynamicCheckForSymMap;

    std::vector<Symbol *> staticCheckSymsForDynamicCandidates;

    bool autoLocalAccessChecked;
    bool hasAlignedFollowers;

    ForallAutoLocalAccessCloneType cloneType;

    ForallOptimizationInfo();
};

///////////////////////////////////
    // forall loop statement //
///////////////////////////////////

class ForallStmt final : public Stmt
{
public:
  bool       zippered()       const; // 'zip' keyword used and >1 index var
  AList&     inductionVariables();   // DefExprs, one per iterated expr
  const AList& constInductionVariables() const; // const counterpart
  AList&     iteratedExpressions();  // Exprs, one per iterated expr
  const AList& constIteratedExpressions() const;  // const counterpart
  AList&     shadowVariables();      // DefExprs of ShadowVarSymbols
  BlockStmt* loopBody()       const; // the body of the forall loop
  std::vector<BlockStmt*> loopBodies() const; // body or bodies of followers
  LabelSymbol* continueLabel();      // create it if not already
  CallExpr* zipCall() const;

  // when originating from a ForLoop or a reduce expression
  bool createdFromForLoop()     const;  // is converted from a for-loop
  bool needToHandleOuterVars()  const;  // yes, convert to shadow vars
  bool needsInitialAccumulate() const;  // for a reduce intent
  bool fromReduce()             const;  // for a Chapel reduce expression
  bool overTupleExpand()        const;  // contains (...tuple) iterable(s)
  bool allowSerialIterator()    const;  // ok to loop over a serial iterator?
  bool requireSerialIterator()  const;  // do not seek standalone or leader

  DECLARE_COPY(ForallStmt);
  ForallStmt* copyInner(SymbolMap* map) override;


  void        verify() override;
  void        accept(AstVisitor* visitor) override;
  GenRet      codegen() override;

  void        replaceChild(Expr* oldAst, Expr* newAst) override;
  Expr*       getFirstExpr() override;
  Expr*       getNextExpr(Expr* expr) override;
  void        setZipCall(CallExpr *call);

  static ForallStmt* buildHelper(Expr* indices, Expr* iterator,
                                 CallExpr* intents, BlockStmt* body,
                                 bool zippered, bool fromForLoop);

  static BlockStmt*  build(Expr* indices, Expr* iterator, CallExpr* intents,
                           BlockStmt* body, bool zippered, bool serialOK);

  static ForallStmt* fromForLoop(ForLoop* forLoop);

  static ForallStmt* fromReduceExpr(VarSymbol* idx, SymExpr* dataExpr,
                                    ShadowVarSymbol* svar,
                                    bool zippered, bool requireSerial);

  // helpers

  int numInductionVars()  const;
  int numIteratedExprs()  const;
  int numShadowVars()     const;

  DefExpr* firstInductionVarDef() const;
  Expr*    firstIteratedExpr()    const;
  void setNotZippered();
  bool isReduceIntent(Symbol* var) const;
  bool hasVectorizationHazard() const;
  void setHasVectorizationHazard(bool v);

  // indicates a forall expression (vs a forall statement)
  bool isForallExpr() const;

  ForallOptimizationInfo optInfo;

  void insertZipSym(Symbol *sym);

private:
  AList          fIterVars;
  AList          fIterExprs;
  AList          fShadowVars;  // may be empty
  BlockStmt*     fLoopBody;    // always present
  bool           fZippered;
  CallExpr*      fZipCall;
  bool           fFromForLoop; // see comment below
  bool           fFromReduce;
  bool           fOverTupleExpand;
  bool           fAllowSerialIterator;
  bool           fRequireSerialIterator;
  bool           fVectorizationHazard;
  bool           fIsForallExpr;

  // constructor
  ForallStmt(BlockStmt* body);

public:
  LabelSymbol*            fContinueLabel;
  LabelSymbol*            fErrorHandlerLabel;

  // for recursive iterators during lowerIterators
  DefExpr*       fRecIterIRdef;
  DefExpr*       fRecIterICdef;
  CallExpr*      fRecIterGetIterator;
  CallExpr*      fRecIterFreeIterator;
};

/*
fFromForLoop and its accessors

These support handling of some ForLoops by converting them to ForallStmts.
They cause skipping certain actions for these "conversion" ForallStmt nodes.

Why not just have a single accessor to fFromForLoop? This is to emphasize
that the three accessors check different properties. These properties could
potentially be independent of each other.

fFromReduce and its accessors

These support handling of reduce exprs by converting them to ForallStmts.
Same idea as fFromForLoop.
*/

/// accessor implementations ///

inline bool   ForallStmt::zippered()         const { return fZippered;   }
inline AList& ForallStmt::inductionVariables()     { return fIterVars;   }
inline const AList& ForallStmt::constInductionVariables() const {
  return fIterVars;
}
inline AList& ForallStmt::iteratedExpressions()    { return fIterExprs;  }
inline const AList& ForallStmt::constIteratedExpressions() const {
  return fIterExprs;
}
inline AList& ForallStmt::shadowVariables()        { return fShadowVars; }
inline BlockStmt* ForallStmt::loopBody()     const { return fLoopBody;   }

inline CallExpr* ForallStmt::zipCall()       const { return fZipCall;    }

inline bool ForallStmt::needToHandleOuterVars() const { return !fFromForLoop; }
inline bool ForallStmt::createdFromForLoop()    const { return  fFromForLoop; }
inline bool ForallStmt::needsInitialAccumulate()const { return !fFromReduce;  }
inline bool ForallStmt::fromReduce()            const { return  fFromReduce;  }
inline bool ForallStmt::overTupleExpand()       const { return fOverTupleExpand;       }
inline bool ForallStmt::allowSerialIterator()   const { return fAllowSerialIterator;   }
inline bool ForallStmt::requireSerialIterator() const { return fRequireSerialIterator; }

/// conveniences ///

inline int   ForallStmt::numInductionVars()  const { return fIterVars.length; }
inline int   ForallStmt::numIteratedExprs()  const { return fIterExprs.length;}
inline int   ForallStmt::numShadowVars()     const { return fShadowVars.length;}
inline Expr* ForallStmt::firstIteratedExpr() const { return fIterExprs.head;  }
inline DefExpr* ForallStmt::firstInductionVarDef() const { return toDefExpr(fIterVars.head); }

#define for_shadow_var_defs(SVD,TEMP,FS)    \
  for_alist(TEMP,(FS)->shadowVariables())   \
    if (DefExpr* SVD = toDefExpr(TEMP))

#define for_shadow_vars_and_defs(SV,DEF,TEMP,FS)           \
  for_shadow_var_defs(DEF,TEMP,FS)                         \
    if (ShadowVarSymbol* SV = toShadowVarSymbol(DEF->sym))

#define for_shadow_vars(SV,TEMP,FS)         \
  for_shadow_vars_and_defs(SV,SVD,TEMP,FS)

/// helpers ///

ForallStmt* enclosingForallStmt(Expr* expr);
ForallStmt* isForallIterVarDef(Expr* expr);
ForallStmt* isForallIterExpr(Expr* expr);
ForallStmt* isForallRecIterHelper(Expr* expr);
ForallStmt* isForallLoopBody(Expr* expr);
const ForallStmt* isConstForallLoopBody(const Expr* expr);
VarSymbol*  parIdxVar(ForallStmt* fs);

QualifiedType fsIterYieldType(Expr* ref, FnSymbol* iterFn);
bool fsGotFollower(Expr* anchor, Symbol* followThis, Symbol* iterSym);
void fsCheckNumIdxVarsVsIterables(ForallStmt* fs, int numIdx, int numIter);

/// done ///

#endif
