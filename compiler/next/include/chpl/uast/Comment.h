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

#ifndef CHPL_UAST_COMMENT_H
#define CHPL_UAST_COMMENT_H

#include "chpl/uast/Expression.h"
#include "chpl/queries/Location.h"

#include <string>

namespace chpl {
namespace uast {


class Builder;

/**
  This class represents a comment that might be used for documentation.
  Not all comments are represented in the AST (since the comments could
  go anywhere and that would be hard to represent). However, comments that
  are at a statement level will be represented with this type.
 */
class Comment final : public Expression {
 private:
  std::string comment_;

  Comment(std::string s)
   : Expression(asttags::Comment), comment_(std::move(s)) {
  }

  bool contentsMatchInner(const ASTNode* other) const override;
  void markUniqueStringsInner(Context* context) const override;

 public:
  ~Comment() override = default;
  static owned<Comment> build(Builder* builder, Location loc, std::string c);

  /**
   Returns the contents of this comment, including the comment
   characters (e.g. `//`) as a C string.
   */
  const char* c_str() const { return comment_.c_str(); }
  /**
   Returns the contents of this comment, including the comment
   characters (e.g. `//`) as a C++ string.
   */
  const std::string& str() const { return comment_; }
};

/**
 Defines an iterator over the AST list elements that ignores comments.
 The iterator hides the ownership (it always returns a pointer e.g. ASTNode*)
 and casts elements to a particular type.
 */
template<typename CastToType>
class ASTListNoCommentsIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = const CastToType*;
  using difference_type = ASTList::const_iterator::difference_type;
  using pointer = const CastToType**;
  using reference = const CastToType*&;

 private:
  ASTList::const_iterator it;
  ASTList::const_iterator end;

 public:
  // needs to be default-constructible, copy-constructible,
  // copy-assignable and destructible
  ASTListNoCommentsIterator() = default;
  explicit ASTListNoCommentsIterator(ASTList::const_iterator start,
                                     ASTList::const_iterator end)
    : it(start), end(end) {

    while (this->it != this->end && this->it->get()->isComment()) {
      ++this->it;
    }
  }
  ~ASTListNoCommentsIterator() = default;

  ASTListNoCommentsIterator<CastToType>& operator=(const ASTListNoCommentsIterator<CastToType>& it) = default;

  // needs to be support == and !=
  bool operator==(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it == rhs.it;
  }
  bool operator!=(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it != rhs.it;
  }

  // needs to support * and ->
  const CastToType* operator*() const {
    return (const CastToType*) this->it->get();
  }
  const CastToType* operator->() const {
    return (const CastToType*) this->it->get();
  }

  // needs to support preincrement and postincrement
  ASTListNoCommentsIterator<CastToType>& operator++() {
    do {
      ++this->it;
    } while (this->it != this->end && this->it->get()->isComment());
    return *this;
  }
  ASTListNoCommentsIterator<CastToType> operator++(int) {
    ASTListNoCommentsIterator<CastToType> tmp = *this;
    do {
      ++this->it;
    } while (this->it != this->end && this->it->get()->isComment());
    return tmp;
  }

  // needs to support < > <= >=
  bool operator<(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it < rhs.it;
  }
  bool operator>(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it > rhs.it;
  }
  bool operator<=(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it <= rhs.it;
  }
  bool operator>=(const ASTListNoCommentsIterator<CastToType> rhs) const {
    return this->it >= rhs.it;
  }

  // must be swappable but that should work with the default impl
};

template<typename CastToType>
struct ASTListNoCommentsIteratorPair {
  ASTListNoCommentsIterator<CastToType> begin_;
  ASTListNoCommentsIterator<CastToType> end_;

  ASTListNoCommentsIteratorPair(ASTList::const_iterator begin,
                                ASTList::const_iterator end)
    : begin_(begin, end), end_(end, end) {
  }
  ~ASTListNoCommentsIteratorPair() = default;

  ASTListNoCommentsIterator<CastToType> begin() const {
    return begin_;
  }
  ASTListNoCommentsIterator<CastToType> end() const {
    return end_;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
