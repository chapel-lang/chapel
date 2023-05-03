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
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallSet.h"

namespace chpl {
namespace resolution {

class BorrowedIdsWithName;

/** Helper type to store an ID and visibility constraints. */
class IdAndFlags {
 public:
  // helper types
  enum {
    /** Public */
    PUBLIC = 1,
    /** Not public (aka private) */
    NOT_PUBLIC = 2,
    /** A method or field declaration */
    METHOD_FIELD = 4,
    /** Something other than (a method or field declaration) */
    NOT_METHOD_FIELD = 8,
    // note: if adding something here, also update flagsToString
    /** A function using parens (including iterators and methods) */
    PARENFUL_FUNCTION = 16,
    /** A non-function or a function without parentheses */
    NOT_PARENFUL_FUNCTION = 32,
    // note: if adding something here, also update flagsToString
    /** A method declaration */
    METHOD = 64,
    /** Something other than a method declaration */
    NOT_METHOD = 128,
    // note: if adding something here, also update flagsToString
  };
  /**
    A bit-set of the flags defined in the above enum.
    Represents a conjunction / AND of all the set bit.
    E.g.:
      * just IdAndFlags::PUBLIC -- only public symbols
      * just IdAndFlags::METHOD_FIELD -- only methods/fields
      * IdAndFlags::PUBLIC | IdAndFlags::METHOD_FIELD --
        only public symbols that are methods/fields
      * Empty Flags -- match everything
   */
  using Flags = uint16_t;

  /** A set of bit-sets / flag combinations. Logically, while Flags
      represents a conjunction of all of its bitfields (see documentation
      on Flags), this FlagSet represents a disjunction of each flag
      combination. That is to say, if FlagSet contains:

          IdAndFlags::PUBLIC, IdAndFlags::NOT_PUBLIC | IdAndFlags::METHOD_FIELD

      This represents the following condition on variables:

          IdAndFlags::PUBLIC ∨ (IdAndFlags::NOT_PUBLIC ∧ IdAndFlags::METHOD_FIELD)

      That is, either any public symbol, or a private method or field.

      Other examples:
        * [] (empty FlagSet) -- matches nothing.
        * [IdAndFlags::PUBLIC | IdAndFlags::METHOD_FIELD] --
          only public symbols that are methods / fields.
        * More generally, [f] (singleton set) --
          equivalent to the single contained Flags value f.

      Inserting into the FlagSet automatically tries to perform basic
      simplificaton to avoid growing the size. */
  class FlagSet {
   private:
     llvm::SmallVector<Flags, 4> flagVec;

   public:
    FlagSet() = default;
    FlagSet(const FlagSet& other) = default;
    FlagSet(FlagSet&& other) = default;
    FlagSet& operator=(const FlagSet& other) = default;
    FlagSet& operator=(FlagSet&& other) = default;

    /** Create a FlagSet consisting of only one combination of Flags.
        Flags represents a conjunction (AND) of properties; see the comment
        on Flags for more info. */
    static FlagSet singleton(Flags flags);

    /** Create a FlagSet consisting of no flag combinations; such a set
        matches nothing (the base case of OR is false). */
    static FlagSet empty();

    /** Add a new disjunct to the set of flag combinations. Automatically
        performs some deduplication and packing to avoid growing the set
        if possible. */
    void addDisjunction(Flags excludeFlags);

    /* Checks if any flag combinations in the set already subsume
       the given flag combination. */
    bool subsumes(Flags mightBeSubsumed) const;

    /** Checks that none of the or'ed flag combinations match the given flags. */
    bool noneMatch(Flags match) const;

    bool operator==(const FlagSet& other) const;
    bool operator!=(const FlagSet& other) const;
    size_t hash() const;
    void mark(Context* context) const;
  };

 private:
  // friends
  friend class OwnedIdsWithName;
  friend class BorrowedIdsWithName;

  // all fields
  ID id_;
  Flags flags_ = 0;

