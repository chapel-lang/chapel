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

using IdAndVis = std::pair<ID, uast::Decl::Visibility>;

/**
  Collects IDs with a particular name. These can be referred to
  by a BorrowedIdsWithName in a way that avoids copies.
 */
class OwnedIdsWithName {
 friend class BorrowedIdsWithName;

 private:
  // If there is just one ID with this name, it is stored here,
  // and moreIds == nullptr.
  IdAndVis id_;
  // If there is more than one, all are stored in here,
  // and id redundantly stores the first one.
  // This field is 'owned' in order to allow reuse of pointers to it.
  owned<std::vector<IdAndVis>> moreIds_;

 public:
  /** Construct an OwnedIdsWithName containing one ID. */
  OwnedIdsWithName(ID id, uast::Decl::Visibility vis)
    : id_(std::make_pair(std::move(id), vis)), moreIds_(nullptr)
  { }

  /** Append an ID to an OwnedIdsWithName. */
  void appendIdAndVis(ID id, uast::Decl::Visibility vis) {
    if (moreIds_.get() == nullptr) {
      // create the vector and add the single existing id to it
      moreIds_ = toOwned(new std::vector<IdAndVis>());
      moreIds_->push_back(id_);
    }
    // add the id passed
    moreIds_->push_back(std::make_pair(std::move(id), vis));
  }

  bool operator==(const OwnedIdsWithName& other) const {
    if (id_ != other.id_)
      return false;

    if ((moreIds_.get()==nullptr) != (other.moreIds_.get()==nullptr))
      return false;

    if (moreIds_.get()==nullptr && other.moreIds_.get()==nullptr)
      return true;

    // otherwise, check the vector elements
    return *moreIds_.get() == *other.moreIds_.get();
  }
  bool operator!=(const OwnedIdsWithName& other) const {
    return !(*this == other);
  }
  void mark(Context* context) const {
    id_.first.mark(context);
    if (auto ptr = moreIds_.get()) {
      for (auto const& elt : *ptr) {
        context->markOwnedPointer(&elt.first);
      }
    }
  }

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  llvm::Optional<BorrowedIdsWithName> borrow(bool arePrivateIdsIgnored) const;

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
  static inline bool isIdVisible(const IdAndVis& id, bool arePrivateIdsIgnored) {
    return !arePrivateIdsIgnored || id.second != uast::Decl::PRIVATE;
  }

  bool isIdVisible(const IdAndVis& id) const {
    return isIdVisible(id, arePrivateIdsIgnored);
  }

