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

#ifndef CHPL_RESOLUTION_INTENTS_H
#define CHPL_RESOLUTION_INTENTS_H

#include "chpl/types/QualifiedType.h"

namespace chpl {
namespace resolution {


/** Resolve the intent of a formal argument based on its type
    and (possibly generic) intent. For example 'const' is a generic
    intent and it has different behavior depending on the type.

    The argument 'isThis' indicates if the formal argument is
    the 'this' method receiver argument.

    The generic and resolved intents are all represented as QualifiedType::Kind.

    If the type is not known or only partially known, this function
    can return a generic intent.
 */
types::QualifiedType::Kind resolveIntent(const types::QualifiedType& t,
                                         bool isThis);


} // end namespace resolution
} // end namespace chpl

#endif
