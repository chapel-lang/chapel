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

#include "chpl/AST/ASTBase.h"
#include "chpl/AST/Block.h"
#include "chpl/AST/Builder.h"
#include "chpl/AST/Comment.h"
#include "chpl/AST/Decl.h"
#include "chpl/AST/ErroneousExp.h"
#include "chpl/AST/Exp.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/Location.h"
#include "chpl/AST/Module.h"
#include "chpl/AST/ModuleDecl.h"
#include "chpl/AST/UniqueString.h"
#include "chpl/AST/Sym.h"
#include "chpl/AST/Variable.h"
#include "chpl/AST/VariableDecl.h"
#include "chpl/Queries/Context.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace chpl;
using namespace ast;
using chpl::ast::detail::PODUniqueString;

struct ParserError;
struct ParserComment;
