/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RESOLUTION_SCOPE_TYPES_H
#define CHPL_RESOLUTION_SCOPE_TYPES_H

#include "chpl/types/Type.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/Decl.h"
#include "chpl/util/memory.h"
#include "chpl/util/iteration.h"

#include <unordered_map>
#include <utility>

#include "llvm/ADT/Optional.h"

namespace chpl {
namespace resolution {

class BorrowedIdsWithName;

/** Helper type to store an ID and visibility constraints. */
class IdAndVis {
  friend class OwnedIdsWithName;
  friend class BorrowedIdsWithName;

  ID id_;
  uast::Decl::Visibility vis_ = uast::Decl::PUBLIC;
  bool isMethodOrField_ = false;

 public:
  IdAndVis(ID id, uast::Decl::Visibility vis, bool isMethodOrField)
    : id_(std::move(id)), vis_(vis), isMethodOrField_(isMethodOrField)
  {
  }

  bool operator==(const IdAndVis& other) const {
    return id_ == other.id_ &&
           vis_ == other.vis_ &&
           isMethodOrField_ == other.isMethodOrField_;
  }
  bool operator!=(const IdAndVis& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(id_));
    ret = hash_combine(ret, chpl::hash(vis_));
    ret = hash_combine(ret, chpl::hash(isMethodOrField_));
    return ret;
  }

  void mark(Context* context) const {
    id_.mark(context);
  }

  const ID& id() const { return id_; }
  uast::Decl::Visibility vis() const { return vis_; }
  bool isPrivate() const { return vis_ == uast::Decl::PRIVATE; }
  bool isMethodOrField() const { return isMethodOrField_; }
};

/**
  Collects IDs with a particular name. These can be referred to
  by a BorrowedIdsWithName in a way that avoids copies.
 */
class OwnedIdsWithName {
 friend class BorrowedIdsWithName;

 private:
  // If there is just one ID with this name, it is stored here,
  // and moreIds == nullptr.
  IdAndVis idv_;
  // If there is more than one, all are stored in here,
  // and id redundantly stores the first one.
  // This field is 'owned' in order to allow reuse of pointers to it.
  owned<std::vector<IdAndVis>> moreIdvs_;

 public:
  /** Construct an OwnedIdsWithName containing one ID. */
  OwnedIdsWithName(ID id, uast::Decl::Visibility vis, bool isMethodOrField)
    : idv_(IdAndVis(std::move(id), vis, isMethodOrField)), moreIdvs_(nullptr)
  { }

  /** Append an ID to an OwnedIdsWithName. */
  void appendIdAndVis(ID id, uast::Decl::Visibility vis, bool isMethodOrField) {
    if (moreIdvs_.get() == nullptr) {
      // create the vector and add the single existing id to it
      moreIdvs_ = toOwned(new std::vector<IdAndVis>());
      moreIdvs_->push_back(idv_);
    }
    // add the id passed
    moreIdvs_->push_back(IdAndVis(std::move(id), vis, isMethodOrField));
  }

