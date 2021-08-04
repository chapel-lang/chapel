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
#include "chpl/types/BoolType.h"
#include "chpl/types/BytesType.h"
#include "chpl/types/CStringType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/NothingType.h"
#include "chpl/types/ParamClasses.h"
#include "chpl/types/ParamTag.h"
#include "chpl/types/RealType.h"
#include "chpl/types/StringType.h"
#include "chpl/types/UintType.h"
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
  struct Complex128Storage {
    double re;
    double im;
    bool operator==(const Complex128Storage& other) const {
      return this->re == other.re && this->im == other.im;
    }
    bool operator!=(const Complex128Storage& other) const {
      return !(*this == other);
    }
    size_t hash() const {
      return chpl::hash(re, im);
    }
  };
  struct Complex64Storage {
    float re;
    float im;
    bool operator==(const Complex64Storage& other) const {
      return this->re == other.re && this->im == other.im;
    }
    bool operator!=(const Complex64Storage& other) const {
      return !(*this == other);
    }
    size_t hash() const {
      return chpl::hash(re, im);
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

  /**
    This function needs to be defined by subclasses. It should
    return the Type of the Param.
   */
  virtual const Type* getTypeInner(Context* context) const = 0;

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
  static std::string valueToString(Complex128Storage v) {
    return std::to_string(v.re) + "+" + std::to_string(v.im) + "i";
  }
  static std::string valueToString(Complex64Storage v) {
    return std::to_string(v.re) + "+" + std::to_string(v.im) + "i";
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

  /**
    Returns the type of the Param.
   */
  const Type* getType(Context* context) const {
    return getTypeInner(context);
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

  std::string toString() const;

  // define is__ methods for the various Param subclasses
  // using macros and ParamClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define PARAM_IS(NAME) \
    bool is##NAME() const { \
      return paramtags::is##NAME(this->tag_); \
    }
  #define PARAM_NODE(NAME, VALTYPE, TYPEEXPR) PARAM_IS(NAME)
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
  #define PARAM_NODE(NAME, VALTYPE, TYPEEXPR) PARAM_TO(NAME)
  /// \endcond
  // Apply the above macros to ParamClassesList.h
  #include "chpl/types/ParamClassesList.h"
  // clear the macros
  #undef PARAM_NODE
  #undef PARAM_TO
};

// define the subclasses using macros and ParamClassesList.h
/// \cond DO_NOT_DOCUMENT
#define PARAM_NODE(NAME, VALTYPE, TYPEEXPR) \
  class NAME : public Param { \
   private: \
    VALTYPE value_; \
    NAME(VALTYPE value) : Param(paramtags::NAME), value_(value) { } \
    static const owned<NAME>& get##NAME(Context* context, VALTYPE value); \
    bool contentsMatchInner(const Param* other) const override { \
      const NAME* lhs = this; \
      const NAME* rhs = (const NAME*) other; \
      return lhs->value_ == rhs->value_; \
    } \
    void markUniqueStringsInner(Context* context) const override { \
      Param::markValue(context, value_); \
    } \
    const Type* getTypeInner(Context* context) const override { \
      return TYPEEXPR; \
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
  template<> struct hash<chpl::types::Param::Complex128Storage> {
    size_t operator()(const chpl::types::Param::Complex128Storage key) const {
      return key.hash();
    }
  };
  template<> struct hash<chpl::types::Param::Complex64Storage> {
    size_t operator()(const chpl::types::Param::Complex64Storage key) const {
      return key.hash();
    }
  };
} // end namespace std

#endif
