/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallSet.h"

namespace chpl {
namespace resolution {

class MatchingIdsWithName;

/** Helper type to store an ID and key information that is relevant
    to scope resolution. */
class IdAndFlags {
 public:
  // helper types
  /// \cond DO_NOT_DOCUMENT
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
    /** A module declaration */
    MODULE = 256,
    /** Something other than a module declaration */
    NOT_MODULE = 512,
    /** A type declaration */
    TYPE = 1024,
    /** Something other than a type declaration */
    NOT_TYPE = 2048,

    // note: if adding something here, be sure to also update flagsToString and
    // the IdAndFlags constructor.
  };
  /// \endcond

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
      simplification to avoid growing the size. */
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

    /** Add a new disjunct (OR) to the set of flag combinations. Automatically
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
  friend class MatchingIdsWithName;

  // all fields
  ID id_;
  Flags flags_ = 0;

 public:
  IdAndFlags() { }

  IdAndFlags(ID id, bool isPublic, bool isMethodOrField,
             bool isParenfulFunction, bool isMethod,
             bool isModule, bool isType);

  static IdAndFlags createForModule(ID id, bool isPublic) {
    return IdAndFlags(std::move(id),
                      /* isPublic */ isPublic,
                      /* isMethodOrField */ false,
                      /* isParenfulFunction */ false,
                      /* isMethod */ false,
                      /* isModule */ true,
                      /* isType */ false);
  }

  static IdAndFlags createForBuiltinType() {
    return IdAndFlags(ID(),
                      /* isPublic */ true,
                      /* isMethodOrField */ false,
                      /* isParenfulFunction */ false,
                      /* isMethod */ false,
                      /* isModule */ false,
                      /* isType */ true);
  }

  static IdAndFlags createForBuiltinVar() {
    return IdAndFlags(ID(),
                      /* isPublic */ true,
                      /* isMethodOrField */ false,
                      /* isParenfulFunction */ false,
                      /* isMethod */ false,
                      /* isModule */ false,
                      /* isType */ false);
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
  bool isModule() const {
    return (flags_ & MODULE) != 0;
  }
  bool isType() const {
    return (flags_ & TYPE) != 0;
  }

  /** Returns true if haveFlags matches filterFlags, and does not match
      the exclude flag set. See the comments on Flags and FlagSet for
      how the matching works.
   */
  static bool matchFilter(Flags haveFlags,
                          Flags filterFlags,
                          const FlagSet& excludeFlagSet) {
    return (haveFlags & filterFlags) == filterFlags &&
           excludeFlagSet.noneMatch(haveFlags);
  }

  /** Returns true if this IdAndFlags matches filterFlags, and does not match
      the exclude flag set. See the comments on Flags and FlagSet for
      how the matching works.
   */
  bool matchesFilter(Flags filterFlags, const FlagSet& excludeFlagSet) const {
    return matchFilter(flags_, filterFlags, excludeFlagSet);
  }

  static std::string flagsToString(Flags flags);

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

/**
  Collects IDs with a particular name.
 */
class OwnedIdsWithName {
 friend class MatchingIdsWithName;

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
  /** Construct an OwnedIdsWithName containing one IdAndFlags */
  OwnedIdsWithName(IdAndFlags idv)
    : idv_(std::move(idv)),
      flagsAnd_(idv_.flags_),
      flagsOr_(idv_.flags_),
      moreIdvs_(nullptr)
  { }

  /** Append an IdAndFlags to an OwnedIdsWithName. */
  void appendIdAndFlags(IdAndFlags idv) {
    if (moreIdvs_.get() == nullptr) {
      // create the vector and add the single existing id to it
      moreIdvs_ = toOwned(new std::vector<IdAndFlags>());
      moreIdvs_->push_back(idv_);
      // flagsAnd_ and flagsOr_ will have already been set in constructor
      // from idv_.
    }
    // add the id passed
    moreIdvs_->push_back(std::move(idv));
    // update the flags
    flagsAnd_ &= idv.flags_;
    flagsOr_ |= idv.flags_;
  }

  /** Append any entries that match filterFlags and aren't excluded
      by excludeFlagSet to a MatchingIdsWithName.
      Returns 'true' if any matches were appended. */
  bool gatherMatches(MatchingIdsWithName& dst,
                     IdAndFlags::Flags filterFlags,
                     const IdAndFlags::FlagSet& excludeFlagSet) const;

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

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};


/**
  Contains IDs with a particular name that matched some filter.
 */
class MatchingIdsWithName {
 // To allow us to use the internals of OwnedIdsWithName
 friend class OwnedIdsWithName;

 public:
  using const_iterator = std::vector<IdAndFlags>::const_iterator;

 private:
  llvm::SmallVector<IdAndFlags, 1> idvs_;

  /** Construct a MatchingIdsWithName referring to the same IDs
      as the passed OwnedIdsWithName.  */
  MatchingIdsWithName(const OwnedIdsWithName& ownedIds,
                      IdAndFlags::Flags filterFlags,
                      const IdAndFlags::FlagSet& excludeFlagSet) {
    ownedIds.gatherMatches(*this, filterFlags, excludeFlagSet);
  }

  /** Construct a MatchingIdsWithName referring to one ID. Assumes
      that the caller did any necessary filtering.
    */
  MatchingIdsWithName(IdAndFlags idv) {
    idvs_.push_back(std::move(idv));
  }

 public:
  /**
    Iterator that yields IDs rather than IdAndFlags.
   */
  class MatchingIdsWithNameIter {
    friend class MatchingIdsWithName;
   private:
    /** The ID this iterator is pointing too. */
    const IdAndFlags* currentIdv;

    MatchingIdsWithNameIter(const IdAndFlags* currentIdv)
      : currentIdv(currentIdv) {
    }

   public:
    bool operator!=(const MatchingIdsWithNameIter& other) const {
      return currentIdv != other.currentIdv;
    }
    MatchingIdsWithNameIter& operator++() {
      currentIdv++;
      return *this;
    }
    inline const ID& operator*() const { return currentIdv->id_; }

    // TODO: instead of having curIdAndFlags here, write support code
    // so that the loop iterating requests IDs or IdAndFlags:
    //   for (const IdAndFlags& x : myMatchingIdsWithName)
    // vs
    //   for (const ID& x : myMatchingIdsWithName.ids())
    inline const IdAndFlags& curIdAndFlags() const { return *currentIdv; }

    // iterator traits
    using difference_type = std::ptrdiff_t;
    using value_type = ID;
    using pointer = const ID*;
    using reference = const ID&;
    using iterator_category = std::forward_iterator_tag;
  };

  static MatchingIdsWithName
  createWithIdAndFlags(IdAndFlags idv) {
    return MatchingIdsWithName(std::move(idv));
  }

  /** Construct a empty MatchingIdsWithName containing no IDs. */
  MatchingIdsWithName() { }

  /** Append an IdAndFlags. */
  void append(IdAndFlags idv) {
    idvs_.push_back(std::move(idv));
  }

  /** Remove any duplicates IDs present here. */
  void removeDuplicateIds(); 

  /** Truncate to a particular number of IDs. Must be < numIds(). */
  void truncate(int sz);

  /** Remove all IDs. */ 
  void clear();

  /** Returns 'true' if this MatchingIdsWithName has no IDs */
  bool isEmpty() const { return idvs_.empty(); }

  /** Return the number of IDs stored here */
  int numIds() const {
    return (int) idvs_.size();
  }

  /** Returns the first ID in this list. */
  const ID& firstId() const {
    return idvs_.front().id();
  }

  /** Returns the first IdAndFlags in this list. */
  const IdAndFlags& firstIdAndFlags() const {
    return idvs_.front();
  }

  /** Return the i'th ID in the list */
  const ID& id(int i) const {
    CHPL_ASSERT(0 <= i && i < (int) idvs_.size());
    return idvs_[i].id();
  }

  /** Return the i'th IdAndFlags in the list */
  const IdAndFlags& idAndFlags(int i) const {
    CHPL_ASSERT(0 <= i && i < (int) idvs_.size());
    return idvs_[i];
  }

  /** Returns 'true' if the list contains only IDs that represent
      methods or fields. */
  bool containsOnlyMethodsOrFields() const;

  MatchingIdsWithNameIter begin() const {
    if (idvs_.empty()) {
      return MatchingIdsWithNameIter(nullptr);
    }
    const IdAndFlags* ptr = &idvs_.front();
    return MatchingIdsWithNameIter(ptr);
  }

  MatchingIdsWithNameIter end() const {
    if (idvs_.empty()) {
      return MatchingIdsWithNameIter(nullptr);
    }
    const IdAndFlags* ptr = &idvs_.back();
    return MatchingIdsWithNameIter(ptr + 1);
  }

  bool operator==(const MatchingIdsWithName& other) const {
    return idvs_ == other.idvs_;
  }
  bool operator!=(const MatchingIdsWithName& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    for (const auto& x : idvs_) {
      ret = hash_combine(ret, chpl::hash(x));
    }
    return ret;
  }

  void mark(Context* context) const {
    for (auto const& elt : idvs_) {
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
  Gather matches to 'name' that match 'filterFlags' and aren't
  excluded by 'excludeFlags'. Store any gathered into 'result'
 */
bool lookupInDeclMap(const DeclMap& declared,
                     UniqueString name,
                     MatchingIdsWithName& result,
                     IdAndFlags::Flags filterFlags,
                     const IdAndFlags::FlagSet& excludeFlags);

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
  /// \cond DO_NOT_DOCUMENT
  enum {
    CONTAINS_FUNCTION_DECLS = 1,
    CONTAINS_USE_IMPORT = 2,
    CONTAINS_REQUIRE = 4,
    AUTO_USES_MODULES = 8,
    METHOD_SCOPE = 16,
    CONTAINS_EXTERN_BLOCK = 32,
  };
  /// \endcond

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
  Scope(Context* context, const uast::AstNode* ast, const Scope* parentScope,
        bool autoUsesModules);

  /** Add a builtin type with the provided name. This needs to
      be called to populate the root scope with builtins. */
  void addBuiltinType(UniqueString name);

  /** Add a builtin var with the provided name. This needs to
      be called to populate the root scope with builtins. */
  void addBuiltinVar(UniqueString name);

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

  const DeclMap& declared() const { return declared_; }

  /** Returns 'true' if this Scope directly contains use or import statements
      including the automatic 'use' for the standard library. */
  bool containsUseImport() const {
    return (flags_ & (CONTAINS_USE_IMPORT|AUTO_USES_MODULES)) != 0;
  }

  /** Returns 'true' if this Scope directly contains 'require' statements */
  bool containsRequire() const {
    return (flags_ & CONTAINS_REQUIRE) != 0;
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
                     MatchingIdsWithName& result,
                     IdAndFlags::Flags filterFlags,
                     const IdAndFlags::FlagSet& excludeFlagSet) const {
    return lookupInDeclMap(declared_, name, result,
                           filterFlags, excludeFlagSet);
  }

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

          use Foo;

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

  /** Returns 'true' if the name 'name' could be brought in by use/import
      represented here.
   */
  bool mightHaveName(UniqueString name) const;

  /** Lookup the declared name for a given name
      Returns true if `name` is found in the list of renamed names and
      stores the declared name in `declared`
      Returns false if `name` is not found
  */
  bool lookupName(UniqueString name, UniqueString &declared) const;

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
  std::vector<ID> modulesNamedInUseOrImport_;

 public:
  using VisibilitySymbolsIterable = Iterable<std::vector<VisibilitySymbols>>;
  using ModuleIdsIterator = Iterable<std::vector<ID>>;

  ResolvedVisibilityScope(const Scope* scope)
    : scope_(scope)
  { }

  /** Return the scope */
  const Scope *scope() const { return scope_; }

  /** Return an iterator over the visibility clauses */
  VisibilitySymbolsIterable visibilityClauses() const {
    return VisibilitySymbolsIterable(visibilityClauses_);
  }

  /** Return an iterator over the modules named in use/import */
  ModuleIdsIterator modulesNamedInUseOrImport() const {
    return ModuleIdsIterator(modulesNamedInUseOrImport_);
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

  /** Add a module ID for a module named in use/import */
  void addModuleNamedInUseOrImport(ID id) {
    modulesNamedInUseOrImport_.push_back(std::move(id));
  }

  bool operator==(const ResolvedVisibilityScope& other) const {
    return scope_ == other.scope_ &&
           visibilityClauses_ == other.visibilityClauses_ &&
           modulesNamedInUseOrImport_ == other.modulesNamedInUseOrImport_;
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
    for (const auto& id: modulesNamedInUseOrImport_) {
      id.mark(context);
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

/// \cond DO_NOT_DOCUMENT
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
/// \endcond

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
    UniqueString usedImportedThingName;
    const Scope* usedImportedScope = nullptr;
    bool fromUseImport = false;

    // this indicates a method receiver scope
    const Scope* methodReceiverScope = nullptr;

    // this indicates a parent scope
    const Scope* parentScope = nullptr;

    // these cover other cases
    bool automaticModule = false;
    bool toplevelModule = false;
    bool containingModule = false;
    bool externBlock = false;
    bool rootScope = false;

    bool operator==(const VisibilityTraceElt& other) const {
      return shadowScope == other.shadowScope &&
             resolvedVisibilityScope == other.resolvedVisibilityScope &&
             visibilityClauseId == other.visibilityClauseId &&
             visibilityStmtKind == other.visibilityStmtKind &&
             renameFrom == other.renameFrom &&
             usedImportedThingName == other.usedImportedThingName &&
             usedImportedScope == other.usedImportedScope &&
             fromUseImport == other.fromUseImport &&
             methodReceiverScope == other.methodReceiverScope &&
             parentScope == other.parentScope &&
             automaticModule == other.automaticModule &&
             toplevelModule == other.toplevelModule &&
             containingModule == other.containingModule &&
             externBlock == other.externBlock &&
             rootScope == other.rootScope;
    }
    bool operator!=(const VisibilityTraceElt& other) const {
      return !(*this == other);
    }
    void mark(Context* context) const {
      context->markPointer(resolvedVisibilityScope);
      renameFrom.mark(context);
      usedImportedThingName.mark(context);
      context->markPointer(usedImportedScope);
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

/** This type collects all public symbols used in a module */
class ModulePublicSymbols {
  DeclMap syms_;

 public:
  ModulePublicSymbols(DeclMap syms) : syms_(std::move(syms)) { }

  const DeclMap& syms() const { return syms_; }

  bool lookupInModule(UniqueString name,
                      MatchingIdsWithName& result,
                      IdAndFlags::Flags filterFlags,
                      const IdAndFlags::FlagSet& excludeFlags) const {
    return lookupInDeclMap(syms_, name, result, filterFlags, excludeFlags);
  }

  bool operator==(const ModulePublicSymbols& other) const {
    return syms_ == other.syms_;
  }
  bool operator!=(const ModulePublicSymbols& other) const {
    return !(*this == other);
  }
  static bool update(owned<ModulePublicSymbols>& keep,
                     owned<ModulePublicSymbols>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    for (const auto& pair: syms_) {
      pair.first.mark(context);
      pair.second.mark(context);
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
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

template<> struct hash<chpl::resolution::MatchingIdsWithName>
{
  size_t operator()(const chpl::resolution::MatchingIdsWithName& key) const {
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
