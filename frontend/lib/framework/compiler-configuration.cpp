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

#include "chpl/framework/compiler-configuration.h"

#include "chpl/framework/query-impl.h"

namespace chpl {


const CompilerFlags& compilerFlags(Context* context) {
  QUERY_BEGIN_INPUT(compilerFlags, context);
  CompilerFlags ret;
  return QUERY_END(ret);
}

void setCompilerFlags(Context* context, CompilerFlags list) {
  QUERY_STORE_INPUT_RESULT(compilerFlags, context, list);
}

bool isCompilerFlagSet(Context* context, CompilerFlags::Name flag) {
  auto& list = chpl::compilerFlags(context);
  return list.get(flag);
}


} // end namespace chpl
