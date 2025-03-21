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

#ifndef CHPL_UAST_FUNCTION_SIGNATURE_H
#define CHPL_UAST_FUNCTION_SIGNATURE_H

#include "chpl/framework/Location.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Function.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Qualifier.h"

namespace chpl {
namespace uast {


/**
  This class represents a function signature. For example:

  \rst
  .. code-block:: chapel

    type T = proc(int, int): int;

  \endrst

  The type alias 'T' stores the function signature for a procedure
  that takes two ints and returns an int.
*/
class FunctionSignature final : public AstNode {
 friend class AstNode;

 public:
  using ReturnIntent = Function::ReturnIntent;
  using Kind = Function::Kind;

 private:
  Kind kind_;
  ReturnIntent returnIntent_;
  int formalsChildNum_;
  int thisFormalChildNum_;
  int numFormals_; // Count includes the 'this' formal.
  int returnTypeChildNum_;
  bool throws_;
  bool isParenless_;

  FunctionSignature(AstList children, Kind kind,
                    ReturnIntent returnIntent,
                    bool throws,
                    bool isParenless,
                    int formalsChildNum,
                    int thisFormalChildNum,
                    int numFormals,
                    int returnTypeChildNum) :
      AstNode(asttags::FunctionSignature, std::move(children)),
      kind_(kind),
      returnIntent_(returnIntent),
      formalsChildNum_(formalsChildNum),
      thisFormalChildNum_(thisFormalChildNum),
      numFormals_(numFormals),
      returnTypeChildNum_(returnTypeChildNum),
      throws_(throws),
      isParenless_(isParenless) {

    CHPL_ASSERT(NO_CHILD <= formalsChildNum_ &&
                 formalsChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= thisFormalChildNum_ &&
                 thisFormalChildNum_ < (ssize_t)children_.size());
    CHPL_ASSERT(0 <= numFormals_ &&
                numFormals_ <= (ssize_t)children_.size());
    CHPL_ASSERT(NO_CHILD <= returnTypeChildNum_ &&
                 returnTypeChildNum_ < (ssize_t)children_.size());
  }

  void serializeInner(Serializer& ser) const override {
    ser.write(kind_);
    ser.write(returnIntent_);
    ser.writeVInt(formalsChildNum_);
    ser.writeVInt(thisFormalChildNum_);
    ser.writeVInt(numFormals_);
    ser.writeVInt(returnTypeChildNum_);
    ser.write(throws_);
    ser.write(isParenless_);
  }

  explicit FunctionSignature(Deserializer& des)
    : AstNode(asttags::FunctionSignature, des) {
      kind_ = des.read<Kind>();
      returnIntent_ = des.read<ReturnIntent>();
      formalsChildNum_ = des.readVInt();
      thisFormalChildNum_ = des.readVInt();
      numFormals_= des.readVInt();
      returnTypeChildNum_ = des.readVInt();
      throws_ = des.read<bool>();
      isParenless_ = des.read<bool>();
    }

  bool contentsMatchInner(const AstNode* other) const override {
    auto lhs = this;
    auto rhs = (const FunctionSignature*) other;
    return lhs->kind_ == rhs->kind_ &&
           lhs->returnIntent_ == rhs->returnIntent_ &&
           lhs->throws_ == rhs->throws_ &&
           lhs->isParenless_ == rhs->isParenless_ &&
           lhs->formalsChildNum_ == rhs->formalsChildNum_ &&
           lhs->thisFormalChildNum_ == rhs->thisFormalChildNum_ &&
           lhs->numFormals_ == rhs->numFormals_ &&
           lhs->returnTypeChildNum_ == rhs->returnTypeChildNum_;
  }

  void markUniqueStringsInner(Context* context) const override { }

  void dumpFieldsInner(const DumpSettings& s) const override;
  std::string dumpChildLabelInner(int i) const override;

 public:
  ~FunctionSignature() override = default;

  static owned<FunctionSignature> build(Builder* builder, Location loc,
                                        Kind kind,
                                        owned<Formal> receiver,
                                        ReturnIntent returnIntent,
                                        bool parenless,
                                        AstList formals,
                                        owned<AstNode> returnType,
                                        bool throws);

  inline Kind kind() const { return this->kind_; }
  inline ReturnIntent returnIntent() const { return this->returnIntent_; }
  inline bool throws() const { return this->throws_; }
  inline bool isParenless() const { return isParenless_; }

  /**
    Return a way to iterate over the formals, including the method
    receiver, if present, as the first formal.
  */
  AstListIteratorPair<Decl> formals() const {
    return childRange<Decl>(formalsChildNum_, numFormals_);
  }

  /**
    Return the number of Formals
  */
  inline int numFormals() const { return numFormals_; }

  /**
    Return the i'th formal
  */
  const Decl* formal(int i) const {
    CHPL_ASSERT(0 <= i && i < numFormals_);
    auto ret = this->child(formalsChildNum_ + i);
    return (const Decl*) ret;
  }

  /**
    Returns the Formal for the 'this' formal argument,
    or 'nullptr' if there is none.
  */
  const Formal* thisFormal() const {
    if (thisFormalChildNum_ == NO_CHILD) return nullptr;
    auto ret = this->child(thisFormalChildNum_);
    CHPL_ASSERT(ret->isFormal());
    return (const Formal*) ret;
  }

  /**
    Returns the expression for the return type or nullptr if there was none.
  */
  const AstNode* returnType() const {
    if (returnTypeChildNum_ == NO_CHILD) return nullptr;
    const AstNode* ret = this->child(returnTypeChildNum_);
    return ret;
  }
};


} // end namespace uast
} // end namespace chpl

#endif
