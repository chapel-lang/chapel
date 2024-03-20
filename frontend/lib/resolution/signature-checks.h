/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef SIGNATURE_CHECKS_
#define SIGNATURE_CHECKS_

namespace chpl {
class Context;

namespace resolution {
class TypedFnSignature;


/**
  Check for errors with the passed signature.
  This is supported by a query so that errors are only
  reported once per signature.
 */
void checkSignature(Context* context, const TypedFnSignature* sig);


} // end namespace resolution
} // end namespace chpl

#endif