 public:
  IdAndFlags(ID id, uast::Decl::Visibility vis,
             bool isField, bool isMethod, bool isParenfulFunction)
    : id_(std::move(id)) {
    // setup the flags
    Flags flags = 0;
    switch (vis) {
      case uast::Decl::DEFAULT_VISIBILITY:
      case uast::Decl::PUBLIC:
        flags |= PUBLIC;
        break;
      case uast::Decl::PRIVATE:
        flags |= NOT_PUBLIC;
        break;
      // no default for compilation error if more are added
    }
    if (isMethod || isField) {
      flags |= METHOD_FIELD;
    } else {
      flags |= NOT_METHOD_FIELD;
    }
    if (isMethod) {
      flags |= METHOD;
    } else {
      flags |= NOT_METHOD;
    }
    if (isParenfulFunction) {
      flags |= PARENFUL_FUNCTION;
    } else {
      flags |= NOT_PARENFUL_FUNCTION;
    }
    flags_ = flags;
  }

  bool operator==(const IdAndFlags& other) const {
    return id_ == other.id_ &&
           flags_ == other.flags_;
  }
  bool operator!=(const IdAndFlags& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(id_));
    ret = hash_combine(ret, chpl::hash(flags_));
    return ret;
  }

  void mark(Context* context) const {
    id_.mark(context);
  }

  const ID& id() const { return id_; }
  bool isPublic() const {
    return (flags_ & PUBLIC) != 0;
  }
  bool isMethodOrField() const {
    return (flags_ & METHOD_FIELD) != 0;
  }
  bool isMethod() const {
    return (flags_ & METHOD) != 0;
  }
  bool isParenfulFunction() const {
    return (flags_ & PARENFUL_FUNCTION) != 0;
  }

  // return true if haveFlags matches filterFlags, and does not match
  // the exclude flag set. See the comments on Flags adn FlagSet for
  // how the matching works.
  static bool matchFilter(Flags haveFlags,
                          Flags filterFlags,
                          const FlagSet& excludeFlagSet) {
    return (haveFlags & filterFlags) == filterFlags &&
           excludeFlagSet.noneMatch(haveFlags);
  }

  static std::string flagsToString(Flags flags);
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
  IdAndFlags idv_;
  // the result of bitwise and (&) of flags of all of the symbols
  IdAndFlags::Flags flagsAnd_;
  // the result of bitwise or (|) of flags of all of the symbols
  IdAndFlags::Flags flagsOr_;
  // If there is more than one, all are stored in here,
  // and id redundantly stores the first one.
  // This field is 'owned' in order to allow reuse of pointers to it.
  owned<std::vector<IdAndFlags>> moreIdvs_;

 public:
  /** Construct an OwnedIdsWithName containing one ID. */
  OwnedIdsWithName(ID id, uast::Decl::Visibility vis,
                   bool isField, bool isMethod, bool isParenfulFunction)
    : idv_(IdAndFlags(std::move(id), vis, isField, isMethod, isParenfulFunction)),
      flagsAnd_(idv_.flags_), flagsOr_(idv_.flags_),
      moreIdvs_(nullptr)
  { }

  /** Append an ID to an OwnedIdsWithName. */
  void appendIdAndFlags(ID id, uast::Decl::Visibility vis,
                        bool isField, bool isMethod, bool isParenfulFunction) {
    if (moreIdvs_.get() == nullptr) {
      // create the vector and add the single existing id to it
      moreIdvs_ = toOwned(new std::vector<IdAndFlags>());
      moreIdvs_->push_back(idv_);
      // flagsAnd_ and flagsOr_ will have already been set in constructor
      // from idv_.
    }
    auto idv = IdAndFlags(std::move(id), vis,
                          isField, isMethod, isParenfulFunction);
    // add the id passed
    moreIdvs_->push_back(std::move(idv));
    // update the flags
    flagsAnd_ &= idv.flags_;
    flagsOr_ |= idv.flags_;
  }

  int numIds() const {
    if (moreIdvs_.get() == nullptr) {
      return 1;
    }

    return moreIdvs_->size();
  }

