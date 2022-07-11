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
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

namespace impl {

using namespace chpl;
using namespace resolution;
using namespace types;
using namespace uast;

// Get the top-level module resulting from parsing the given string.
const Module* parseModule(Context* context, const char* src);
const Module* parseModule(Context* context, std::string& str);

// assumes the last statement is a variable declaration for x
// with an initialization expression.
// Returns the type of the initializer expression.
QualifiedType
parseTypeOfXInit(Context* context, const char* program, bool requireTypeKnown = true);

} // end namespace impl

using namespace impl;

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif
