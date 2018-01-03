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

#ifndef _AGGREGATE_TYPE_H_
#define _AGGREGATE_TYPE_H_

#include "type.h"

/************************************* | **************************************
*                                                                             *
* A base type for union, class, and record.                                   *
*                                                                             *
************************************** | *************************************/

enum AggregateTag {
  AGGREGATE_CLASS,
  AGGREGATE_RECORD,
  AGGREGATE_UNION
};


enum InitializerStyle {
  DEFINES_CONSTRUCTOR,
  DEFINES_INITIALIZER,
  DEFINES_NONE_USE_DEFAULT
};


class AggregateType : public Type {
public:
  static void                 setCreationStyle(TypeSymbol* t, FnSymbol* fn);

public:
                              AggregateType(AggregateTag initTag);
                             ~AggregateType();

  DECLARE_COPY(AggregateType);

  virtual void                replaceChild(BaseAST* oldAst, BaseAST* newAst);

  virtual void                verify();

  virtual void                accept(AstVisitor* visitor);

  virtual void                printDocs(std::ostream* file, unsigned int tabs);

  bool                        isClass()                                  const;
  bool                        isRecord()                                 const;
  bool                        isUnion()                                  const;

  bool                        isGeneric()                                const;
  void                        markAsGeneric();

  const char*                 classStructName(bool standalone);

  int                         numFields()                                const;

  Symbol*                     getField(int i)                            const;

  Symbol*                     getField(const char* name,
                                       bool        fatal = true)         const;

  int                         getFieldPosition(const char* name,
                                               bool        fatal = true);

  // e is as used in PRIM_GET_MEMBER/PRIM_GET_SVEC_MEMBER
  QualifiedType               getFieldType(Expr* e);

  void                        addDeclarations(Expr* expr);

  bool                        hasInitializers()                          const;

  bool                        mayHaveInstances()                         const;

  void                        codegenDef();

  void                        codegenPrototype();

  GenRet                      codegenClassStructType();

  int                         codegenStructure(FILE*       outfile,
                                               const char* baseoffset);

  int                         codegenFieldStructure(FILE*       outfile,
                                                    bool        nested,
                                                    const char* baseOffset);

  bool                        setFirstGenericField();

  AggregateType*              getInstantiation(Symbol* sym, int index);
  AggregateType*              getInstantiationParent(AggregateType* pt);

  AggregateType*              getInstantiationMulti(SymbolMap& subs,
                                                    FnSymbol*  fn);

  bool                        isInstantiatedFrom(const AggregateType* base)
                                                                         const;

  DefExpr*                    toLocalField(const char* name)             const;
  DefExpr*                    toLocalField(SymExpr*    expr)             const;
  DefExpr*                    toLocalField(CallExpr*   expr)             const;

  DefExpr*                    toSuperField(SymExpr*  expr);
  DefExpr*                    toSuperField(CallExpr* expr);

  int                         getMemberGEP(const char* name);

  void                        createOuterWhenRelevant();

  void                        buildConstructors();

  void                        addRootType();

  void                        addClassToHierarchy();

  bool                        parentDefinesInitializer()                 const;

  bool                        wantsDefaultInitializer()                  const;

  void                        buildDefaultInitializer();


  //
  // Public fields
  //

  AggregateTag                aggregateTag;

  FnSymbol*                   defaultTypeConstructor;

  FnSymbol*                   defaultInitializer;

  void                        buildCopyInitializer();

  AggregateType*              instantiatedFrom;

  InitializerStyle            initializerStyle;

  bool                        initializerResolved;

  AList                       fields;

  // used from parsing, sets dispatchParents
  AList                       inherits;

  // pointer to an outer class if this is an inner class
  Symbol*                     outer;

  // Attached only to iterator class/records
  IteratorInfo*               iteratorInfo;

  // What to delegate to with 'forwarding'
  AList                       forwardingTo;

  const char*                 doc;

  // Used during code generation for subclass checking,
  // isa checking. This is the value we store in chpl__cid_XYZ.
  int                         classId;

  Vec<AggregateType*>         dispatchParents;    // dispatch hierarchy
  Vec<AggregateType*>         dispatchChildren;   // dispatch hierarchy

private:
  static ArgSymbol*           createGenericArg(VarSymbol* field);
  static void                 insertImplicitThis(
                                            FnSymbol*         fn,
                                            Vec<const char*>& fieldNamesSet);

private:
  virtual std::string         docsDirective();

  std::string                 docsSuperClass();

  void                        addDeclaration(DefExpr* defExpr);

  void                        addClassToHierarchy(
                                          std::set<AggregateType*>& seen);

  AggregateType*              discoverParentAndCheck(Expr* storesName);

  FnSymbol*                   buildTypeConstructor();

  CallExpr*                   typeConstrSuperCall(FnSymbol* fn)          const;

  bool                        isFieldInThisClass(const char* name)       const;

  void                        typeConstrSetFields(FnSymbol* fn,
                                                  CallExpr* superCall)   const;

  void                        typeConstrSetField(FnSymbol*  fn,
                                                 VarSymbol* field,
                                                 Expr*      expr)        const;

  ArgSymbol*                  insertGenericArg(FnSymbol*  fn,
                                               VarSymbol* field)         const;

  void                        buildConstructor();

  bool                        needsConstructor();

  ArgSymbol*                  moveConstructorToOuter(FnSymbol* fn);

  void                        fieldToArg(FnSymbol*              fn,
                                         std::set<const char*>& names,
                                         SymbolMap&             fieldArgMap);

  void                        fieldToArgType(DefExpr*   fieldDef,
                                             ArgSymbol* arg);

  bool                        addSuperArgs(FnSymbol*                    fn,
                                           const std::set<const char*>& names);

  std::vector<AggregateType*> instantiations;

  // genericField stores the index of the first generic field in the
  // AggregateType which does not have a substitution,
  // but only if the AggregateType defines an initializer.
  //
  // If the type has no generic fields without substitutions,
  // or if setNextGenericField has not been called on the base AggregateType,
  // this will be set to 0.
  int                         genericField;

  bool                        mIsGeneric;
};

extern AggregateType* dtObject;

extern AggregateType* dtString;

DefExpr* defineObjectClass();

#endif
