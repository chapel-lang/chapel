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

#include "chpl/queries/query-impl.h"
#include "chpl/types/BoolType.h"
#include "chpl/types/BytesType.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/CStringType.h"
#include "chpl/types/ImagType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/Param.h"
#include "chpl/types/RealType.h"
#include "chpl/types/StringType.h"
#include "chpl/types/UintType.h"

#include "../immediates/num.h"
#include "../immediates/prim_data.h"

namespace chpl {
namespace types {


Param::Param()
{
  // clear the union members
  memset(&u, 0, sizeof(u));
}

bool Param::equals(const Param& a, const Param& b) {
  if (a.tag_ != b.tag_)
    return false;

  if (a.tag_ == paramtags::None)
    return true;

  // otherwise, tags are equal and have value, so check value
  switch (a.tag_) {
    // define the cases using macros and ParamClassesList.h
    /// \cond DO_NOT_DOCUMENT
    #define PARAM_NODE(NAME, TYPE) \
    case paramtags::NAME: \
        return a.u.NAME == b.u.NAME;
    /// \endcond
    // Apply the above macros to ParamClassesList.h
    #include "chpl/types/ParamClassesList.h"
    // clear the macros
    #undef PARAM_NODE
  }
  assert(false && "should not be reachable");
  return true;
}

void Param::mark(Context* context) const {
  switch (tag_) {
    case paramtags::Bytes:   u.Bytes.mark(context);   break;
    case paramtags::CString: u.CString.mark(context); break;
    case paramtags::String:  u.String.mark(context);  break;
    default:
      ; // do nothing
  }
}

const owned<Param>& Param::getIntQuery(Context* context,
                                       int64_t v,
                                       int bitwidth) {
  QUERY_BEGIN(getIntQuery, context, v, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = IntType::get(context, bitwidth);

  switch (bitwidth) {
    case 64:
      result->tag_ = paramtags::Int64;
      result->u.Int64 = v;
      break;
    case 32:
      result->tag_ = paramtags::Int32;
      result->u.Int32 = v;
      break;
    case 16:
      result->tag_ = paramtags::Int16;
      result->u.Int16 = v;
      break;
    case 8:
      result->tag_ = paramtags::Int8;
      result->u.Int8 = v;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getInt(Context* context, int64_t v, int bitwidth) {
  if (bitwidth == 0) bitwidth = 64; // canonicalize default width
  return getIntQuery(context, v, bitwidth).get();
}

const owned<Param>& Param::getUintQuery(Context* context,
                                        uint64_t v,
                                        int bitwidth) {
  QUERY_BEGIN(getUintQuery, context, v, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = UintType::get(context, bitwidth);

  switch (bitwidth) {
    case 64:
      result->tag_ = paramtags::Uint64;
      result->u.Uint64 = v;
      break;
    case 32:
      result->tag_ = paramtags::Uint32;
      result->u.Uint32 = v;
      break;
    case 16:
      result->tag_ = paramtags::Uint16;
      result->u.Uint16 = v;
      break;
    case 8:
      result->tag_ = paramtags::Uint8;
      result->u.Uint8 = v;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getUint(Context* context, uint64_t v, int bitwidth) {
  if (bitwidth == 0) bitwidth = 64; // canonicalize default width
  return getUintQuery(context, v, bitwidth).get();
}

const owned<Param>& Param::getBoolQuery(Context* context,
                                        uint64_t v,
                                        int bitwidth) {
  QUERY_BEGIN(getBoolQuery, context, v, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = BoolType::get(context, bitwidth);

  switch (bitwidth) {
    case 0:
      result->tag_ = paramtags::BoolSys;
      result->u.BoolSys = v;
      break;
    case 64:
      result->tag_ = paramtags::Bool64;
      result->u.Bool64 = v;
      break;
    case 32:
      result->tag_ = paramtags::Bool32;
      result->u.Bool32 = v;
      break;
    case 16:
      result->tag_ = paramtags::Bool16;
      result->u.Bool16 = v;
      break;
    case 8:
      result->tag_ = paramtags::Bool8;
      result->u.Bool8 = v;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getBool(Context* context, uint64_t v, int bitwidth) {
  // bool width of 0 is the system bool
  return getBoolQuery(context, v, bitwidth).get();
}

const owned<Param>& Param::getRealQuery(Context* context,
                                        double v,
                                        int bitwidth) {
  QUERY_BEGIN(getRealQuery, context, v, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = RealType::get(context, bitwidth);

  switch (bitwidth) {
    case 64:
      result->tag_ = paramtags::Real64;
      result->u.Real64 = v;
      break;
    case 32:
      result->tag_ = paramtags::Real32;
      result->u.Real32 = v;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getReal(Context* context, double v, int bitwidth) {
  if (bitwidth == 0) bitwidth = 64; // canonicalize default width
  return getRealQuery(context, v, bitwidth).get();
}

const owned<Param>& Param::getImagQuery(Context* context,
                                        double v,
                                        int bitwidth) {
  QUERY_BEGIN(getImagQuery, context, v, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = ImagType::get(context, bitwidth);

  switch (bitwidth) {
    case 64:
      result->tag_ = paramtags::Imag64;
      result->u.Imag64 = v;
      break;
    case 32:
      result->tag_ = paramtags::Imag32;
      result->u.Imag32 = v;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getImag(Context* context, double v, int bitwidth) {
  if (bitwidth == 0) bitwidth = 64; // canonicalize default width
  return getImagQuery(context, v, bitwidth).get();
}

const owned<Param>& Param::getComplexQuery(Context* context,
                                           double re, double im,
                                           int bitwidth) {
  QUERY_BEGIN(getComplexQuery, context, re, im, bitwidth);

  auto result = toOwned(new Param());
  result->type_ = ComplexType::get(context, bitwidth);

  switch (bitwidth) {
    case 128:
      result->tag_ = paramtags::Complex128;
      result->u.Complex128.re = re;
      result->u.Complex128.im = im;
      break;
    case 64:
      result->tag_ = paramtags::Complex64;
      result->u.Complex64.re = re;
      result->u.Complex64.im = im;
      break;
    default:
      assert(false && "should not be reachable");
  }

  return QUERY_END(result);
}

const Param* Param::getComplex(Context* context,
                               double re, double im, int bitwidth) {
  if (bitwidth == 0) bitwidth = 128; // canonicalize default width
  return getComplexQuery(context, re, im, bitwidth).get();
}

const owned<Param>& Param::getStringQuery(Context* context,
                                          chpl::detail::PODUniqueString str) {
  QUERY_BEGIN(getStringQuery, context, str);

  auto result = toOwned(new Param());
  result->type_ = StringType::get(context);
  result->tag_ = paramtags::String;
  result->u.String = str;

  return QUERY_END(result);
}

const Param* Param::getString(Context* context, const char* str, size_t len) {
  auto s = chpl::detail::PODUniqueString::build(context, str, len);
  return getStringQuery(context, s).get();
}

const owned<Param>& Param::getBytesQuery(Context* context,
                                         chpl::detail::PODUniqueString str) {
  QUERY_BEGIN(getBytesQuery, context, str);

  auto result = toOwned(new Param());
  result->type_ = BytesType::get(context);
  result->tag_ = paramtags::Bytes;
  result->u.Bytes = str;

  return QUERY_END(result);
}

const Param* Param::getBytes(Context* context, const char* str, size_t len) {
  auto s = chpl::detail::PODUniqueString::build(context, str, len);
  return getBytesQuery(context, s).get();
}

const owned<Param>& Param::getCStringQuery(Context* context,
                                           chpl::detail::PODUniqueString str) {
  QUERY_BEGIN(getCStringQuery, context, str);

  auto result = toOwned(new Param());
  result->type_ = CStringType::get(context);
  result->tag_ = paramtags::CString;
  result->u.CString = str;

  return QUERY_END(result);
}

const Param* Param::getCString(Context* context, const char* str, size_t len) {
  auto s = chpl::detail::PODUniqueString::build(context, str, len);
  return getCStringQuery(context, s).get();
}



} // end namespace types
} // end namespace chpl
