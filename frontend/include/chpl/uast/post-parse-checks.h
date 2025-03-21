/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UAST_POST_PARSE_CHECKS_H
#define CHPL_UAST_POST_PARSE_CHECKS_H

#include "chpl/framework/Context.h"
#include "chpl/uast/BuilderResult.h"

namespace chpl {
namespace uast {

/**
  Runs post-parse checks on the given given builder result, constructed from
  the contents of the file at path. This is not itself a query, and thus
  errors are reported to the calling query.
  */
void
checkBuilderResult(Context* context, UniqueString path,
                   const BuilderResult& result);

} // end namespace uast
} // end namespace chpl

#endif