  /** Returns an iterator referring to the first element stored. */
  const IdAndVis* beginIdAndVis() const {
    if (moreIds_ == nullptr) {
      return &id_;
    }
    return &(*moreIds_)[0];
  }
  /** Returns an iterator referring just past the last element stored. */
  const IdAndVis* endIdAndVis() const {
    const IdAndVis* last = nullptr;
    if (moreIds_ == nullptr) {
      last = &id_;
    } else {
      last = &moreIds_->back();
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
    const IdAndVis* currentId;

    BorrowedIdsWithNameIter(const BorrowedIdsWithName* ids, const IdAndVis* currentId)
      : ids(ids), currentId(currentId) {
      fastForward();
    }

    /** Skip over symbols deemed invisible by the BorrowedIdsWithName. **/
    void fastForward() {
      while (currentId != ids->endIdAndVis() && !ids->isIdVisible(*currentId)) {
        currentId++;
      }
    }

   public:
    bool operator!=(const BorrowedIdsWithNameIter& other) const {
      return ids != other.ids || currentId != other.currentId;
    }
    BorrowedIdsWithNameIter& operator++() {
      currentId++;
      fastForward();
      return *this;
    }
    inline const ID& operator*() const { return currentId->first; }
  };
 private:
  /**
    Whether or not this list of IDs should include the private IDs
    from the scope.
   */
  bool arePrivateIdsIgnored;
  /** How many IDs are visible in this list. */
  int numVisibleIds_;
  // TODO: consider storing a variant of ID here
  // with symbolPath, postOrderId, and tag
  IdAndVis id_;
  const std::vector<IdAndVis>* moreIds_ = nullptr;

  /** Construct a BorrowedIdsWithName referring to the same IDs
      as the passed OwnedIdsWithName.
      This BorrowedIdsWithName assumes that the OwnedIdsWithName
      will continue to exist. */
  BorrowedIdsWithName(IdAndVis id, const std::vector<IdAndVis>* moreIds,
                      bool arePrivateIdsIgnored)
    : arePrivateIdsIgnored(arePrivateIdsIgnored), id_(id),
      moreIds_(moreIds) {
    if(moreIds_ == nullptr) {
      numVisibleIds_ = 1;
      return;
    }

    // Count all the visible IDs.
    numVisibleIds_ = 0;
    for (const auto& id : *moreIds_) {
      if (isIdVisible(id)) numVisibleIds_ += 1;
    }
  }

  /** Construct a BorrowedIdsWithName referring to one ID. Requires
      that the ID is visible. */
  BorrowedIdsWithName(IdAndVis id, bool arePrivateIdsIgnored = true)
    : arePrivateIdsIgnored(arePrivateIdsIgnored),
      numVisibleIds_(1), id_(std::move(id)) {
    assert(isIdVisible(id_, arePrivateIdsIgnored));
  }
 public:

  static llvm::Optional<BorrowedIdsWithName>
  createWithSingleId(ID id, uast::Decl::Visibility vis,
                     bool arePrivateIdsIgnored = true) {
    auto idAndVis = std::make_pair(id, vis);
    if (isIdVisible(idAndVis, arePrivateIdsIgnored)) {
      return BorrowedIdsWithName(std::move(idAndVis), arePrivateIdsIgnored);
    }
    return llvm::None;
  }

  static BorrowedIdsWithName
  createWithSinglePublicId(ID id, bool arePrivateIdsIgnored = true) {
    auto maybeIds = createWithSingleId(std::move(id),
                                       uast::Decl::Visibility::PUBLIC,
                                       arePrivateIdsIgnored);
    assert(maybeIds);
    return maybeIds.getValue();
  }

  /** Return the number of IDs stored here */
  int numIds() const {
    return numVisibleIds_;
  }

  /** Returns the first ID in this list. */
  const ID& firstId() const {
    return id_.first;
  }

  BorrowedIdsWithNameIter begin() const {
    return BorrowedIdsWithNameIter(this, beginIdAndVis());
  }

  BorrowedIdsWithNameIter end() const {
    return BorrowedIdsWithNameIter(this, endIdAndVis());
  }

  bool operator==(const BorrowedIdsWithName& other) const {
    return arePrivateIdsIgnored == other.arePrivateIdsIgnored &&
           numVisibleIds_ == other.numVisibleIds_ &&
           id_ == other.id_ &&
           moreIds_ == other.moreIds_;
  }
  bool operator!=(const BorrowedIdsWithName& other) const {
    return !(*this == other);
  }

  size_t hash() const {
    size_t ret = 0;
    ret = hash_combine(ret, chpl::hash(arePrivateIdsIgnored));
    ret = hash_combine(ret, chpl::hash(numVisibleIds_));
    ret = hash_combine(ret, chpl::hash(moreIds_));
    if (moreIds_ == nullptr) {
      ret = hash_combine(ret, chpl::hash(id_));
    } else {
      for (const auto& x : *moreIds_) {
        ret = hash_combine(ret, chpl::hash(x));
      }
    }
    return ret;
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
  bool containsUseImport_ = false;
  bool containsFunctionDecls_ = false;
  bool autoUsesModules_ = false;
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

  /** Returns 'true' if the Scope includes the automatic 'use' for
      the standard library. */
  bool autoUsesModules() const {
    return autoUsesModules_;
  }

  /** Returns 'true' if this Scope directly contains any Functions */
  bool containsFunctionDecls() const { return containsFunctionDecls_; }

  int numDeclared() const { return declared_.size(); }

  /** If the scope contains IDs with the provided name,
      append the relevant BorrowedIdsToName the the vector.
      Returns true if something was appended. */
  bool lookupInScope(UniqueString name,
                     std::vector<BorrowedIdsWithName>& result,
                     bool arePrivateIdsIgnored) const {
    auto search = declared_.find(name);
    if (search != declared_.end()) {
      // There might not be any IDs that are visible to us, so borrow returns
      // an optional list.
      auto borrowedIds = search->second.borrow(arePrivateIdsIgnored);
      if (borrowedIds.hasValue()) {
        result.push_back(std::move(borrowedIds.getValue()));
        return true;
      }
    }
    return false;
  }

  bool operator==(const Scope& other) const {
    return parentScope_ == other.parentScope_ &&
           tag_ == other.tag_ &&
           containsUseImport_ == other.containsUseImport_ &&
           containsFunctionDecls_ == other.containsFunctionDecls_ &&
           autoUsesModules_ == other.autoUsesModules_ &&
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

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const {
    ss << "Scope ";
    ss << tagToString(tag());
    ss << " ";
    id().stringify(ss, stringKind);
    ss << " ";
    ss << std::to_string(numDeclared());
  }

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

 private:
  const Scope* scope_; // Scope of the Module etc
                       // This could technically be an ID but basically
                       // anything we do with it needs a Scope* anyway.
  Kind kind_ = SYMBOL_ONLY;
  bool isPrivate_ = true;

  // the names/renames:
  //  pair.first is the name as declared
  //  pair.second is the name here
  std::vector<std::pair<UniqueString,UniqueString>> names_;

 public:
  VisibilitySymbols() { }
  VisibilitySymbols(const Scope* scope, Kind kind, bool isPrivate,
                    std::vector<std::pair<UniqueString,UniqueString>> names)
    : scope_(scope), kind_(kind), isPrivate_(isPrivate),
      names_(std::move(names))
  { }

  /** Return the imported scope */
  const Scope* scope() const { return scope_; }

  /** Return the kind of the imported symbol */
  Kind kind() const { return kind_; }

  /** Return whether or not the imported symbol is private */
  bool isPrivate() const { return isPrivate_; }

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
           names_ == other.names_ &&
           isPrivate_ == other.isPrivate_;
  }
  bool operator!=(const VisibilitySymbols& other) const {
    return !(*this == other);
  }

  void swap(VisibilitySymbols& other) {
    std::swap(scope_, other.scope_);
    std::swap(kind_, other.kind_);
    names_.swap(other.names_);
    std::swap(isPrivate_, other.isPrivate_);
  }

  static bool update(VisibilitySymbols& keep,
                     VisibilitySymbols& addin) {
    return defaultUpdate(keep, addin);
  }

  void mark(Context* context) const {
    context->markPointer(scope_);
    for (auto p : names_) {
      p.first.mark(context);
      p.second.mark(context);
    }
  }
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
                           std::vector<std::pair<UniqueString,UniqueString>> n)
  {
    visibilityClauses_.emplace_back(scope, kind, isPrivate, std::move(n));
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
};

/*
 * Type of visibility statement (use or import)
 */
enum VisibilityStmtKind {
  VIS_USE,
  VIS_IMPORT,
};

enum {
  LOOKUP_DECLS = 1,
  LOOKUP_IMPORT_AND_USE = 2,
  LOOKUP_PARENTS = 4,
  LOOKUP_TOPLEVEL = 8,
  LOOKUP_INNERMOST = 16,
  LOOKUP_SKIP_PRIVATE_VIS = 32,
};

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
