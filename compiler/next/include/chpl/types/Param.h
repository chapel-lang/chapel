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

#include "chpl/queries/UniqueString.h"
#include "chpl/types/ParamTag.h"

#include <cstddef>
#include <cstdint>

namespace chpl {
class Context;

namespace types {
class Type;
class QualifiedType;
class Immediate;

/**
  This type represents a param value.
 */
class Param {
 private:
  struct Complex128Storage {
    double re;
    double im;
    bool operator==(const Complex128Storage& other) const {
      return this->re == other.re && this->im == other.im;
    }
    bool operator!=(const Complex128Storage& other) const {
      return !(*this == other);
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
  };
  struct UniqueStringAndLength {
    chpl::detail::PODUniqueString str;
    size_t len;
    bool operator==(const UniqueStringAndLength& other) const {
      return this->str == other.str && this->len == other.len;
    }
    bool operator!=(const UniqueStringAndLength& other) const {
      return !(*this == other);
    }
    static UniqueStringAndLength build(chpl::detail::PODUniqueString str, size_t len) {
      UniqueStringAndLength ret = {str, len};
      return ret;
    }
  };

  ParamTag tag_ = paramtags::None;
  union {
    // define the enum using macros and ParamClassesList.h
    /// \cond DO_NOT_DOCUMENT
    #define PARAM_NODE(NAME, TYPE) TYPE NAME;
    /// \endcond
    // Apply the above macros to ParamClassesList.h
    #include "chpl/types/ParamClassesList.h"
    // clear the macros
    #undef PARAM_NODE
  } u;
  const Type* type_ = nullptr;

  Param();

  static const owned<Param>& getIntQuery(Context* context, int64_t v, int bitwidth);
  static const owned<Param>& getUintQuery(Context* context, uint64_t v, int bitwidth);
  static const owned<Param>& getBoolQuery(Context* context, uint64_t v, int bitwidth);
  static const owned<Param>& getRealQuery(Context* context, double v, int bitwidth);
  static const owned<Param>& getImagQuery(Context* context, double v, int bitwidth);
  static const owned<Param>& getComplexQuery(Context* context, double re, double im, int bitwidth);
  static const owned<Param>& getStringQuery(Context* context, chpl::detail::PODUniqueString str, size_t len);
  static const owned<Param>& getBytesQuery(Context* context, chpl::detail::PODUniqueString str, size_t len);

 public:
  static const Param* getInt(Context* context, int64_t v, int bitwidth);
  static const Param* getUint(Context* context, uint64_t v, int bitwidth);
  static const Param* getBool(Context* context, uint64_t v, int bitwidth);
  static const Param* getReal(Context* context, double v, int bitwidth);
  static const Param* getImag(Context* context, double v, int bitwidth);
  static const Param* getComplex(Context* context, double re, double im, int bitwidth);
  static const Param* getString(Context* context, const char* str, size_t len);
  static const Param* getBytes(Context* context, const char* str, size_t len);

  ParamTag tag() const { return tag_; }
  const Type* type(Context* context) const { return type_; }

  static bool equals(const Param& a, const Param& b);

  bool operator==(const Param& other) const {
    return equals(*this, other);
  }
  bool operator!=(const Param& other) const {
    return !(*this == other);
  }


};


} // end namespace types
} // end namespace chpl

#endif
