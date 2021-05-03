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

#ifndef CHPL_AST_LITERAL_H
#define CHPL_AST_LITERAL_H

#include "chpl/uast/Exp.h"

namespace chpl {
namespace uast {


/**
  This class represents a literal.

  Here are some example literals:

  \rst
  .. code-block:: chapel

    1 2.0 3.0i "string" b"bytes"
  \endrst
 */
class Literal final : public Exp {
  // TODO: move over 'ifa' code
};


} // end namespace uast
} // end namespace chpl

#endif
