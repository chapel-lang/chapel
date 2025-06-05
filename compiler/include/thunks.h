/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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
#include "astutil.h"

/* Lower calls to PRIM_CREATE_THUNK into calls to a builder function for
   a thunk record. This behaves similarly to lowering loop expressions. */
void lowerThunkPrims(BaseAST* ast);

/* After a builder function for a thunk has been resolved, create the
   corresponding thunk record and necessary method prototypes.
   The prototypes are filled in during the iterator lowering pass,
   since they behave similarly. */
void protoThunkRecord(FnSymbol* fn);

/* During iterator lowering, finalize the "invoke" method of the thunk record
   and flesh out the builder function. */
void lowerThunk(FnSymbol* fn);
