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

#ifndef _FN_SYMBOL_H_
#define _FN_SYMBOL_H_

#include "library.h"
#include "symbol.h"

class IteratorGroup;     // see iterator.h
class GenericsCacheInfo; // see caches.h
void cleanupCacheInfo(FnSymbol* fn);

enum RetTag {
  RET_VALUE,
  RET_REF,
  RET_CONST_REF,
  RET_PARAM,
  RET_TYPE
};

enum TagGenericResult {
  TGR_ALREADY_TAGGED,
  TGR_NEWLY_TAGGED,
  TGR_TAGGING_ABORTED
};

// Additional data for interface constraints / constrained generics / CG.
// Stored in a FnSymbol for a constrained-generic function.
class InterfaceInfo {
public:
  InterfaceInfo(FnSymbol* parent);
  void addConstrainedType(DefExpr* def);
  void addInterfaceConstraint(IfcConstraint* icon);

  // each element is a DefExpr of a ConstrainedType
  // that is used in the type of this function's formal
  AList constrainedTypes;

  // each element is an IfcConstraint representing
  // an interface constraint of this function
  AList interfaceConstraints;

  // contains one SymbolMap per IfcConstraint in 'interfaceConstraints'
  // with mapping: the FnSymbol for a required function in the interface def
  //   -> the FnSymbol used to represent calls to that required function
  //      throughout the body of this function
  //
  // a single SymbolMap for all constraints in a CG function is not sufficient
  // when the same interface is implemented by different ConstrainedTypes
  std::vector<SymbolMap> repsForRequiredFns;
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
  BlockStmt*                 lifetimeConstraints;
  BlockStmt*                 retExprType;
  BlockStmt*                 body;
  IntentTag                  thisTag;
  RetTag                     retTag;

  // Support for iterator lowering.
  IteratorInfo*              iteratorInfo;
  // Pointers to other iterator variants - serial, standalone, etc.
  IteratorGroup*             iteratorGroup;
  // Support for genericsCache.
  GenericsCacheInfo*         cacheInfo;
  // Support for interface constraints / constrained generics / CG.
  InterfaceInfo*             interfaceInfo;

  Symbol*                    _this;
  FnSymbol*                  instantiatedFrom;

  SymbolMap                  substitutions;
  SymbolNameVec              substitutionsPostResolve;

  astlocT                    userInstantiationPointLoc;

private:
  BlockStmt*                 _instantiationPoint;
  FnSymbol*                  _backupInstantiationPoint;

public:
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
  void                       codegenFortran(int indent);
  void                       codegenPython(PythonFileType pxd);
  GenRet                     codegenPXDType();
  GenRet                     codegenPYXType();
  std::string                getPythonArrayReturnStmts();

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

  // getReturnSymbol returns the variable marked RVV, but if
  // the return-by-ref transformation has been applied, it returns gVoid.
  Symbol*                    getReturnSymbol();

  // Removes all statements from body and adds all statements from block.
  void                       replaceBodyStmtsWithStmts(BlockStmt* block);
  // Removes all statements from body and adds the passed statement.
  void                       replaceBodyStmtsWithStmt(Expr* stmt);

  // Sets instantiationPoint and backupInstantiationPoint appropriately
  //  expr might be a call or a BlockStmt
  void                       setInstantiationPoint(Expr* expr);
  // returns instantiationPoint or uses the backupInstantiationPoint
  // if necessary
  BlockStmt*                 instantiationPoint()                        const;

  int                        numFormals()                                const;
  ArgSymbol*                 getFormal(int i)                            const;

  void                       collapseBlocks();

  CallExpr*                  singleInvocation()                          const;

  TagGenericResult           tagIfGeneric(SymbolMap* map = NULL, bool abortOK = false);

  bool                       isNormalized()                              const;
  void                       setNormalized(bool value);

  bool                       isResolved()                                const;

  bool                       isMethod()                                  const;
  bool                       isMethodOnClass()                           const;
  bool                       isMethodOnRecord()                          const;
  bool                       isTypeMethod()                              const;

  void                       setMethod(bool value);

  bool                       isPrimaryMethod()                           const;
  bool                       isSecondaryMethod()                         const;
  bool                       isCompilerGenerated()                       const;

  bool                       isInitializer()                             const;
  bool                       isPostInitializer()                         const;
  bool                       isDefaultInit()                             const;
  bool                       isCopyInit()                                const;

  bool                       isGeneric()                                 const;
  bool                       isGenericIsValid()                          const;
  void                       setGeneric(bool generic);
  void                       clearGeneric();
  bool                       isConstrainedGeneric()                      const;
  InterfaceInfo*             ensureInterfaceInfo();
  void                       addConstrainedType(DefExpr* def);
  void                       addInterfaceConstraint(IfcConstraint* icon);

  AggregateType*             getReceiverType()                           const;

  bool                       isIterator()                                const;

  bool                       returnsRefOrConstRef()                      const;

  QualifiedType              getReturnQualType()                         const;

  virtual void               printDocs(std::ostream* file,
                                       unsigned int  tabs);

  void                       throwsErrorInit();
  bool                       throwsError()                               const;

  bool                       retExprDefinesNonVoid()                     const;

  Symbol*                    getSubstitutionWithName(const char* name)   const;

  std::string                nameAndArgsToString(const char* sep,
                                                 bool forError,
                                                 bool& printedUnderline) const;

private:
  virtual std::string        docsDirective();

  bool                       hasGenericFormals(SymbolMap* map)           const;

  bool                       mIsNormalized;
  bool                       _throwsError;
  bool                       mIsGeneric;
  bool                       mIsGenericIsValid;
};

const char*                     toString(FnSymbol* fn);

const char*                     retTagDescrString(RetTag retTag);

extern FnSymbol*                initStringLiterals;

extern FnSymbol*                chpl_gen_main;

extern FnSymbol*                gAddModuleFn;

extern FnSymbol*                gGenericTupleTypeCtor;
extern FnSymbol*                gGenericTupleDestroy;

extern std::map<FnSymbol*, int> ftableMap;
extern std::vector<FnSymbol*>   ftableVec;


#endif
