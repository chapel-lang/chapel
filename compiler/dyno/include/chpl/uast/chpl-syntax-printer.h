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

#ifndef CHPL_UAST_CHPL_SYNTAX_VISITOR_H
#define CHPL_UAST_CHPL_SYNTAX_VISITOR_H

#include "chpl/queries/Context.h"
#include "chpl/uast/ASTTag.h"
#include "chpl/uast/ASTTypes.h"
#include "chpl/uast/TypeDecl.h"
#include "chpl/uast/all-uast.h"


#include<iostream>

namespace chpl {

  /*
    With this function we are tyring to create a faithful representation
    of the uAST in Chapel syntax. The goal is to generate strings that could
    be copy/pasted and compiled back to the same uAST.
  */
  void printChapelSyntax(std::ostream& os, const uast::ASTNode* node);


  /*
    This function can be used to generate the signature of a function without
    the declaration (proc, iter etc). Suitable for the userString field
    of the old AST.
  */
  void printFunctionSignature(std::ostream& os, const uast::Function* node);

}
#endif
