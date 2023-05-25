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

#ifndef TEST_COMMON_H
#define TEST_COMMON_H

// always check assertions in the tests
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "chpl/parsing/Parser.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/AstNode.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Module.h"
#include <cassert>
#include "./ErrorGuard.h"

/** Use the name of the enclosing CPP function as the name for a Chapel
    source file. Returns a UniqueString. */
#define TEST_NAME_FROM_FN_NAME(context__) \
  chpl::UniqueString::getConcat(context__, __FUNCTION__, ".chpl")

/** Parse to BuilderResult but report encountered errors to the context. */
const chpl::uast::BuilderResult&
parseAndReportErrors(chpl::Context* context, chpl::UniqueString path);

chpl::uast::BuilderResult
parseStringAndReportErrors(chpl::parsing::Parser* parser, const char* filename,
                           const char* content);

const chpl::uast::AstNode* findOnlyNamed(const chpl::uast::Module* mod, std::string name);

#endif
