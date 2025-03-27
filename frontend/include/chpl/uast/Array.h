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
       associative_;

  Array(AstList children, bool trailingComma, bool associative)
    : AstNode(asttags::Array, std::move(children)),
      trailingComma_(trailingComma),
      associative_(associative) {
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
    return this->numExprs() > 0 && this->expr(0)->isArrayRow();
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
  class FlatArrayIterator {
   private:
    std::vector<AstList::const_iterator> rowIterStack;
    AstList::const_iterator topLevelEnd;

    /*
     * Descend to the final array dimension, adding an iterator for each
     * dimension along the way.
     */
    void descend() {
      if (!rowIterStack.empty()) {
        while (auto row = (*rowIterStack.back())->toArrayRow()) {
          this->rowIterStack.push_back(row->begin());
        }
      }
    }

   public:
    FlatArrayIterator(const Array* iterand, bool end = false) {
      if (end) {
        rowIterStack.push_back(iterand->end());
      } else {
        rowIterStack.push_back(iterand->begin());
        descend();
      }
      topLevelEnd = iterand->end();
    }

    bool operator==(const FlatArrayIterator rhs) const {
      return this->rowIterStack == rhs.rowIterStack;
    }
    bool operator!=(const FlatArrayIterator rhs) const {
      return !(*this == rhs);
    }

    const AstNode* operator*() const {
      return (const AstNode*)this->rowIterStack.back()->get();
    }
    const AstNode* operator->() const { return operator*(); }

    FlatArrayIterator& operator++() {
      ++rowIterStack.back();

      // Pop up the stack until we're either at the top level, or at a row we
      // haven't already gone through.
      while (rowIterStack.size() > 1) {
        AstList::const_iterator rowEnd;
        auto parent = rowIterStack[rowIterStack.size() - 2];
        if (auto parentRow = (*parent)->toArrayRow()) {
          rowEnd = parentRow->end();
        } else if (auto parentArray = (*parent)->toArray()) {
          rowEnd = parentArray->end();
        } else {
          CHPL_ASSERT(false &&
                      "shouldn't be reachable, descended into an array "
                      "dimension not contained within an array or array row");
        }

        if (rowIterStack.back() == rowEnd) {
          rowIterStack.pop_back();
          ++rowIterStack.back();
          if (rowIterStack.back() != topLevelEnd) {
            descend();
          }
        } else {
          break;
        }
      }

      return *this;
    }
    FlatArrayIterator operator++(int) {
      FlatArrayIterator tmp = *this;
      operator++();
      return tmp;
    }
  };

  struct FlatArrayIteratorPair {
    FlatArrayIterator begin_;
    FlatArrayIterator end_;

    FlatArrayIteratorPair(FlatArrayIterator begin, FlatArrayIterator end)
        : begin_(begin), end_(end) {}
    ~FlatArrayIteratorPair() = default;

    FlatArrayIterator begin() const {
      return begin_;
    }
    FlatArrayIterator end() const {
      return end_;
    }
  };

  /**
    Return a way to iterate over the expressions of this array, transparently
    flattened into a single list if multi-dimensional.
  */
  FlatArrayIteratorPair flattenedExprs() const {
    FlatArrayIterator begin(this);
    FlatArrayIterator end(this, /* end */ true);
    return FlatArrayIteratorPair(begin, end);
  }
};


} // end namespace uast
} // end namespace chpl

#endif
