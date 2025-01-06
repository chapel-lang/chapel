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

#ifndef CHPL_UAST_VISIBILITYCLAUSE_H
#define CHPL_UAST_VISIBILITYCLAUSE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Decl.h"
#include "chpl/uast/AstNode.h"

namespace chpl {
namespace uast {


/**
  This class represents a visibility clause. Visibility clauses make up the
  contents of Use and Import statements. For example:

  \rst
  .. code-block:: chapel

    // This contains two visibility clauses, 'Foo as X' and 'Baz as Y'.
    use Foo as X, Baz as Y;

    // This contains one visibility clause, 'Baz as Z'.
    import Baz as Z;

  \endrst
 */
class VisibilityClause final : public AstNode {
 friend class AstNode;

 public:

  /**
    These values represent the kind of limitations possessed by a visibility
    clause. Visibility clauses within use statements may have limitations of
    kind 'EXCEPT', 'ONLY', or 'NONE'. Those within import statements may have
    have limitations of kind 'BRACES' or 'NONE'.

    If the limitation kind is 'NONE', then the visibility clause should have
    no limitations. If the kind is 'BRACES' or 'EXCEPT', then the visibility
    clause should have one or more limitations. If the kind is 'ONLY', then
    it may have zero or more limitations.
  */
  enum LimitationKind {
    BRACES,
    EXCEPT,
    ONLY,
    NONE
  };

 private:
  // These always exist and their position never changes.
  static const int8_t symbolChildNum_ = 0;
  static const int8_t limitationChildNum_ = 1;

  LimitationKind limitationKind_;
  int numLimitations_;

  VisibilityClause(AstList children,  LimitationKind limitationKind,
                   int numLimitations)
    : AstNode(asttags::VisibilityClause, std::move(children)),
      limitationKind_(limitationKind),
      numLimitations_(numLimitations) {

    switch (limitationKind_) {
      case BRACES:
      case EXCEPT: CHPL_ASSERT(numLimitations >= 1); break;
      case ONLY: CHPL_ASSERT(numLimitations_ >= 0); break;
      case NONE: CHPL_ASSERT(numLimitations_ == 0); break;
    }
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(limitationKind_);
    ser.writeVInt(numLimitations_);
  }

  explicit VisibilityClause(Deserializer& des)
    : AstNode(asttags::VisibilityClause, des) {
    limitationKind_ = des.read<LimitationKind>();
    numLimitations_ = des.readVInt();
  }

  // No need to check 'symbolChildNum_' or 'limitationChildNum_'.
  bool contentsMatchInner(const AstNode* other) const override {
    const VisibilityClause* rhs = other->toVisibilityClause();
    return this->limitationKind_ == rhs->limitationKind_ &&
      this->numLimitations_ == rhs->numLimitations_;
  }

  void markUniqueStringsInner(Context* context) const override {
  }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~VisibilityClause() override = default;

  /**
    Create and return a visibility clause.
  */
  static owned<VisibilityClause> build(Builder* builder, Location loc,
                                       owned<AstNode> symbol);

  /**
    Create and return a visibility clause.
  */
  static owned<VisibilityClause> build(Builder* builder, Location loc,
                                       owned<AstNode> symbol,
                                       LimitationKind limitationKind,
                                       AstList limitations);

  /**
    Get the symbol of this visibility clause. It may be a Dot, As,
    or Identifier.
  */
  const AstNode* symbol() const {
    auto ret = child(symbolChildNum_);
    CHPL_ASSERT(ret->isDot() || ret->isAs() || ret->isIdentifier());
    return ret;
  }

  /**
    Return the kind of limitations contained by this visibility clause.
  */
  LimitationKind limitationKind() const {
    return limitationKind_;
  }

  /**
    Return a way to iterate over the limitations of this visibility clause.
  */
  AstListIteratorPair<AstNode> limitations() const {
    auto begin = (numLimitations() > 0)
        ? children_.begin() + limitationChildNum_
        : children_.end();
    auto end = begin + numLimitations_;
    return AstListIteratorPair<AstNode>(begin, end);
  }

  /**
    Return the number of limitations contained in this visibility clause.
  */
  int numLimitations() const {
    return this->numLimitations_;
  }

  /**
    Return the i'th limitation of this visibility clause. If the limitation
    kind is 'EXCEPT', then the limitations will all be Identifier. If the
    limitation kind is 'ONLY' or 'BRACES', then the limitations may be
    Identifier or As expressions.
  */
  const AstNode* limitation(int i) const {
    CHPL_ASSERT(i >= 0 && i < numLimitations_);
    const AstNode* ast = this->child(limitationChildNum_+i);
    return ast;
  }

  /**
    Return a string describing the passed LimitationKind.
   */
  static const char* limitationKindToString(LimitationKind kind);
};


} // end namespace uast

/// \cond DO_NOT_DOCUMENT

template <>
struct stringify<uast::VisibilityClause::LimitationKind> {
  void operator()(std::ostream& streamOut, StringifyKind stringKind,
                  uast::VisibilityClause::LimitationKind limitationKind) const {
    switch (limitationKind) {
      case uast::VisibilityClause::LimitationKind::BRACES:
        streamOut << "braces";
        break;
      case uast::VisibilityClause::LimitationKind::EXCEPT:
        streamOut << "except";
        break;
      case uast::VisibilityClause::LimitationKind::ONLY:
        streamOut << "only";
        break;
      case uast::VisibilityClause::LimitationKind::NONE:
        streamOut << "none";
        break;
      default:
        CHPL_ASSERT(false && "should not reach this point");
    }
  }
};

template <>
struct mark<uast::VisibilityClause::LimitationKind> {
  void operator()(Context* context, uast::VisibilityClause::LimitationKind t) {
    // No need to mark enums
  }
};

DECLARE_SERDE_ENUM(uast::VisibilityClause::LimitationKind, uint8_t);

/// \endcond

} // end namespace chpl


namespace std {

/// \cond DO_NOT_DOCUMENT

template <>
struct hash<chpl::uast::VisibilityClause::LimitationKind> {
  size_t operator()(
      const chpl::uast::VisibilityClause::LimitationKind& key) const {
    return (size_t)key;
  }
};

/// \endcond

} // end namespace std

#endif
