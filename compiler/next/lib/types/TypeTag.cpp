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

#include "chpl/types/TypeTag.h"

namespace chpl {
namespace types {
namespace typetags {


static const char* tagToStringTable[NUM_TYPE_TAGS] = {
// define tag to string conversion
#define NAMESTR(NAME) \
  #NAME,
#define TYPE_NODE(NAME) NAMESTR(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME) NAMESTR(START_##NAME)
#define TYPE_END_SUBCLASSES(NAME) NAMESTR(END_##NAME)
// Apply the above macros to TypeClassesList.h
#include "chpl/types/TypeClassesList.h"
// clear the macros
#undef TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef NAMESTR
};

const char* tagToString(TypeTag tag) {
  if (0 <= tag && tag < NUM_TYPE_TAGS)
    return tagToStringTable[tag];
  else
    return "<unknown-tag>";
}


} // end namespace typetags
} // end namespace types
} // end namespace chpl
