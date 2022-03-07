/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
#include "chpl/types/ParamTag.h"
#include "chpl/types/QualifiedType.h"
#include "chpl/uast/PrimOp.h"
#include "chpl/util/hash.h"

namespace chpl {
namespace types {


// forward declare the various Param subclasses
// using macros and param-classes-list.h
/// \cond DO_NOT_DOCUMENT
#define PARAM_NODE(NAME, VALTYPE) class NAME;
/// \endcond
// Apply the above macros to param-classes-list.h
#include "chpl/types/param-classes-list.h"
// clear the macros
#undef PARAM_NODE

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
    return v.str();
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
    (void)tag_;  // quiet nextLinter
    return completeMatch(&other);
  }
  bool operator!=(const Param& other) const {
    return !(*this == other);
  }
  template<typename T>
  static bool update(owned<T>& keep, owned<T>& addin) {
    return defaultUpdateOwned(keep, addin);
  }
  void mark(Context* context) const {
    return markUniqueStringsInner(context);
  }

  bool completeMatch(const Param* other) const;

  static bool isParamOpFoldable(chpl::uast::PrimitiveTag op);

  static QualifiedType fold(Context* context,
                            chpl::uast::PrimitiveTag op,
                            QualifiedType a,
                            QualifiedType b);

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  static uint64_t binStr2uint64(const char* str, size_t len, std::string& err);
  static uint64_t octStr2uint64(const char* str, size_t len, std::string& err);
  static uint64_t decStr2uint64(const char* str, size_t len, std::string& err);
  static int64_t decStr2int64(const char* str, size_t len, std::string& err);
  static uint64_t hexStr2uint64(const char* str, size_t len, std::string& err);
  static double str2double(const char* str, size_t len, std::string& err);

  // define is__ methods for the various Param subclasses
  // using macros and param-classes-list.h
  /// \cond DO_NOT_DOCUMENT
  #define PARAM_IS(NAME) \
    bool is##NAME() const { \
      return paramtags::is##NAME(this->tag_); \
    }
  #define PARAM_NODE(NAME, VALTYPE) PARAM_IS(NAME)
  /// \endcond
  // Apply the above macros to param-classes-list.h
  #include "chpl/types/param-classes-list.h"
  // clear the macros
  #undef PARAM_NODE
  #undef PARAM_IS

  // define to__ methods for the various Param subclasses
  // using macros and param-classes-list.h
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
  // Apply the above macros to param-classes-list.h
  #include "chpl/types/param-classes-list.h"
  // clear the macros
  #undef PARAM_NODE
  #undef PARAM_TO

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT
};

// define the subclasses using macros and param-classes-list.h
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

// Apply the above macros to param-classes-list.h
#include "chpl/types/param-classes-list.h"

// clear the macros
#undef PARAM_NODE


} // end namespace types
/// \cond DO_NOT_DOCUMENT
template<> struct stringify<chpl::types::Param::ComplexDouble> {
  void operator()(std::ostream& streamOut,
                  chpl::StringifyKind stringKind,
                  const chpl::types::Param::ComplexDouble& stringMe) const {
    streamOut << "types::Param::ComplexDouble is not stringified";
  }
};

template<> struct stringify<chpl::types::Param::NoneValue> {
  void operator()(std::ostream& streamOut,
                  chpl::StringifyKind stringKind,
                  const chpl::types::Param::NoneValue& stringMe) const {
    streamOut << "types::Param::NoneValue is not stringified";
  }
};
/// \endcond DO_NOT_DOCUMENT
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
