/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_SYMDECL_H
#define CHPL_UAST_SYMDECL_H

#include "chpl/uast/Decl.h"
#include "chpl/uast/Sym.h"
#include "chpl/queries/UniqueString.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations that
  declare a single symbol. All Syms have a corresponding SymDecl.
 */
class SymDecl : public Decl {
 protected:
  int8_t symChildNum_;
  SymDecl(ASTTag tag, owned<Sym> sym)
    : Decl(tag, makeASTList(std::move(sym))), symChildNum_(0) {
  }
  SymDecl(ASTTag tag, ASTList children, int8_t symChildNum)
    : Decl(tag, std::move(children)), symChildNum_(symChildNum) {
  }
  bool symDeclContentsMatchInner(const SymDecl* other) const {
    return this->symChildNum_ == other->symChildNum_ &&
           declContentsMatchInner(other);
  }
  void symDeclMarkUniqueStringsInner(Context* context) const {
    declMarkUniqueStringsInner(context);
  }

 public:
  virtual ~SymDecl() = 0; // this is an abstract base class

  /** Returns the symbol declared by the declaration. */
  const Sym* sym() const {
    const ASTNode* ast = child(symChildNum_);
    assert(ast->isSym());
    return (const Sym*) ast;
  }
  UniqueString name() const {
    return sym()->name();
  }
  Sym::Visibility visibility() const {
    return sym()->visibility();
  }
};

/**
 Defines an iterator over the AST list elements when those
 elements are all SymDecls. The iterator returns the declared Syms
 casted to a particular type.
 */
template<typename CastToType>
class SymDeclListSymIterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = const CastToType*;
  using difference_type = ASTList::const_iterator::difference_type;
  using pointer = const CastToType**;
  using reference = const CastToType*&;

 private:
  ASTList::const_iterator it;

 public:
  // needs to be default-constructible, copy-constructible,
  // copy-assignable and destructible
  SymDeclListSymIterator() = default;
  explicit SymDeclListSymIterator(ASTList::const_iterator it) : it(it) { }
  ~SymDeclListSymIterator() = default;

  SymDeclListSymIterator<CastToType>& operator=(const SymDeclListSymIterator<CastToType>& it) = default;

  // needs to be support == and !=
  bool operator==(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it == rhs.it;
  }
  bool operator!=(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it != rhs.it;
  }

  // needs to support * and ->
  const CastToType* operator*() const {
    const ASTNode* ast = this->it->get();
    assert(ast->isSymDecl());
    const SymDecl* d = (const SymDecl*) ast;
    return (const CastToType*) d->sym();
  }
  const CastToType* operator->() const {
    const ASTNode* ast = this->it->get();
    assert(ast->isSymDecl());
    const SymDecl* d = (const SymDecl*) ast;
    return (const CastToType*) d->sym();
  }

  // needs to support preincrement and postincrement
  SymDeclListSymIterator<CastToType>& operator++() {
    ++this->it;
    return *this;
  }
  SymDeclListSymIterator<CastToType> operator++(int) {
    SymDeclListSymIterator<CastToType> tmp = *this;
    ++this->it;
    return tmp;
  }

  // needs to support predecrement and postdecrement
  SymDeclListSymIterator<CastToType>& operator--() {
    --this->it;
    return *this;
  }
  SymDeclListSymIterator<CastToType> operator--(int) {
    SymDeclListSymIterator<CastToType> tmp = *this;
    --this->it;
    return tmp;
  }

  // needs to support + and -
  SymDeclListSymIterator<CastToType> operator+(const difference_type rhs) const {
    return SymDeclListSymIterator<CastToType>(this->it + rhs);
  }
  SymDeclListSymIterator<CastToType> operator-(const difference_type rhs) const {
    return SymDeclListSymIterator<CastToType>(this->it - rhs);
  }
  difference_type operator-(const SymDeclListSymIterator<CastToType> rhs) const {
    return SymDeclListSymIterator<CastToType>(this->it - rhs.it);
  }

  // needs to support < > <= >=
  bool operator<(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it < rhs.it;
  }
  bool operator>(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it > rhs.it;
  }
  bool operator<=(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it <= rhs.it;
  }
  bool operator>=(const SymDeclListSymIterator<CastToType> rhs) const {
    return this->it >= rhs.it;
  }

  // needs to support += and -=
  const SymDeclListSymIterator<CastToType>& operator+=(const difference_type& rhs) {
    this->it += rhs;
    return *this;
  }
  const SymDeclListSymIterator<CastToType>& operator-=(const difference_type& rhs) {
    this->it -= rhs;
    return *this;
  }

  // support the [] operator
  const CastToType* operator[](const int rhs) {
    return this->it[rhs];
  }

  // must be swappable but that should work with the default impl
};

template<typename CastToType>
struct SymDeclListSymIteratorPair {
  SymDeclListSymIterator<CastToType> begin_;
  SymDeclListSymIterator<CastToType> end_;

  SymDeclListSymIteratorPair(ASTList::const_iterator begin,
                             ASTList::const_iterator end)
    : begin_(begin), end_(end) {
  }
  ~SymDeclListSymIteratorPair() = default;

  SymDeclListSymIterator<CastToType> begin() const {
    return begin_;
  }
  SymDeclListSymIterator<CastToType> end() const {
    return end_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
