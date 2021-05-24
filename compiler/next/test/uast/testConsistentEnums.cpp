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

#include "chpl/uast/Variable.h"
#include "chpl/uast/Formal.h"
#include "chpl/uast/Function.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

using namespace chpl;
using namespace uast;

static void checkConsistentEnums() {
  assert((int) Variable::VAR == (int) Formal::DEFAULT_INTENT);
  assert((int) Formal::DEFAULT_INTENT == (int) Function::DEFAULT_RETURN_INTENT);
  
  assert((int) Variable::CONST == (int) Formal::CONST);
  assert((int) Formal::CONST == (int) Function::CONST);

  assert((int) Variable::CONST_REF == (int) Formal::CONST_REF);
  assert((int) Formal::CONST_REF == (int) Function::CONST_REF);

  assert((int) Variable::REF == (int) Formal::REF);
  assert((int) Formal::REF == (int) Function::REF);

  assert((int) Variable::PARAM == (int) Formal::PARAM);
  assert((int) Formal::PARAM == (int) Function::PARAM);

  assert((int) Variable::TYPE == (int) Formal::TYPE);
  assert((int) Formal::TYPE == (int) Function::TYPE);
}

int main(int argc, char** argv) {
  checkConsistentEnums();

  return 0;
}
