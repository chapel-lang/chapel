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

#ifndef CHPL_UAST_ASTLIST_H
#define CHPL_UAST_ASTLIST_H

#include "chpl/uast/ASTTypes.h"
#include "chpl/util/memory.h"

#include <iterator>
#include <vector>

namespace chpl {
class Context;

namespace uast {


/**
  ASTList is just a list that owns some AST nodes.
 */
using ASTList = std::vector<owned<ASTNode>>;

/**
  Create an ASTList containing a single ast element, transferring
  ownership of that element to the list.
 */
static inline ASTList makeASTList(owned<ASTNode> ast) {
  ASTList lst;
  lst.push_back(std::move(ast));
  return lst;
}

/**
 Update an AST list with some replacement AST.

 It's kindof like swapping 'keep' and 'addin' but it tries
 to keep old AST nodes when they are the same. This allows
 for more reuse of results in the query framework.

 'keep' is some old AST
 'addin' is some new AST we wish to replace it with

 on exit, 'keep' stores the AST we need to 'keep', and anything
 not kept is stored in 'addin'. On exit, the order of nodes stored
 in 'addin' does not matter.

 The function returns 'true' if anything changed in 'keep'.
 */
bool updateASTList(ASTList& keep, ASTList& addin);

/**
 Mark UniqueStrings in an AST list.
 */
void markASTList(Context* context, const ASTList& keep);

/**
 Defines an iterator over the AST list elements.
 The iterator hides the ownership (it always returns a pointer e.g. ASTNode*)
 and casts elements to a particular type.
 */
template<typename CastToType>
class ASTListIterator {
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
  ASTListIterator() = default;
  explicit ASTListIterator(ASTList::const_iterator it) : it(it) { }
  ~ASTListIterator() = default;

  ASTListIterator<CastToType>& operator=(const ASTListIterator<CastToType>& it) = default;

  // needs to be support == and !=
  bool operator==(const ASTListIterator<CastToType> rhs) const {
    return this->it == rhs.it;
  }
  bool operator!=(const ASTListIterator<CastToType> rhs) const {
    return this->it != rhs.it;
  }

  // needs to support * and ->
  const CastToType* operator*() const {
    return this->it->get();
  }
  const CastToType* operator->() const {
    return this->it->get();
  }

  // needs to support preincrement and postincrement
  ASTListIterator<CastToType>& operator++() {
    ++this->it;
    return *this;
  }
  ASTListIterator<CastToType> operator++(int) {
    ASTListIterator<CastToType> tmp = *this;
    ++this->it;
    return tmp;
  }

  // needs to support predecrement and postdecrement
  ASTListIterator<CastToType>& operator--() {
    --this->it;
    return *this;
  }
  ASTListIterator<CastToType> operator--(int) {
    ASTListIterator<CastToType> tmp = *this;
    --this->it;
    return tmp;
  }

  // needs to support + and -
  ASTListIterator<CastToType> operator+(const difference_type rhs) const {
    return ASTListIterator<CastToType>(this->it + rhs);
  }
  ASTListIterator<CastToType> operator-(const difference_type rhs) const {
    return ASTListIterator<CastToType>(this->it - rhs);
  }
  difference_type operator-(const ASTListIterator<CastToType> rhs) const {
    return ASTListIterator<CastToType>(this->it - rhs.it);
  }

  // needs to support < > <= >=
  bool operator<(const ASTListIterator<CastToType> rhs) const {
    return this->it < rhs.it;
  }
  bool operator>(const ASTListIterator<CastToType> rhs) const {
    return this->it > rhs.it;
  }
  bool operator<=(const ASTListIterator<CastToType> rhs) const {
    return this->it <= rhs.it;
  }
  bool operator>=(const ASTListIterator<CastToType> rhs) const {
    return this->it >= rhs.it;
  }

  // needs to support += and -=
  const ASTListIterator<CastToType>& operator+=(const difference_type& rhs) {
    this->it += rhs;
    return *this;
  }
  const ASTListIterator<CastToType>& operator-=(const difference_type& rhs) {
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
struct ASTListIteratorPair {
  ASTListIterator<CastToType> begin_;
  ASTListIterator<CastToType> end_;

  ASTListIteratorPair(ASTList::const_iterator begin,
                      ASTList::const_iterator end)
    : begin_(begin), end_(end) {
  }
  ~ASTListIteratorPair() = default;

  ASTListIterator<CastToType> begin() const {
    return begin_;
  }
  ASTListIterator<CastToType> end() const {
    return end_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
