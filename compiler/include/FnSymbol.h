/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _FN_SYMBOL_H_
#define _FN_SYMBOL_H_

#include "symbol.h"

enum RetTag {
  RET_VALUE,
  RET_REF,
  RET_CONST_REF,
  RET_PARAM,
  RET_TYPE
};

class FnSymbol : public Symbol {
public:
  // each formal is an ArgSymbol, but the elements are DefExprs
  AList                      formals;

  // The return type of the function. This field is not fully established
  // until function resolution, and could be NULL before then.  Up to that
  // point, return type information is stored in the retExprType field.
  Type*                      retType;

  BlockStmt*                 where;
  BlockStmt*                 retExprType;
  BlockStmt*                 body;
  IntentTag                  thisTag;
  RetTag                     retTag;

  // Attached original (user) iterators before lowering.
  IteratorInfo*              iteratorInfo;

  Symbol*                    _this;
  Symbol*                    _outer;
  FnSymbol*                  instantiatedFrom;
  SymbolMap                  substitutions;
  BlockStmt*                 instantiationPoint;
  std::vector<BasicBlock*>*  basicBlocks;
  Vec<CallExpr*>*            calledBy;
  const char*                userString;

  // pointer to value function (created in function resolution
  // and used in cullOverReferences)
  FnSymbol*                  valueFunction;

  int                        codegenUniqueNum;
  const char*                doc;

  // Used to store the return symbol during partial copying.
  Symbol*                    retSymbol;

  // Number of formals before tuple type constructor formals are added.
  int                        numPreTupleFormals;

#ifdef HAVE_LLVM
  llvm::MDNode*              llvmDISubprogram;
#else
  void*                      llvmDISubprogram;
#endif


                             FnSymbol(const char* initName);
                            ~FnSymbol();

  void                       verify();
  virtual void               accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(FnSymbol);

  FnSymbol*                  copyInnerCore(SymbolMap* map);
  void                       replaceChild(BaseAST* oldAst, BaseAST* newAst);

  FnSymbol*                  partialCopy(SymbolMap* map);
  void                       finalizeCopy();

  // Returns an LLVM type or a C-cast expression
  GenRet                     codegenFunctionType(bool forHeader);
  GenRet                     codegenCast(GenRet fnPtr);

  GenRet                     codegen();
  void                       codegenHeaderC();
  void                       codegenPrototype();
  void                       codegenDef();

  void                       insertAtHead(Expr* ast);
  void                       insertAtHead(const char* format, ...);

  void                       insertAtTail(Expr* ast);
  void                       insertAtTail(const char* format, ...);

  void                       insertFormalAtHead(BaseAST* ast);
  void                       insertFormalAtTail(BaseAST* ast);

  void                       insertBeforeEpilogue(Expr* ast);

  // insertIntoEpilogue adds an Expr before the final return,
  // but after the epilogue label
  void                       insertIntoEpilogue(Expr* ast);

  LabelSymbol*               getEpilogueLabel();
  LabelSymbol*               getOrCreateEpilogueLabel();

  Symbol*                    getReturnSymbol();
  Symbol*                    replaceReturnSymbol(Symbol* newRetSymbol,
                                                 Type*   newRetType);

  int                        numFormals()                                const;
  ArgSymbol*                 getFormal(int i)                            const;

  void                       collapseBlocks();

  CallExpr*                  singleInvocation()                          const;

  bool                       tagIfGeneric();

  bool                       isNormalized()                              const;
  void                       setNormalized(bool value);

  bool                       isResolved()                                const;

  bool                       isMethod()                                  const;
  bool                       isMethodOnClass()                           const;
  bool                       isMethodOnRecord()                          const;

  void                       setMethod(bool value);

  bool                       isPrimaryMethod()                           const;
  bool                       isSecondaryMethod()                         const;

  bool                       isInitializer()                             const;
  bool                       isPostInitializer()                         const;

  AggregateType*             getReceiver()                               const;

  bool                       isIterator()                                const;

  bool                       returnsRefOrConstRef()                      const;

  QualifiedType              getReturnQualType()                         const;

  virtual void               printDocs(std::ostream* file,
                                       unsigned int  tabs);

  void                       throwsErrorInit();
  bool                       throwsError()                               const;

  bool                       retExprDefinesNonVoid()                     const;

private:
  virtual std::string        docsDirective();

  int                        hasGenericFormals()                         const;

  bool                       mIsNormalized;
  bool                       _throwsError;
};

const char*                     toString(FnSymbol* fn);

const char*                     retTagDescrString(RetTag retTag);

extern FnSymbol*                initStringLiterals;

extern FnSymbol*                chpl_gen_main;

extern FnSymbol*                gAddModuleFn;

extern FnSymbol*                gGenericTupleTypeCtor;
extern FnSymbol*                gGenericTupleInit;
extern FnSymbol*                gGenericTupleDestroy;

extern std::map<FnSymbol*, int> ftableMap;
extern std::vector<FnSymbol*>   ftableVec;


#endif
