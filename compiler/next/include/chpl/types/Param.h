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

#ifndef CHPL_TYPES_PARAM_H
#define CHPL_TYPES_PARAM_H

#include "chpl/queries/Context.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/types/ParamClasses.h"
#include "chpl/types/ParamTag.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/util/hash.h"

namespace chpl {
namespace types {


/**
  This is the base class for classes that represent a param value.

  Functions like someParam->isUint32Param() / someParam->toUint32Param() are
  available and generated for all Param subclasses.

 */
class Param {
 public:
  struct ComplexDouble {
    double re;
    double im;
    ComplexDouble(double re, double im)
      : re(re), im(im)
    { }
    bool operator==(const ComplexDouble& other) const {
      return this->re == other.re && this->im == other.im;
    }
    bool operator!=(const ComplexDouble& other) const {
      return !(*this == other);
    }
    size_t hash() const {
      return chpl::hash(re, im);
    }
  };
  struct NoneValue {
    bool operator==(const NoneValue& other) const {
      return true;
    }
    bool operator!=(const NoneValue& other) const {
      return false;
    }
    size_t hash() const {
      return 0;
    }
  };

 private:
  ParamTag tag_;

 protected:
  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the Param fields such as tag_).
    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const Param* other) const = 0;

  /**
   This function needs to be defined by subclasses.
   It should call the 'mark' method on any UniqueStrings
   stored as fields.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

  // helper functions to mark a value
  static void markValue(Context* context, UniqueString v) {
    v.mark(context);
  }
  // do nothing for non-UniqueString values
  template<typename T> static void markValue(Context* context, T v) { }

  // helper function to convert a value to a string
  static std::string valueToString(UniqueString v) {
    return v.toString();
  }
  static std::string valueToString(ComplexDouble v) {
    return std::to_string(v.re) + "+" + std::to_string(v.im) + "i";
  }
  static std::string valueToString(NoneValue v) {
    return "none";
  }
  template<typename T> static std::string valueToString(T v) {
    return std::to_string(v);
  }

  Param(ParamTag tag)
    : tag_(tag) {
  }

 public:
  virtual ~Param() = 0; // this is an abstract base class

  /**
    Returns the tag indicating which Param subclass this is.
   */
  ParamTag tag() const {
    return tag_;
  }

  bool operator==(const Param& other) const {
    return completeMatch(&other);
  }
  bool operator!=(const Param& other) const {
    return !(*this == other);
  }

  bool completeMatch(const Param* other) const;

  // 'keep' is some old Param
  // 'addin' is some new Param we wish to combine with it
  //
  // on exit, 'keep' stores the Param we need to keep, and anything
  // not kept is stored in 'addin'.
  // the function returns 'true' if anything changed in 'keep'.
  static bool updateParam(owned<Param>& keep, owned<Param>& addin);

  static void markParam(Context* context, const Param* keep);

  static QualifiedType fold(Context* context,
                            UniqueString op,
                            QualifiedType a,
                            QualifiedType b);

  std::string toString() const;

  // define is__ methods for the various Param subclasses
  // using macros and ParamClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define PARAM_IS(NAME) \
    bool is##NAME() const { \
      return paramtags::is##NAME(this->tag_); \
    }
  #define PARAM_NODE(NAME, VALTYPE) PARAM_IS(NAME)
  /// \endcond
  // Apply the above macros to ParamClassesList.h
  #include "chpl/types/ParamClassesList.h"
  // clear the macros
  #undef PARAM_NODE
  #undef PARAM_IS

  // define to__ methods for the various Param subclasses
  // using macros and ParamClassesList.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  /// \cond DO_NOT_DOCUMENT
  #define PARAM_TO(NAME) \
    const NAME * to##NAME() const { \
      return this->is##NAME() ? (const NAME *)this : nullptr; \
    } \
    NAME * to##NAME() { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define PARAM_NODE(NAME, VALTYPE) PARAM_TO(NAME)
  /// \endcond
  // Apply the above macros to ParamClassesList.h
  #include "chpl/types/ParamClassesList.h"
  // clear the macros
  #undef PARAM_NODE
  #undef PARAM_TO
};

// define the subclasses using macros and ParamClassesList.h
/// \cond DO_NOT_DOCUMENT
#define PARAM_NODE(NAME, VALTYPE) \
  class NAME : public Param { \
   private: \
    VALTYPE value_; \
    explicit NAME(VALTYPE value) : Param(paramtags::NAME), value_(value) { } \
    static const owned<NAME>& get##NAME(Context* context, VALTYPE value); \
    bool contentsMatchInner(const Param* other) const override { \
      const NAME* lhs = this; \
      const NAME* rhs = (const NAME*) other; \
      return lhs->value_ == rhs->value_; \
    } \
    void markUniqueStringsInner(Context* context) const override { \
      Param::markValue(context, value_); \
    } \
   public: \
    ~NAME() = default; \
    static const NAME* get(Context* context, VALTYPE value) { \
      return get##NAME(context, value).get(); \
    } \
    VALTYPE value() const { \
      return value_; \
    } \
  };
/// \endcond

// Apply the above macros to ParamClassesList.h
#include "chpl/types/ParamClassesList.h"

// clear the macros
#undef PARAM_NODE


} // end namespace types
} // end namespace chpl

// TODO: is there a reasonable way to define std::less on Param*?
// Comparing pointers would lead to some nondeterministic ordering.
namespace std {
  template<> struct hash<chpl::types::Param::ComplexDouble> {
    size_t operator()(const chpl::types::Param::ComplexDouble key) const {
      return key.hash();
    }
  };
  template<> struct hash<chpl::types::Param::NoneValue> {
    size_t operator()(const chpl::types::Param::NoneValue key) const {
      return key.hash();
    }
  };
} // end namespace std

#endif
