/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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

// this is a convenient wrapper around type-clases-list.h. See that file
// for documentation on how to use it. This file simplifies using the
// header by auto-defining undefined X-macros to sensible defaults, and
// by clearing the macro definitions at the end of the file.


#ifndef TYPE_NODE
#define TYPE_NODE(NAME)
#endif

#ifndef BUILTIN_TYPE_NODE
#define BUILTIN_TYPE_NODE(NAME, CHAPEL_NAME) TYPE_NODE(NAME)
#endif

#ifndef TYPE_BEGIN_SUBCLASSES
#define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_NODE(NAME)
#endif

#ifndef TYPE_END_SUBCLASSES
#define TYPE_END_SUBCLASSES(NAME)
#endif

#include "type-classes-list.h"

#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
