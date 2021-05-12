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

#ifndef CHPL_UAST_DECL_H
#define CHPL_UAST_DECL_H

#include "chpl/uast/Expression.h"
#include "chpl/uast/Sym.h"
#include "chpl/queries/UniqueString.h"

namespace chpl {
namespace uast {


/**
  This is an abstract base class for declarations
 */
class Decl : public Expression {
 protected:
  Decl(ASTTag tag, owned<Sym> sym)
    : Expression(tag, makeASTList(std::move(sym))) {
  }
  bool declContentsMatchInner(const Decl* other) const {
    return true;
  }
  void declMarkUniqueStringsInner(Context* context) const {
  }

 public:
  virtual ~Decl() = 0; // this is an abstract base class

  /** Returns the symbol declared by the declaration. */
  const Sym* sym() const {
    const ASTNode* ast = child(0);
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
 elements are all Decls. The iterator returns the declared Syms
 casted to a particular type.
 */
template<typename CastToType>
class DeclListSymIterator {
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
  DeclListSymIterator() = default;
  explicit DeclListSymIterator(ASTList::const_iterator it) : it(it) { }
  ~DeclListSymIterator() = default;

  DeclListSymIterator<CastToType>& operator=(const DeclListSymIterator<CastToType>& it) = default;

  // needs to be support == and !=
  bool operator==(const DeclListSymIterator<CastToType> rhs) const {
    return this->it == rhs.it;
  }
  bool operator!=(const DeclListSymIterator<CastToType> rhs) const {
    return this->it != rhs.it;
  }

  // needs to support * and ->
  const CastToType* operator*() const {
    const ASTNode* ast = this->it->get();
    assert(ast->isDecl());
    const Decl* d = (const Decl*) ast;
    return (const CastToType*) d->sym();
  }
  const CastToType* operator->() const {
    const ASTNode* ast = this->it->get();
    assert(ast->isDecl());
    const Decl* d = (const Decl*) ast;
    return (const CastToType*) d->sym();
  }

  // needs to support preincrement and postincrement
  DeclListSymIterator<CastToType>& operator++() {
    ++this->it;
    return *this;
  }
  DeclListSymIterator<CastToType> operator++(int) {
    DeclListSymIterator<CastToType> tmp = *this;
    ++this->it;
    return tmp;
  }

  // needs to support predecrement and postdecrement
  DeclListSymIterator<CastToType>& operator--() {
    --this->it;
    return *this;
  }
  DeclListSymIterator<CastToType> operator--(int) {
    DeclListSymIterator<CastToType> tmp = *this;
    --this->it;
    return tmp;
  }

  // needs to support + and -
  DeclListSymIterator<CastToType> operator+(const difference_type rhs) const {
    return DeclListSymIterator<CastToType>(this->it + rhs);
  }
  DeclListSymIterator<CastToType> operator-(const difference_type rhs) const {
    return DeclListSymIterator<CastToType>(this->it - rhs);
  }
  difference_type operator-(const DeclListSymIterator<CastToType> rhs) const {
    return DeclListSymIterator<CastToType>(this->it - rhs.it);
  }

  // needs to support < > <= >=
  bool operator<(const DeclListSymIterator<CastToType> rhs) const {
    return this->it < rhs.it;
  }
  bool operator>(const DeclListSymIterator<CastToType> rhs) const {
    return this->it > rhs.it;
  }
  bool operator<=(const DeclListSymIterator<CastToType> rhs) const {
    return this->it <= rhs.it;
  }
  bool operator>=(const DeclListSymIterator<CastToType> rhs) const {
    return this->it >= rhs.it;
  }

  // needs to support += and -=
  const DeclListSymIterator<CastToType>& operator+=(const difference_type& rhs) {
    this->it += rhs;
    return *this;
  }
  const DeclListSymIterator<CastToType>& operator-=(const difference_type& rhs) {
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
struct DeclListSymIteratorPair {
  DeclListSymIterator<CastToType> begin_;
  DeclListSymIterator<CastToType> end_;

  DeclListSymIteratorPair(ASTList::const_iterator begin,
                          ASTList::const_iterator end)
    : begin_(begin), end_(end) {
  }
  ~DeclListSymIteratorPair() = default;

  DeclListSymIterator<CastToType> begin() const {
    return begin_;
  }
  DeclListSymIterator<CastToType> end() const {
    return end_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
