/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#ifndef _CONVERT_UAST_H_
#define _CONVERT_UAST_H_

#include "alist.h"
#include "baseAST.h"
#include "ModuleSymbol.h"
#include "chpl/queries/Context.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Module.h"

// TODO: once chpldoc is implemented as a separate tool on uAST,
// remove the comment and builderResult arguments here.
ModuleSymbol*
convertToplevelModule(chpl::Context* context,
                      const chpl::uast::Module* mod,
                      ModTag modTag,
                      const chpl::uast::Comment* comment,
                      const chpl::uast::BuilderResult& builderResult);

// apply fixups to fix SymExprs to refer to Symbols that
// might have been created in a different order.
// TODO: in the future, this should be a method on Converter,
// and there should be 1 Converter to convert a module and its dependencies.
void postConvertApplyFixups(chpl::Context* context);

#endif
