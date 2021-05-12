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

// include these first since they set some things that
// are used by system headers.
#include "../util/sys_basic.h"
#include "../util/bswap.h"

#include "chpl/queries/UniqueString.h"

#include "chpl/queries/Context.h"

#include <cassert>
#include <cstring>

namespace chpl {
namespace detail {


char* InlinedString::dataAssumingTag(void* vptr) {
  char* ptr = (char*) vptr;
  // assuming the tag is present, where is the string data?
  // on a little endian system, need to pass the tag.
  // on big endian systems, the tag is after the null terminator,
  // so no action is necessary.
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    ptr += 1; // pass the tag
  #endif
  return ptr;
}


InlinedString InlinedString::buildUsingContextTable(Context* context,
                                                    const char* s, int len) {
  const char* u = context->uniqueCString(s);
  // assert that the address returned is even
  assert( (((uintptr_t)u)&1)==0 );
  return InlinedString::buildFromAligned(u, len);
}


} // end namespace detail


static bool stringContainsZeroBytes(const char* s, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (s[i] == '\0')
      return true;
  }
  return false;
}

UniqueString UniqueString::build(Context* context,
                                 const char* s, size_t len) {
  const char* u;
  assert(!stringContainsZeroBytes(s, len));
  if (s[len] == '\0') {
    // string is already appropriately null terminated
    detail::PODUniqueString ret =
      detail::PODUniqueString::build(context, s, len);
    return UniqueString(ret);
  } else {
    // otherwise, construct a truncated string
    std::string str(s, len);
    detail::PODUniqueString ret =
      detail::PODUniqueString::build(context, str.c_str(), len);
    return UniqueString(ret);
  }
}

void UniqueString::mark(Context* context) const {
  if (this->s.i.isInline()) {
    // nothing to do since string data is stored inline, not in map
  } else {
    context->markUniqueCString(this->c_str());
  }
}


} // end namespace chpl
