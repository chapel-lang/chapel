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

// This is included at the right time in the generated bison .h/.cpp
// It is split out into this separate file for easier maintenance.
// It contains any #includes necessary for the parser.

#include "chpl/ast/ASTBase.h"
#include "chpl/ast/Block.h"
#include "chpl/ast/Builder.h"
#include "chpl/ast/Comment.h"
#include "chpl/ast/Decl.h"
#include "chpl/ast/ErroneousExp.h"
#include "chpl/ast/Exp.h"
#include "chpl/ast/Identifier.h"
#include "chpl/ast/Location.h"
#include "chpl/ast/Module.h"
#include "chpl/ast/ModuleDecl.h"
#include "chpl/ast/UniqueString.h"
#include "chpl/ast/Sym.h"
#include "chpl/ast/Variable.h"
#include "chpl/ast/VariableDecl.h"
#include "chpl/queries/Context.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace chpl;
using namespace uast;
using chpl::uast::detail::PODUniqueString;

struct ParserError;
struct ParserComment;
