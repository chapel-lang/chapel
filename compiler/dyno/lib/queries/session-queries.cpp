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

#include "chpl/queries/query-impl.h"
#include "chpl/queries/session-queries.h"

namespace chpl {


const Flags& flagsList(Context* context) {
  QUERY_BEGIN_INPUT(flagsList, context);
  Flags ret;
  return QUERY_END(ret);
}

void setFlagsList(Context* context, Flags list) {
  QUERY_STORE_INPUT_RESULT(flagsList, context, list);
}

bool isFlagSet(Context* context, Flags::Name flag) {
  auto& list = chpl::flagsList(context);
  return list.get(flag);
}

} // end namespace chpl
