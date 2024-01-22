/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef CHPL_TOOLS_CHPLDEF_COMPILER_GADGETS_H
#define CHPL_TOOLS_CHPLDEF_COMPILER_GADGETS_H

#include "protocol-types.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"

// This header contains 'compiler gadgets', functions manipulating the
// Chapel compiler that may be useful for multiple LSP computations.
// They come in two flavors:
//
//  -- Those that take 'Server*' as the first argument. Generally, this
//     implies that the gadget will log to the log file, as it will
//     implement a more substantial operation.
//  -- Those that take the 'chpl::Context' only. These are smaller gadgets
//     that do not deserve a line in the logger.
//
namespace chpldef {

using LineToIdsMap = std::map<int, std::vector<chpl::ID>>;

/** Parse a top level module given a path. */
const chpl::uast::BuilderResult&
parseFromUri(chpl::Context* chapel, chpl::UniqueString uri);

/** Parse a top level module given a path. */
const chpl::uast::BuilderResult&
parseFromUri(chpl::Context* chapel, const std::string& uri);

/** A compiler query which maps source lines to AST nodes on that line. */
const LineToIdsMap&
mapLinesToIdsInModule(chpl::Context* chapel, chpl::UniqueString uri);

/** Given a cursor position and URI, create a Chapel location. */
chpl::Location
locationFromUriAndPosition(chpl::Context* chapel, const std::string& uri,
                           const Position& pos);

/** If 'ast' is the base expression of a call, return the parent call. */
const chpl::uast::FnCall*
parentCallIfBaseExpression(chpl::Context* chapel,
                           const chpl::uast::AstNode* ast);

} // end namespace 'chpldef'

#endif