  bool operator==(const OwnedIdsWithName& other) const {
    // check the initial fields
    if (idv_ != other.idv_ ||
        flagsAnd_ != other.flagsAnd_ ||
        flagsOr_ != other.flagsOr_)
      return false;

    // check moreIdvs for null ptr vs not null ptr
    if ((moreIdvs_.get()==nullptr) != (other.moreIdvs_.get()==nullptr))
      return false;
    if (moreIdvs_.get()==nullptr && other.moreIdvs_.get()==nullptr)
      return true;

    // otherwise, check the vector elements, which cannot be nullptr here
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

  llvm::Optional<BorrowedIdsWithName>
  borrow(IdAndFlags::Flags filterFlags, const IdAndFlags::FlagSet& excludeFlagSet) const;

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
    Filter to symbols that match these flags. See the comment on Flags
    for how matching is performed.
   */
  IdAndFlags::Flags filterFlags_ = 0;
  /**
    Exclude symbols whose flags are matched by this FlagSet; see
    the comment on FlagSet for how the matching is performed.
   */
  IdAndFlags::FlagSet excludeFlagSet_;

  /** How many IDs are visible in this list. */
  int numVisibleIds_ = 0;

  // TODO: consider storing a variant of ID here
  // with symbolPath, postOrderId, and tag
  IdAndFlags idv_;
  const std::vector<IdAndFlags>* moreIdvs_ = nullptr;

  static inline bool isIdVisible(const IdAndFlags& idv,
                                 IdAndFlags::Flags filterFlags,
                                 const IdAndFlags::FlagSet& excludeFlagSet) {
    // check that all flags set in filterFlags are also set in idv.flags_
    return IdAndFlags::matchFilter(idv.flags_, filterFlags, excludeFlagSet);
  }

  bool isIdVisible(const IdAndFlags& idv) const {
    return isIdVisible(idv, filterFlags_, excludeFlagSet_);
  }

  /** Returns an iterator referring to the first element stored. */
  const IdAndFlags* beginIdAndFlags() const {
    if (moreIdvs_ == nullptr) {
      return &idv_;
    }
    return &(*moreIdvs_)[0];
  }
  /** Returns an iterator referring just past the last element stored. */
  const IdAndFlags* endIdAndFlags() const {
    const IdAndFlags* last = nullptr;
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
    const IdAndFlags* currentIdv;

    BorrowedIdsWithNameIter(const BorrowedIdsWithName* ids,
                            const IdAndFlags* currentIdv)
      : ids(ids), currentIdv(currentIdv) {
      fastForward();
    }

    /** Skip over symbols deemed invisible by the BorrowedIdsWithName. **/
    void fastForward() {
      while (currentIdv != ids->endIdAndFlags() &&
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
    inline const IdAndFlags& curIdAndFlags() const { return *currentIdv; }
  };

 private:

  int countVisibleIds(IdAndFlags::Flags flagsAnd, IdAndFlags::Flags flagsOr);

  /** Construct a BorrowedIdsWithName referring to the same IDs
      as the passed OwnedIdsWithName.
      This BorrowedIdsWithName assumes that the OwnedIdsWithName
      will continue to exist. */
  BorrowedIdsWithName(const OwnedIdsWithName& ownedIds,
                      const IdAndFlags& firstMatch,
                      IdAndFlags::Flags filterFlags,
                      IdAndFlags::FlagSet excludeFlagSet)
    : filterFlags_(filterFlags), excludeFlagSet_(std::move(excludeFlagSet)),
      idv_(firstMatch), moreIdvs_(ownedIds.moreIdvs_.get()) {
    numVisibleIds_ = countVisibleIds(ownedIds.flagsAnd_, ownedIds.flagsOr_);
    CHPL_ASSERT(isIdVisible(idv_, filterFlags, excludeFlagSet));
  }

  /** Construct a BorrowedIdsWithName referring to one ID. Requires
      that the ID will not be filtered out according to the passed
      settings arePrivateIdsIgnored and onlyMethodsFields.
    */
  BorrowedIdsWithName(IdAndFlags idv,
                      IdAndFlags::Flags filterFlags,
                      IdAndFlags::FlagSet excludeFlagSet)
    : filterFlags_(filterFlags), excludeFlagSet_(std::move(excludeFlagSet)),
      numVisibleIds_(1), idv_(std::move(idv)) {
    CHPL_ASSERT(isIdVisible(idv_, filterFlags, excludeFlagSet));
  }
 public:

  static llvm::Optional<BorrowedIdsWithName>
  createWithSingleId(ID id, uast::Decl::Visibility vis,
                     bool isField, bool isMethod, bool isParenfulFunction,
                     IdAndFlags::Flags filterFlags,
                     IdAndFlags::FlagSet excludeFlagSet) {
    auto idAndVis = IdAndFlags(id, vis, isField, isMethod, isParenfulFunction);
    if (isIdVisible(idAndVis, filterFlags, excludeFlagSet)) {
      return BorrowedIdsWithName(std::move(idAndVis),
                                 filterFlags, std::move(excludeFlagSet));
    }
    return llvm::None;
  }

  static BorrowedIdsWithName
  createWithToplevelModuleId(ID id) {
    auto vis = uast::Decl::Visibility::PUBLIC;
    bool isField = false;
    bool isMethod = false;
    bool isParenfulFunction = false;
    IdAndFlags::Flags filterFlags = 0;
    IdAndFlags::FlagSet excludeFlagSet;
    auto maybeIds = createWithSingleId(std::move(id), vis,
                                       isField, isMethod, isParenfulFunction,
                                       filterFlags, excludeFlagSet);
    CHPL_ASSERT(maybeIds.hasValue());
    return maybeIds.getValue();
  }

  static BorrowedIdsWithName
  createWithBuiltinId() {
    // Implementation only happens to coincide; there is no philosophical
    // relation between a top-level module ID and a builtin ID.
    return createWithToplevelModuleId(ID());
  }

  /** Return the number of IDs stored here */
  int numIds() const {
    return numVisibleIds_;
  }

  /** Returns the first ID in this list. */
  const ID& firstId() const {
    return idv_.id_;
  }

  /** Returns the first IdAndFlags in this list. */
  const IdAndFlags& firstIdAndFlags() const {
    return idv_;
  }

  /** Returns 'true' if the list contains only IDs that represent
      methods or fields. */
  bool containsOnlyMethodsOrFields() const;

  BorrowedIdsWithNameIter begin() const {
    return BorrowedIdsWithNameIter(this, beginIdAndFlags());
  }

  BorrowedIdsWithNameIter end() const {
    return BorrowedIdsWithNameIter(this, endIdAndFlags());
  }

  bool operator==(const BorrowedIdsWithName& other) const {
    return filterFlags_ == other.filterFlags_ &&
           excludeFlagSet_ == other.excludeFlagSet_ &&
           numVisibleIds_ == other.numVisibleIds_ &&
           idv_ == other.idv_ &&
           moreIdvs_ == other.moreIdvs_;
  }
  bool operator!=(const BorrowedIdsWithName& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(filterFlags_));
    ret = hash_combine(ret, chpl::hash(excludeFlagSet_));
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
    excludeFlagSet_.mark(context);
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
 public:
  // supporting types/enums
  enum {
    CONTAINS_FUNCTION_DECLS = 1,
    CONTAINS_USE_IMPORT = 2,
    AUTO_USES_MODULES = 4,
    METHOD_SCOPE = 8,
    CONTAINS_EXTERN_BLOCK = 16,
  };
  /** A bit-set of the flags defined in the above enum */
  using ScopeFlags = unsigned int;

 private:
  // all fields
  const Scope* parentScope_ = nullptr;
  uast::asttags::AstTag tag_ = uast::asttags::AST_TAG_UNKNOWN;
  ScopeFlags flags_ = 0;
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
    return (flags_ & (CONTAINS_USE_IMPORT|AUTO_USES_MODULES)) != 0;
  }

  /** Returns 'true' if this Scope directly contains an 'extern' block
      (with C code to supporting interoperability) */
  bool containsExternBlock() const {
    return (flags_ & CONTAINS_EXTERN_BLOCK) != 0;
  }

  /** Returns 'true' if the Scope includes the automatic 'use' for
      the standard library. */
  bool autoUsesModules() const {
    return (flags_ & AUTO_USES_MODULES) != 0;
  }

  /** Returns 'true' if this Scope represents a method's scope.
      Methods have special scoping behavior to use other fields/methods
      without writing 'this.bla'. */
  bool isMethodScope() const {
    return (flags_ & METHOD_SCOPE) != 0;
  }

  /** Returns 'true' if this Scope directly contains any Functions */
  bool containsFunctionDecls() const {
    return (flags_ & CONTAINS_FUNCTION_DECLS) != 0;
  }

  int numDeclared() const { return declared_.size(); }

  /** If the scope contains IDs with the provided name,
      append the relevant BorrowedIdsToName the the vector.
      Returns true if something was appended. */
  bool lookupInScope(UniqueString name,
                     std::vector<BorrowedIdsWithName>& result,
                     IdAndFlags::Flags filterFlags,
                     const IdAndFlags::FlagSet& excludeFlagSet) const;

  /** Check to see if the scope contains IDs with the provided name. */
  bool contains(UniqueString name) const;

  /** Gathers all of the names of symbols declared directly within this scope */
  std::set<UniqueString> gatherNames() const;

  /** Collect names that are declared directly within this scope
      but separately collect names that have multiple definitions. */
  void collectNames(std::set<UniqueString>& namesDefined,
                    std::set<UniqueString>& namesDefinedMultiply) const;

  bool operator==(const Scope& other) const {
    return parentScope_ == other.parentScope_ &&
           tag_ == other.tag_ &&
           flags_ == other.flags_ &&
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
  bool isModulePrivate_ = false;
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
                    bool isPrivate, bool isModulePrivate,
                    ShadowScope shadowScopeLevel,
                    ID visibilityClauseId,
                    std::vector<std::pair<UniqueString,UniqueString>> names)
    : scope_(scope), kind_(kind),
      isPrivate_(isPrivate), isModulePrivate_(isModulePrivate),
      shadowScopeLevel_(shadowScopeLevel),
      visibilityClauseId_(visibilityClauseId),
      names_(std::move(names))
  {
    CHPL_ASSERT(shadowScopeLevel == REGULAR_SCOPE ||
                shadowScopeLevel == SHADOW_SCOPE_ONE ||
                shadowScopeLevel == SHADOW_SCOPE_TWO);
  }

  /** Return the used/imported scope */
  const Scope* scope() const { return scope_; }

  /** Return the kind of the imported symbol */
  Kind kind() const { return kind_; }

  /** Return whether or not the imported symbol is private */
  bool isPrivate() const { return isPrivate_; }

  /** Returns whether or not the used/imported module (or enum) is private */
  bool isModulePrivate() const { return isModulePrivate_; }

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
  bool lookupName(const UniqueString &name, UniqueString &declared) const;

  /** Return a vector of pairs of (original name, new name here)
      for the names declared here. */
  const std::vector<std::pair<UniqueString,UniqueString>>& names() const;

  bool operator==(const VisibilitySymbols &other) const {
    return scope_ == other.scope_ &&
           kind_ == other.kind_ &&
           isPrivate_ == other.isPrivate_ &&
           isModulePrivate_ == other.isModulePrivate_ &&
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
    std::swap(isModulePrivate_, other.isModulePrivate_);
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
                           bool isPrivate, bool isModulePrivate,
                           VisibilitySymbols::ShadowScope shadowScopeLevel,
                           ID visibilityClauseId,
                           std::vector<std::pair<UniqueString,UniqueString>> n)
  {
    auto elt = VisibilitySymbols(scope, kind,
                                 isPrivate, isModulePrivate, shadowScopeLevel,
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
    for (const auto& sym : visibilityClauses_) {
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
    directly nested within a class/record/union.
   */
  LOOKUP_ONLY_METHODS_FIELDS = 128,

  /**
    Lookup in extern blocks
   */
  LOOKUP_EXTERN_BLOCKS = 256,

  /**
    Skip private use/import
   */
  LOOKUP_SKIP_PRIVATE_USE_IMPORT = 512,

  /**
    Skip shadow scopes (for private use)
   */
  LOOKUP_SKIP_SHADOW_SCOPES = 1024,

  /**
    Include methods in the search results (they are excluded by default).
   */
  LOOKUP_METHODS = 2048,
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
    context->markPointer(scope);
    for (const auto& elt : visibleThrough) {
      elt.mark(context);
    }
  }
};

using ScopeSet = llvm::SmallPtrSet<const Scope*, 5>;

/** The type to help maintain a checked scope */
struct CheckedScope {
  UniqueString forName;
  const Scope* scope = nullptr;

  CheckedScope(UniqueString forName,
               const Scope* scope)
    : forName(forName), scope(scope) {
  }

  bool operator==(const CheckedScope& other) const {
    return forName == other.forName &&
           scope == other.scope;
  }
  bool operator!=(const CheckedScope& other) const {
    return !(*this == other);
  }
  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(forName));
    ret = hash_combine(ret, chpl::hash(scope));
    return ret;
  }
};

using CheckedScopes = std::unordered_map<CheckedScope, IdAndFlags::FlagSet>;


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
template<> struct hash<chpl::resolution::IdAndFlags>
{
  size_t operator()(const chpl::resolution::IdAndFlags& key) const {
    return key.hash();
  }
};

template<> struct hash<chpl::resolution::IdAndFlags::FlagSet>
{
  size_t operator()(const chpl::resolution::IdAndFlags::FlagSet& key) const {
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

template <>
struct hash<chpl::resolution::CheckedScope> {
  size_t operator()(const chpl::resolution::CheckedScope& key) const {
    return key.hash();
  }
};


/// \endcond

} // end namespace std

#endif
