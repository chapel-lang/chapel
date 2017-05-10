/*
 * Copyright 2004-2017 Cray Inc.
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

  void                        addDeclarations(Expr* expr);

  void                        codegenDef();

  void                        codegenPrototype();

  GenRet                      codegenClassStructType();

  int                         codegenStructure(FILE*       outfile,
                                               const char* baseoffset);

  int                         codegenFieldStructure(FILE*       outfile,
                                                    bool        nested,
                                                    const char* baseOffset);

  // The following two methods are used for types which define initializers
  bool                        setNextGenericField();

  AggregateType*              getInstantiation(SymExpr* t, int index);

  const char*                 classStructName(bool standalone);

  int                         getMemberGEP(const char* name);

  int                         getFieldPosition(const char* name,
                                               bool        fatal = true);

  Symbol*                     getField(const char* name, bool fatal = true);
  Symbol*                     getField(int i);

  // e is as used in PRIM_GET_MEMBER/PRIM_GET_SVEC_MEMBER
  QualifiedType               getFieldType(Expr* e);

  int                         numFields()                                const;

  bool                        isClass()                                  const;
  bool                        isRecord()                                 const;
  bool                        isUnion()                                  const;

  bool                        isGeneric()                                const;
  void                        markAsGeneric();

  void                        buildConstructors();

  void                        addRootType();

  void                        addClassToHierarchy();

  AggregateTag                aggregateTag;

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
  void                        buildTypeConstructor();
  void                        buildConstructor();
  void                        moveConstructorToOuter(FnSymbol* fn);

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

extern AggregateType* dtString;
extern AggregateType* dtArray;
extern AggregateType* dtBaseArr;
extern AggregateType* dtBaseDom;
extern AggregateType* dtDist;
extern AggregateType* dtTuple;
extern AggregateType* dtLocale;
extern AggregateType* dtLocaleID;
extern AggregateType* dtMainArgument;
extern AggregateType* dtOnBundleRecord;
extern AggregateType* dtTaskBundleRecord;
extern AggregateType* dtError;

#endif
