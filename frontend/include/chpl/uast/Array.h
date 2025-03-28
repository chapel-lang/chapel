/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_ARRAY_H
#define CHPL_UAST_ARRAY_H

#include "chpl/framework/Location.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/ArrayRow.h"
#include <iterator>

namespace chpl {
namespace uast {


/**
  This class represents an array expression. For example:

  \rst
  .. code-block:: chapel

      // Example 1:
      var a = [ 1, 2, 3 ];

  \endrst

  An array expression will never contain comments.
 */
class Array final : public AstNode {
 friend class AstNode;

 private:
  bool trailingComma_,
       associative_,
       isMultiDim_;

  Array(AstList children, bool trailingComma, bool associative)
      : AstNode(asttags::Array, std::move(children)),
        trailingComma_(trailingComma),
        associative_(associative) {
    isMultiDim_ = this->numExprs() > 0 && this->expr(0)->isArrayRow();
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(trailingComma_);
    ser.write(associative_);
  }

  explicit Array(Deserializer& des)
    : AstNode(asttags::Array, des) {
    trailingComma_ = des.read<bool>();
    associative_ = des.read<bool>();
  }

  bool contentsMatchInner(const AstNode* other) const override {
    const Array* rhs = other->toArray();
    return this->trailingComma_ == rhs->trailingComma_ &&
           this->associative_ == rhs->associative_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpInner(const DumpSettings& s) const;

  /** Get an iterator to the first expression in this array */
  AstList::const_iterator begin() const {
    return children_.begin();
  }

  /** Get an iterator to one past the last expression in this array */
  AstList::const_iterator end() const {
    return children_.end();
  }

 public:
  ~Array() override = default;

  /**
   Create and return an Array expression.
   */
  static owned<Array> build(Builder* builder, Location loc,
                            AstList exprs, bool trailingComma=false,
                            bool associative=false);

  bool hasTrailingComma() const { return this->trailingComma_; }
  bool isAssociative() const { return this->associative_; }

  /**
    Return a way to iterate over the expressions of this array.
  */
  AstListIteratorPair<AstNode> exprs() const {
    return AstListIteratorPair<AstNode>(this->begin(), this->end());
  }

  /**
    Return the number of expressions in this array.
  */
  int numExprs() const {
    return this->numChildren();
  }

  /**
    Return the i'th expression in this array.
  */
  const AstNode* expr(int i) const {
    const AstNode* ast = this->child(i);
    return ast;
  }

  /**
    Return whether this is a multi-dimensional array.
  */
  bool isMultiDim() const {
    return this->isMultiDim_;
  }

  /**
   * Return the shape of this multi-dim array, as a list of dimension lengths.
  */
  std::vector<int> shape() const {
    CHPL_ASSERT(this->isMultiDim());
    std::vector<int> ret;
    ret.emplace_back(this->numExprs());
    auto cur = this->expr(0);
    while(cur->toArrayRow()) {
      ret.emplace_back(cur->toArrayRow()->numExprs());
      cur = cur->toArrayRow()->expr(0);
    }
    return ret;
  }

  /**
   * An iterator that flattens a multi-dimensional array into a single list.
   */
  class FlatteningArrayIterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = AstList::const_iterator::value_type;
    using difference_type = AstList::const_iterator::difference_type;
    using pointer = AstList::const_iterator::pointer;
    using reference = AstList::const_iterator::reference;

   private:
    // Stack of current row iterator positions, one for each dimension. The
    // bottom iterates over the array itself, and the top iterates over a row of
    // innermost dimension.
    // Each entry is a pair of (current, end) iterators.
    std::vector<std::pair<AstList::const_iterator, AstList::const_iterator>>
        rowIterStack;

    /*
     * Descend to the innermost array dimension, adding an iterator for each
     * dimension along the way.
     */
    void descendDims() {
      CHPL_ASSERT(!rowIterStack.empty() && "should not be possible");
      while (auto row = (*rowIterStack.back().first)->toArrayRow()) {
        this->rowIterStack.emplace_back(row->begin(), row->end());
      }
    }

   public:
    FlatteningArrayIterator(const Array* iterand, bool end = false) {
      if (end) {
        rowIterStack.emplace_back(iterand->end(), iterand->end());
      } else {
        rowIterStack.emplace_back(iterand->begin(), iterand->end());
        descendDims();
      }
    }

    bool operator==(const FlatteningArrayIterator rhs) const {
      // Should only be necessary to compare the innermost-dimension iterator.
      return this->rowIterStack.back().first == rhs.rowIterStack.back().first;
    }
    bool operator!=(const FlatteningArrayIterator rhs) const {
      return !(*this == rhs);
    }

    const AstNode* operator*() const {
      return (const AstNode*)this->rowIterStack.back().first->get();
    }
    const AstNode* operator->() const { return operator*(); }

    FlatteningArrayIterator& operator++() {
      // Pop up the stack until we're either at the top level, or at a row we
      // haven't already gone through.
      while (++rowIterStack.back().first == rowIterStack.back().second) {
        // Special case: leave the top level array iterator on the stack
        // when it hits the end.
        if (rowIterStack.size() == 1) return *this;
        rowIterStack.pop_back();
      }

      // We're in an unfinished row; continue iteration from the innermost
      // dimension under this row.
      descendDims();
      return *this;
    }

    FlatteningArrayIterator operator++(int) {
      FlatteningArrayIterator tmp = *this;
      operator++();
      return tmp;
    }
  };

  struct FlatteningArrayIteratorPair {
    FlatteningArrayIterator begin_;
    FlatteningArrayIterator end_;

    FlatteningArrayIteratorPair(FlatteningArrayIterator begin,
                                FlatteningArrayIterator end)
        : begin_(begin), end_(end) {}
    ~FlatteningArrayIteratorPair() = default;

    FlatteningArrayIterator begin() const { return begin_; }
    FlatteningArrayIterator end() const { return end_; }
  };

  /**
    Return a way to iterate over the expressions of this array, transparently
    flattened into a single list if multi-dimensional.
  */
  FlatteningArrayIteratorPair flattenedExprs() const {
    FlatteningArrayIterator begin(this);
    FlatteningArrayIterator end(this, /* end */ true);
    return FlatteningArrayIteratorPair(begin, end);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
