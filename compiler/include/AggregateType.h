/*
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

  // is it a generic type (e.g. contains a type field with or without default)
  // e.g. this would return true for
  //    class C { type t = int; }
  //    class C { type t; }
  //    class C { param p; }
  //    class C { var x; }
  bool                        isGeneric()                                const;
  void                        markAsGeneric();

  // is it a generic type with defaults for all type/param fields?
  // these don't behave the same as fully generic types.
  // For example, returns true for
  //    class C { type t = int; }
  // and false for
  //    class C { type t; }
  bool                        isGenericWithDefaults()                    const;
  void                        markAsGenericWithDefaults();

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
  bool                        hasPostInitializer()                       const;
  bool                        hasUserDefinedInitEquals()                 const;

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

  AggregateType*              generateType(SymbolMap& subs);

  bool                        isInstantiatedFrom(const AggregateType* base)
                                                                         const;

  AggregateType*              getRootInstantiation();

  DefExpr*                    toLocalField(const char* name)             const;
  DefExpr*                    toLocalField(SymExpr*    expr)             const;
  DefExpr*                    toLocalField(CallExpr*   expr)             const;

  DefExpr*                    toSuperField(const char* name)             const;
  DefExpr*                    toSuperField(SymExpr*  expr)               const;
  DefExpr*                    toSuperField(CallExpr* expr)               const;

  int                         getMemberGEP(const char* name,
                                           bool& isCArrayField);

  FnSymbol*                   buildTypeConstructor();

  void                        addRootType();

  void                        addClassToHierarchy();

  bool                        wantsDefaultInitializer()                  const;

  void                        buildDefaultInitializer();

  void                        buildCopyInitializer();

  Symbol*                     getSubstitution(const char* name);

  UnmanagedClassType*         getUnmanagedClass();

  void                        generateUnmanagedClassTypes();

  // Returns true if a field is considered generic
  // (i.e. it needs a type constructor argument)
  bool                        fieldIsGeneric(Symbol* field,
                                             bool &hasDefault)           const;


  Type*                       cArrayElementType()                        const;
  int64_t                     cArrayLength()                             const;

  //
  // Public fields
  //

  AggregateTag                aggregateTag;

  // These fields support differentiating between unmanaged class
  // pointers and borrows. At the present time, borrows are represented
  // by plain AggregateType and unmanaged class pointers use this special type.
  UnmanagedClassType*         unmanagedClass;

  FnSymbol*                   typeConstructor;

  bool                        builtDefaultInit;

  AggregateType*              instantiatedFrom;

  bool                        hasUserDefinedInit;

  bool                        initializerResolved;

  AList                       fields;

  // used from parsing, sets dispatchParents
  AList                       inherits;

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

  // Only used for LLVM.
  std::map<std::string, bool> isCArrayFieldMap;

  static ArgSymbol*           createGenericArg(VarSymbol* field);

  void                        insertImplicitThis(FnSymbol*         fn,
                                                 Vec<const char*>& names) const;

private:
  virtual std::string         docsDirective();

  std::string                 docsSuperClass();

  void                        addDeclaration(DefExpr* defExpr);

  void                        addClassToHierarchy(
                                          std::set<AggregateType*>& seen);

  AggregateType*              instantiationWithParent(AggregateType* parent);

  Symbol*                     substitutionForField(Symbol*    field,
                                                   SymbolMap& subs)      const;

  AggregateType*              getCurInstantiation(Symbol* sym);

  AggregateType*              getNewInstantiation(Symbol* sym);

  AggregateType*              discoverParentAndCheck(Expr* storesName);

  CallExpr*                   typeConstrSuperCall(FnSymbol* fn)  const;

  bool                        isFieldInThisClass(const char* name)       const;

  void                        typeConstrSetFields(FnSymbol* fn,
                                                  CallExpr* superCall)   const;

  bool                        setNextGenericField();

  void                        typeConstrSetField(FnSymbol*  fn,
                                                 VarSymbol* field,
                                                 Expr*      expr)        const;

  ArgSymbol*                  insertGenericArg(FnSymbol*  fn,
                                               VarSymbol* field)  const;

private:

  void                        moveTypeConstructorToOuter(FnSymbol* fn);

  void                        fieldToArg(FnSymbol*              fn,
                                         std::set<const char*>& names,
                                         SymbolMap&             fieldArgMap);

  void                        fieldToArgType(DefExpr*   fieldDef,
                                             ArgSymbol* arg);

  bool                        addSuperArgs(FnSymbol*                    fn,
                                           const std::set<const char*>& names,
                                           SymbolMap&                   fieldArgMap);

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
  bool                        mIsGenericWithDefaults;
};

extern AggregateType* dtObject;

extern AggregateType* dtString;
extern AggregateType* dtLocale;

DefExpr* defineObjectClass();

#endif
