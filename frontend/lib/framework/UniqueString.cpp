/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/UniqueString.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/update-functions.h"
#include "chpl/util/string-escapes.h"

#include <cstring>

namespace chpl {
namespace detail {


InlinedString InlinedString::buildUsingContextTable(Context* context,
                                                    const char* s,
                                                    size_t len) {
  const char* u = context->uniqueCString(s, len);
  // assert that the address returned is even
  CHPL_ASSERT( (((uintptr_t)u)&1)==0 );
  return InlinedString::buildFromAlignedPtr(u, len);
}

InlinedString InlinedString::getConcat(Context* context,
                                       const char* s1, const char* s2,
                                       const char* s3,
                                       const char* s4,
                                       const char* s5,
                                       const char* s6,
                                       const char* s7,
                                       const char* s8,
                                       const char* s9) {
  size_t len, len1, len2, len3, len4, len5, len6, len7, len8, len9;
  len = len1 = len2 = len3 = len4 = len5 = len6 = len7 = len8 = len9 = 0;

  if (s1)
    len1 = strlen(s1);
  if (s2)
    len2 = strlen(s2);
  if (s3)
    len3 = strlen(s3);
  if (s4)
    len4 = strlen(s4);
  if (s5)
    len5 = strlen(s5);
  if (s6)
    len6 = strlen(s6);
  if (s7)
    len7 = strlen(s7);
  if (s8)
    len8 = strlen(s8);
  if (s9)
    len9 = strlen(s9);

  len = len1 + len2 + len3 + len4 + len5 + len6 + len7 + len8 + len9;

  if (len <= MAX_SIZE_INLINED) {
    // Copy to a stack-local buffer and create the inlined string
    char s[MAX_SIZE_INLINED+1];
    len = 0;

    // copy each non-empty string
    if (len1 > 0) {
      memcpy(&s[len], s1, len1);
      len += len1;
    }
    if (len2 > 0) {
      memcpy(&s[len], s2, len2);
      len += len2;
    }
    if (len3 > 0) {
      memcpy(&s[len], s3, len3);
      len += len3;
    }
    if (len4 > 0) {
      memcpy(&s[len], s4, len4);
      len += len4;
    }
    if (len5 > 0) {
      memcpy(&s[len], s5, len5);
      len += len5;
    }
    if (len6 > 0) {
      memcpy(&s[len], s6, len6);
      len += len6;
    }
    if (len7 > 0) {
      memcpy(&s[len], s7, len7);
      len += len7;
    }
    if (len8 > 0) {
      memcpy(&s[len], s8, len8);
      len += len8;
    }
    if (len9 > 0) {
      memcpy(&s[len], s9, len9);
      len += len9;
    }

    // add the null terminator
    s[len] = '\0';

    return buildInlined(s, len);

  } else {
    const char* u = context->uniqueCStringConcatLen(s1, len1, s2, len2,
                                                    s3, len3, s4, len4,
                                                    s5, len5, s6, len6,
                                                    s7, len7, s8, len8,
                                                    s9, len9);
    // assert that the address returned is even
    CHPL_ASSERT( (((uintptr_t)u)&1)==0 );
    return InlinedString::buildFromAlignedPtr(u, len);
  }
}

size_t InlinedString::length() const {
  const char* cs = c_str();
  if (this->isInline()) {
    // inlined strings never contain inner nulls
    return strlen(cs);
  }

  return Context::lengthForUniqueString(cs);
}

const char* InlinedString::astr(Context* context) const {
  if (isInline()) {
    // inlined strings never contain inner nulls, so this is OK
    return context->uniqueCString(c_str());
  } else {
    return c_str();
  }
}

void InlinedString::mark(Context* context) const {
  // mark union fields as used for linter
  (void)v;
  (void)data;
  if (isInline()) {
    // nothing to do since string data is stored inline, not in map
  } else {
    context->markUniqueCString(c_str());
  }
}


} // end namespace detail


UniqueString UniqueString::get(Context* context,
                                 const char* s, size_t len) {
  if (s == nullptr || len == 0) return UniqueString();

  if (s[len] == '\0') {
    // string is already appropriately null terminated
    detail::PODUniqueString ret =
      detail::PODUniqueString::get(context, s, len);
    return UniqueString(ret);
  } else {
    // otherwise, construct a truncated string
    std::string str(s, len);
    detail::PODUniqueString ret =
      detail::PODUniqueString::get(context, str.c_str(), len);
    return UniqueString(ret);
  }
}

bool UniqueString::update(UniqueString& keep, UniqueString& addin) {
  return defaultUpdate(keep, addin);
}


void UniqueString::stringify(std::ostream& ss,
                             chpl::StringifyKind stringKind) const {
  ss.write(c_str(),length());
}
IMPLEMENT_DUMP(UniqueString);

std::ostream& operator<<(std::ostream& os, const chpl::UniqueString& uStr) {
  // write obj to stream
  os << uStr.c_str();
  return os;
}

//
// Represent UniqueString occurrences as integer IDs in a table that the
// Serializer builds up while serializing various entities. This reduces
// the file size of generated serialized AST and helps with performance.
//
void UniqueString::serialize(Serializer& ser) const {
  auto id = ser.cacheString(c_str(), length());
  ser.write(id);
}

//
// Deserialize a UniqueString by reading the integer ID and fetching the
// relevant pointer and length from the Deserializer.
//
UniqueString UniqueString::deserialize(Deserializer& des) {
  int uid = des.read<int>();
  const auto& strlen = des.getString(uid);
  return get(des.context(), strlen.second, strlen.first);
}

} // end namespace chpl