  bool operator==(const OwnedIdsWithName& other) const {
    if (idv_ != other.idv_)
      return false;

    if ((moreIdvs_.get()==nullptr) != (other.moreIdvs_.get()==nullptr))
      return false;

    if (moreIdvs_.get()==nullptr && other.moreIdvs_.get()==nullptr)
      return true;

    // otherwise, check the vector elements
    return *moreIdvs_.get() == *other.moreIdvs_.get();
  }
  bool operator!=(const OwnedIdsWithName& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    idv_.id_.mark(context);
    if (auto ptr = moreIdvs_.get()) {
      for (auto const& elt : *ptr) {
        context->markOwnedPointer(&elt.id_);
      }
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  llvm::Optional<BorrowedIdsWithName> borrow(bool arePrivateIdsIgnored,
                                             bool onlyMethodsFields) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
  Contains IDs with a particular name. This class is a lightweight
  reference to a collection stored in OwnedIdsWithName.
 */
class BorrowedIdsWithName {
 // To allow us to use the private constructor
 friend class OwnedIdsWithName;

 private:
  /**
    Whether or not this list of IDs should include the private IDs
    from the scope.
   */
  bool arePrivateIdsIgnored_ = false;
  /**
    Whether or not this list of IDs should filter to only include
    methods and fields.
   */
  bool onlyMethodsFields_ = false;
  /** How many IDs are visible in this list. */
  int numVisibleIds_ = 0;
  // TODO: consider storing a variant of ID here
  // with symbolPath, postOrderId, and tag
  IdAndVis idv_;
  const std::vector<IdAndVis>* moreIdvs_ = nullptr;

  static inline bool isIdVisible(const IdAndVis& idv,
                                 bool arePrivateIdsIgnored,
                                 bool onlyMethodsFields) {
    // check privacy
    if (arePrivateIdsIgnored && idv.isPrivate()) {
      return false; // ignore this id
    }

    // check method/field-ness
    if (onlyMethodsFields && !idv.isMethodOrField()) {
      return false; // ignore a non-method non-field
    }

    return true;
  }

  bool isIdVisible(const IdAndVis& idv) const {
    return isIdVisible(idv, arePrivateIdsIgnored_, onlyMethodsFields_);
  }

  /** Returns an iterator referring to the first element stored. */
  const IdAndVis* beginIdAndVis() const {
    if (moreIdvs_ == nullptr) {
      return &idv_;
    }
    return &(*moreIdvs_)[0];
  }
  /** Returns an iterator referring just past the last element stored. */
  const IdAndVis* endIdAndVis() const {
    const IdAndVis* last = nullptr;
    if (moreIdvs_ == nullptr) {
      last = &idv_;
    } else {
      last = &moreIdvs_->back();
    }
    // return the element just past the last element
    return last+1;
  }
 public:
  /**
    Iterator that skips invisible entries from the list of borrowed IDs.
   */
  class BorrowedIdsWithNameIter : public std::iterator<ID, std::forward_iterator_tag> {
    // To allow use of isIdVisible
    friend class BorrowedIdsWithName;
   private:
    /** The borrowed IDs over which we are iterating. */
    const BorrowedIdsWithName* ids;
    /** The ID this iterator is pointing too. */
    const IdAndVis* currentIdv;

    BorrowedIdsWithNameIter(const BorrowedIdsWithName* ids, const IdAndVis* currentIdv)
      : ids(ids), currentIdv(currentIdv) {
      fastForward();
    }

    /** Skip over symbols deemed invisible by the BorrowedIdsWithName. **/
    void fastForward() {
      while (currentIdv != ids->endIdAndVis() &&
             !ids->isIdVisible(*currentIdv)) {
        currentIdv++;
      }
    }

   public:
    bool operator!=(const BorrowedIdsWithNameIter& other) const {
      return ids != other.ids || currentIdv != other.currentIdv;
    }
    BorrowedIdsWithNameIter& operator++() {
      currentIdv++;
      fastForward();
      return *this;
    }
    inline const ID& operator*() const { return currentIdv->id_; }
  };

 private:

  int countVisibleIds();

  /** Construct a BorrowedIdsWithName referring to the same IDs
      as the passed OwnedIdsWithName.
      This BorrowedIdsWithName assumes that the OwnedIdsWithName
      will continue to exist. */
  BorrowedIdsWithName(IdAndVis idv, const std::vector<IdAndVis>* moreIdvs,
                      bool arePrivateIdsIgnored, bool onlyMethodsFields)
    : arePrivateIdsIgnored_(arePrivateIdsIgnored),
      onlyMethodsFields_(onlyMethodsFields),
      idv_(idv),
      moreIdvs_(moreIdvs) {
    numVisibleIds_ = countVisibleIds();
  }

  /** Construct a BorrowedIdsWithName referring to one ID. Requires
      that the ID will not be filtered out according to the passed
      settings arePrivateIdsIgnored and onlyMethodsFields.
    */
  BorrowedIdsWithName(IdAndVis idv,
                      bool arePrivateIdsIgnored,
                      bool onlyMethodsFields)
    : arePrivateIdsIgnored_(arePrivateIdsIgnored),
      onlyMethodsFields_(onlyMethodsFields),
      numVisibleIds_(1), idv_(std::move(idv)) {
    assert(isIdVisible(idv_, arePrivateIdsIgnored, onlyMethodsFields));
  }
 public:

  static llvm::Optional<BorrowedIdsWithName>
  createWithSingleId(ID id, uast::Decl::Visibility vis,
                     bool isMethodOrField,
                     bool arePrivateIdsIgnored,
                     bool onlyMethodsFields) {
    auto idAndVis = IdAndVis(id, vis, isMethodOrField);
    if (isIdVisible(idAndVis, arePrivateIdsIgnored, onlyMethodsFields)) {
      return BorrowedIdsWithName(std::move(idAndVis),
                                 arePrivateIdsIgnored,
                                 onlyMethodsFields);
    }
    return llvm::None;
  }

  static BorrowedIdsWithName
  createWithToplevelModuleId(ID id) {
    auto vis = uast::Decl::Visibility::PUBLIC;
    bool isMethodOrField = false;
    bool arePrivateIdsIgnored = true;
    bool onlyMethodsFields = false;
    auto maybeIds = createWithSingleId(std::move(id),
                                       vis,
                                       isMethodOrField,
                                       arePrivateIdsIgnored,
                                       onlyMethodsFields);
    assert(maybeIds);
    return maybeIds.getValue();
  }

  /** Return the number of IDs stored here */
  int numIds() const {
    return numVisibleIds_;
  }

  /** Returns the first ID in this list. */
  const ID& firstId() const {
    return idv_.id_;
  }

  /** Returns 'true' if the list contains only IDs that represent
      methods or fields. */
  bool containsOnlyMethodsOrFields() const;

  BorrowedIdsWithNameIter begin() const {
    return BorrowedIdsWithNameIter(this, beginIdAndVis());
  }

  BorrowedIdsWithNameIter end() const {
    return BorrowedIdsWithNameIter(this, endIdAndVis());
  }

  bool operator==(const BorrowedIdsWithName& other) const {
    return arePrivateIdsIgnored_ == other.arePrivateIdsIgnored_ &&
           onlyMethodsFields_ == other.onlyMethodsFields_ &&
           numVisibleIds_ == other.numVisibleIds_ &&
           idv_ == other.idv_ &&
           moreIdvs_ == other.moreIdvs_;
  }
  bool operator!=(const BorrowedIdsWithName& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(arePrivateIdsIgnored_));
    ret = hash_combine(ret, chpl::hash(onlyMethodsFields_));
    ret = hash_combine(ret, chpl::hash(numVisibleIds_));
    ret = hash_combine(ret, chpl::hash(moreIdvs_));
    if (moreIdvs_ == nullptr) {
      ret = hash_combine(ret, chpl::hash(idv_));
    } else {
      for (const auto& x : *moreIdvs_) {
        ret = hash_combine(ret, chpl::hash(x));
      }
    }
    return ret;
  }

  void mark(Context* context) const {
    idv_.mark(context);
    for (auto const& elt : *moreIdvs_) {
      context->markPointer(&elt.id_);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
  A DeclMap has key = string name, and value = vector of ID of a NamedDecl
  Using an ID here prevents needing to recompute the Scope
  if (say) something in the body of a Function changed.
 */
using DeclMap = std::unordered_map<UniqueString, OwnedIdsWithName>;

/**
  A scope roughly corresponds to a `{ }` block. Anywhere a new symbol could be
  defined / is defined is a scope.

  The scope contains a mapping from name to ID for symbols defined within.
  For the root scope, it can also contain empty IDs for builtin types and
  symbols.

  While generic instantiations generate something scope-like, the
  point-of-instantiation reasoning will need to be handled with a different
  type.
 */
class Scope {
 private:
  const Scope* parentScope_ = nullptr;
  uast::asttags::AstTag tag_ = uast::asttags::AST_TAG_UNKNOWN;
  // TODO: better pack these to save space
  bool containsUseImport_ = false;
  bool containsFunctionDecls_ = false;
  bool containsExternBlock_ = false;
  bool autoUsesModules_ = false;
  bool methodScope_ = false;
  ID id_;
  UniqueString name_;
  DeclMap declared_;

 public:
  /** Construct an empty scope.
      This scope will not yet store any defined symbols. */
  Scope() { }

  /** Construct a Scope for a particular AST node
      and with a particular parent. */
  Scope(const uast::AstNode* ast, const Scope* parentScope,
        bool autoUsesModules);

  /** Add a builtin type with the provided name. This needs to
      be called to populate the root scope with builtins. */
  void addBuiltin(UniqueString name);

  /** Return the parent scope for this scope. */
  const Scope* parentScope() const { return parentScope_; }

  /** Return the module scope containing this scope,
      or if it is a module scope, this scope. */
  const Scope* moduleScope() const;

  /** Return the parent module of the module containing
      this scope. This is equivalent to:

      'scope->moduleScope()->parentScope()->moduleScope()'
   */
  const Scope* parentModuleScope() const;

  /** Returns the AST tag of the construct that this Scope represents. */
  uast::asttags::AstTag tag() const { return tag_; }

  /** Return the ID of the Block or other AST node construct that this Scope
      represents. An empty ID indicates that this Scope is the root scope. */
  const ID& id() const { return id_; }

  UniqueString name() const { return name_; }

  /** Returns 'true' if this Scope directly contains use or import statements
      including the automatic 'use' for the standard library. */
  bool containsUseImport() const {
    return containsUseImport_ || autoUsesModules_;
  }

  /** Returns 'true' if this Scope directly contains an 'extern' block
      (with C code to supporting interoperability) */
  bool containsExternBlock() const {
    return containsExternBlock_;
  }

  /** Returns 'true' if the Scope includes the automatic 'use' for
      the standard library. */
  bool autoUsesModules() const { return autoUsesModules_; }

  /** Returns 'true' if this Scope represents a method's scope.
      Methods have special scoping behavior to use other fields/methods
      without writing 'this.bla'. */
  bool isMethodScope() const { return methodScope_; }

  /** Returns 'true' if this Scope directly contains any Functions */
  bool containsFunctionDecls() const { return containsFunctionDecls_; }

  int numDeclared() const { return declared_.size(); }

  /** If the scope contains IDs with the provided name,
      append the relevant BorrowedIdsToName the the vector.
      Returns true if something was appended. */
  bool lookupInScope(UniqueString name,
                     std::vector<BorrowedIdsWithName>& result,
                     bool arePrivateIdsIgnored,
                     bool onlyMethodsFields) const;

  /** Check to see if the scope contains IDs with the provided name. */
  bool contains(UniqueString name) const;

  /** Gathers all of the names of symbols declared directly within this scope */
  std::set<UniqueString> gatherNames() const;

  bool operator==(const Scope& other) const {
    return parentScope_ == other.parentScope_ &&
           tag_ == other.tag_ &&
           containsUseImport_ == other.containsUseImport_ &&
           containsFunctionDecls_ == other.containsFunctionDecls_ &&
           containsExternBlock_ == other.containsExternBlock_ &&
           autoUsesModules_ == other.autoUsesModules_ &&
           methodScope_ == other.methodScope_ &&
           id_ == other.id_ &&
           declared_ == other.declared_ &&
           name_ == other.name_;
  }
  bool operator!=(const Scope& other) const {
    return !(*this == other);
  }
  static bool update(owned<Scope>& keep,
                     owned<Scope>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(parentScope_);
    id_.mark(context);
    name_.mark(context);
    for (const auto& pair: declared_) {
      pair.first.mark(context);
      pair.second.mark(context);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
 This class supports both `use` and `import`.
 It stores a normalized form of the symbols made available
 by a use/import clause.
*/
class VisibilitySymbols {
 public:
  /** The kind of import symbol */
  enum Kind {
    /**
      the scope itself only, e.g.:

          import Bar as Baz;

      The names field will contain one pair, denoting the renaming
      of the module itself.

     */
    SYMBOL_ONLY,
    /**
      all the contents of the scope, e.g.:

          import Foo;

      The names field will be empty.

     */
    ALL_CONTENTS,
    /**
      only the contents of the scope named in names, e.g.:

          import Foo.{a as x,b};

      The names field will contain the imported names.
     */
    ONLY_CONTENTS,
    /**
      contents of the scope except listed names, e.g.:

          use Foo except {a, b};

      The names field will contain the excluded names. There is no renaming
      in this case.
     */
    CONTENTS_EXCEPT,
  };

  /** Indicating which shadow scope level to use, if any */
  enum ShadowScope {
    /**
      `REGULAR_SCOPE` indicates that no shadow scope is used for the symbols
      brought in by this VisibilitySymbols. In other words, the symbols
      brought in are at the same scope level as a variable declared next to
      the `use` / `import`. `REGULAR_SCOPE` is the shadow scope level
      used for:

        * `public use`
        * `import` aka `private import`
        * `public import`
     */
    REGULAR_SCOPE = 0,

    // 'private use' aka just 'use' introduces 2 shadow scopes
    // that appear to be between the module and any parent symbols

    /**
      `SHADOW_SCOPE_ONE` indicates a shadow scope just outside of
      `REGULAR_SCOPE` but before `SHADOW_SCOPE_TWO`. This level is
      used for the module contents brought in by a non-public `use`.
     */
    SHADOW_SCOPE_ONE = 1,

    /**
      `SHADOW_SCOPE_TWO` indicates a shadow scope just outside of
      `SHADOW_SCOPE_ONE` but before any parent scopes. This level
      is used for the module name brought in by a non-public `use`.
     */
    SHADOW_SCOPE_TWO = 2,
  };

 private:
  const Scope* scope_; // Scope of the Module used/imported
                       // This could technically be an ID but basically
                       // anything we do with it needs a Scope* anyway.
  Kind kind_ = SYMBOL_ONLY;
  bool isPrivate_ = true;
  int8_t shadowScopeLevel_ = REGULAR_SCOPE;

  ID visibilityClauseId_; // ID of the uAST that generated this
                          // (this is only needed to support error messages)

  // the names/renames:
  //  pair.first is the name as declared
  //  pair.second is the name here
  std::vector<std::pair<UniqueString,UniqueString>> names_;

 public:
  VisibilitySymbols() { }
  VisibilitySymbols(const Scope* scope, Kind kind,
                    bool isPrivate, ShadowScope shadowScopeLevel,
                    ID visibilityClauseId,
                    std::vector<std::pair<UniqueString,UniqueString>> names)
    : scope_(scope), kind_(kind),
      isPrivate_(isPrivate), shadowScopeLevel_(shadowScopeLevel),
      visibilityClauseId_(visibilityClauseId),
      names_(std::move(names))
  {
    CHPL_ASSERT(shadowScopeLevel == REGULAR_SCOPE ||
                shadowScopeLevel == SHADOW_SCOPE_ONE ||
                shadowScopeLevel == SHADOW_SCOPE_TWO);
  }

  /** Return the imported scope */
  const Scope* scope() const { return scope_; }

  /** Return the kind of the imported symbol */
  Kind kind() const { return kind_; }

  /** Return whether or not the imported symbol is private */
  bool isPrivate() const { return isPrivate_; }

  /** Returns the shadow scope level of the symbols here */
  ShadowScope shadowScopeLevel() const {
    return (ShadowScope) shadowScopeLevel_;
  }

  /**
    Returns the ID of the use/import clause that this VisibilitySymbols was
    created to represent. */
  const ID& visibilityClauseId() const {
    return visibilityClauseId_;
  }

  /** Lookup the declared name for a given name
      Returns true if `name` is found in the list of renamed names and
      stores the declared name in `declared`
      Returns false if `name` is not found
  */
  bool lookupName(const UniqueString &name, UniqueString &declared) const {
    for (const auto &p : names_) {
      if (p.second == name) {
        declared = p.first;
        return true;
      }
    }
    return false;
  }

  bool operator==(const VisibilitySymbols &other) const {
    return scope_ == other.scope_ &&
           kind_ == other.kind_ &&
           isPrivate_ == other.isPrivate_ &&
           shadowScopeLevel_ == other.shadowScopeLevel_ &&
           visibilityClauseId_ == other.visibilityClauseId_ &&
           names_ == other.names_;
  }
  bool operator!=(const VisibilitySymbols& other) const {
    return !(*this == other);
  }

  void swap(VisibilitySymbols& other) {
    std::swap(scope_, other.scope_);
    std::swap(kind_, other.kind_);
    std::swap(isPrivate_, other.isPrivate_);
    std::swap(shadowScopeLevel_, other.shadowScopeLevel_);
    names_.swap(other.names_);
    visibilityClauseId_.swap(other.visibilityClauseId_);
  }

  static bool update(VisibilitySymbols& keep,
                     VisibilitySymbols& addin) {
    return defaultUpdate(keep, addin);
  }

  void mark(Context* context) const {
    context->markPointer(scope_);
    visibilityClauseId_.mark(context);
    for (auto p : names_) {
      p.first.mark(context);
      p.second.mark(context);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
 Stores the result of in-order resolution of use/import statements.
*/
class ResolvedVisibilityScope {
 private:
  const Scope* scope_;
  std::vector<VisibilitySymbols> visibilityClauses_;

 public:
  using VisibilitySymbolsIterable = Iterable<std::vector<VisibilitySymbols>>;

  ResolvedVisibilityScope(const Scope* scope)
    : scope_(scope)
  { }

  /** Return the scope */
  const Scope *scope() const { return scope_; }

  /** Return an iterator over the visibility clauses */
  VisibilitySymbolsIterable visibilityClauses() const {
    return VisibilitySymbolsIterable(visibilityClauses_);
  }

  /** Add a visibility clause */
  void addVisibilityClause(const Scope* scope, VisibilitySymbols::Kind kind,
                           bool isPrivate,
                           VisibilitySymbols::ShadowScope shadowScopeLevel,
                           ID visibilityClauseId,
                           std::vector<std::pair<UniqueString,UniqueString>> n)
  {
    auto elt = VisibilitySymbols(scope, kind,
                                 isPrivate, shadowScopeLevel,
                                 std::move(visibilityClauseId),
                                 std::move(n));
    visibilityClauses_.push_back(std::move(elt));
  }

  bool operator==(const ResolvedVisibilityScope& other) const {
    return scope_ == other.scope_ &&
           visibilityClauses_ == other.visibilityClauses_;
  }
  bool operator!=(const ResolvedVisibilityScope& other) const {
    return !(*this == other);
  }
  static bool update(owned<ResolvedVisibilityScope>& keep,
                     owned<ResolvedVisibilityScope>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(scope_);
    for (auto sym : visibilityClauses_) {
      sym.mark(context);
    }
  }
  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;
  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/*
 * Type of visibility statement (use or import)
 */
enum VisibilityStmtKind {
  VIS_USE,
  VIS_IMPORT,
};

enum {
  /**
    When looking at a scope, find symbols declared in that scope.
   */
  LOOKUP_DECLS = 1,

  /**
    When looking at a scope, consider symbols brought in to that scope by
    'import' and 'use' statements.
   */
  LOOKUP_IMPORT_AND_USE = 2,

  /**
    When looking at a scope, also search parent scopes (those that lexically
    contain this scope). Stops when a module scope is found unless
    LOOKUP_GO_PAST_MODULES is included. Additionally, looks for symbols in the
    root module.
   */
  LOOKUP_PARENTS = 4,

  /**
    Find toplevel modules with the matching name.
   */
  LOOKUP_TOPLEVEL = 8,

  /**
    Find only the innermost match or matches.
   */
  LOOKUP_INNERMOST = 16,

  /**
    Find only public symbols.
   */
  LOOKUP_SKIP_PRIVATE_VIS = 32,

  /**
    Continue the scope search to parent modules instead of stopping
    at the module scope. This setting supports error messages.
   */
  LOOKUP_GO_PAST_MODULES = 64,

  /**
    Lookup only methods, fields, and class/record/union declarations
    directly nested within a class/record/union
   */
  LOOKUP_ONLY_METHODS_FIELDS = 128,

  /**
    Lookup in extern blocks
   */
  LOOKUP_EXTERN_BLOCKS = 256,
};

/** LookupConfig is a bit-set of the LOOKUP_ flags defined above */
using LookupConfig = unsigned int;

/**
 PoiScope is a point-of-instantiation scope
 */
// When resolving a traditional generic, we also need to consider
// the point-of-instantiation scope as a place to find visible functions.
// This type tracks such a scope.
//
// PoiScopes do not need to consider scopes that are visible from
// the function declaration. These can be collapsed away.
//
// Performance: could have better reuse of PoiScope if it used the Scope ID
// rather than changing if the Scope contents do. But, the downside is that
// further queries would be required to compute which functions are
// visible. Which is better?
// If we want to make PoiScope not depend on the contents it might be nice
// to make Scope itself not depend on the contents, too.
class PoiScope {
 private:
  const Scope* inScope_ = nullptr;         // parent Scope for the Call
  const PoiScope* inFnPoi_ = nullptr;      // what is the POI of this POI?

 public:
  PoiScope(const Scope *scope, const PoiScope *poiScope)
      : inScope_(scope), inFnPoi_(poiScope) {}

  /** return the parent scope for the call */
  const Scope *inScope() const { return inScope_; }

  /** return the POI of this POI */
  const PoiScope *inFnPoi() const { return inFnPoi_; }

  bool operator==(const PoiScope& other) const {
    return inScope_ == other.inScope_ &&
           inFnPoi_ == other.inFnPoi_;
  }
  bool operator!=(const PoiScope& other) const {
    return !(*this == other);
  }
  static bool update(owned<PoiScope>& keep,
                     owned<PoiScope>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    context->markPointer(inScope_);
    context->markPointer(inFnPoi_);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    ss << "PoiScope ";
    if (inScope() != nullptr) {
      inScope()->stringify(ss,stringKind);
    } else {
      ss << "null";
    }
    ss << " ";
    if (inFnPoi() != this && inFnPoi() != nullptr) {
      inFnPoi()->stringify(ss, stringKind);
    }
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
 InnermostMatch
 */
class InnermostMatch {
 public:
  typedef enum {
    ZERO = 0,
    ONE = 1,
    MANY = 2,
  } MatchesFound;

 private:
  ID id_;
  MatchesFound found_ = ZERO;

 public:
  InnermostMatch() { }
  InnermostMatch(ID id, MatchesFound found)
    : id_(id), found_(found)
  { }

  /** Return the id */
  ID id() const { return id_;}

  /** Return the matches found */
  MatchesFound found() const { return found_;}

  bool operator==(const InnermostMatch& other) const {
    return id_ == other.id_ &&
           found_ == other.found_;
  }
  bool operator!=(const InnermostMatch& other) const {
    return !(*this == other);
  }
  void swap(InnermostMatch& other) {
    id_.swap(other.id_);
    std::swap(found_, other.found_);
  }
  static bool update(InnermostMatch& keep, InnermostMatch& addin) {
    return defaultUpdate(keep, addin);
  }
  void mark(Context* context) const {
    id_.mark(context);
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    ss << "InnermostMatch not yet stringified";
  }

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};


/** ResultVisibilityTrace stores a tracing of the name lookup process
    which can be useful for error messages. */
struct ResultVisibilityTrace {
  struct VisibilityTraceElt {
    // these contain details for a use/import
    VisibilitySymbols::ShadowScope shadowScope =
       VisibilitySymbols::REGULAR_SCOPE;
    const ResolvedVisibilityScope* resolvedVisibilityScope = nullptr;
    ID visibilityClauseId;
    VisibilityStmtKind visibilityStmtKind = VIS_USE;
    UniqueString renameFrom;
    bool fromUseImport = false;

    // this indicates a method receiver scope
    const Scope* methodReceiverScope = nullptr;

    // this indicates a parent scope
    const Scope* parentScope = nullptr;

    // these cover other cases
    bool automaticModule = false;
    bool toplevelModule = false;
    bool externBlock = false;
    bool rootScope = false;

    bool operator==(const VisibilityTraceElt& other) const {
      return shadowScope == other.shadowScope &&
             resolvedVisibilityScope == other.resolvedVisibilityScope &&
             visibilityClauseId == other.visibilityClauseId &&
             visibilityStmtKind == other.visibilityStmtKind &&
             renameFrom == other.renameFrom &&
             fromUseImport == other.fromUseImport &&
             methodReceiverScope == other.methodReceiverScope &&
             parentScope == other.parentScope &&
             automaticModule == other.automaticModule &&
             toplevelModule == other.toplevelModule &&
             externBlock == other.externBlock &&
             rootScope == other.rootScope;
    }
    bool operator!=(const VisibilityTraceElt& other) const {
      return !(*this == other);
    }
    void mark(Context* context) const {
      context->markPointer(resolvedVisibilityScope);
      renameFrom.mark(context);
      visibilityClauseId.mark(context);
      context->markPointer(methodReceiverScope);
      context->markPointer(parentScope);
    }
  };

  // the scope where it is eventually found
  const Scope* scope = nullptr;

  // how did we get to 'scope' ? this is a vector because there might
  // have been multiple public use / public imports traversed.
  std::vector<VisibilityTraceElt> visibleThrough;

  bool operator==(const ResultVisibilityTrace& other) const {
    return scope == other.scope &&
           visibleThrough == other.visibleThrough;
  }
  bool operator!=(const ResultVisibilityTrace& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    for (const auto& elt : visibleThrough) {
      elt.mark(context);
    }
  }
};


} // end namespace resolution

/// \cond DO_NOT_DOCUMENT

template <>
struct stringify<resolution::VisibilityStmtKind> {
  void operator()(std::ostream& streamOut, StringifyKind stringKind,
                  resolution::VisibilityStmtKind kind) const {
    switch (kind) {
      case resolution::VisibilityStmtKind::VIS_USE:
        streamOut << "use";
        break;
      case resolution::VisibilityStmtKind::VIS_IMPORT:
        streamOut << "import";
        break;
      default:
        CHPL_ASSERT(false && "should not reach this point");
    }
  }
};

template <>
struct mark<resolution::VisibilityStmtKind> {
  void operator()(Context* context, resolution::VisibilityStmtKind t) {
    // No need to mark enums
  }
};

/// \endcond


} // end namespace chpl


namespace std {

/// \cond DO_NOT_DOCUMENT
template<> struct hash<chpl::resolution::IdAndVis>
{
  size_t operator()(const chpl::resolution::IdAndVis& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::BorrowedIdsWithName>
{
  size_t operator()(const chpl::resolution::BorrowedIdsWithName& key) const {
    return key.hash();
  }
};

template <>
struct hash<chpl::resolution::VisibilityStmtKind> {
  size_t operator()(const chpl::resolution::VisibilityStmtKind& key) const {
    return (size_t)key;
  }
};

/// \endcond

} // end namespace std

#endif
