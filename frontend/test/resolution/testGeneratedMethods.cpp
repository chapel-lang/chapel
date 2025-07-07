/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "test-common.h"
#include "test-resolution.h"

static const std::vector<std::string> recordBodies = {
  "" /* empty record */,
  "var field: int;" /* single field */,
  "var field1: int; var field2: int;" /* two fields */,
  "var field1: int; var field2: string;" /* record type (string) as field */,
};

// check that `hash` behaves as expected
static void test1() {
  for (auto& body : recordBodies) {
    Context* context;

    // no declaration (expecting generated method)
    {
      context = buildStdContext();
      ErrorGuard guard(context);
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
          }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.type() && qt.type()->isUintType());
      assert(qt.kind() == QualifiedType::CONST_VAR);
    }

    // primary method
    {
      context = buildStdContext();
      ErrorGuard guard(context);
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
            proc hash() { return 42; }
          }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.type() && qt.type()->isIntType());
      assert(qt.kind() == QualifiedType::CONST_VAR);
    }

    // secondary operator method
    {
      context = buildStdContext();
      ErrorGuard guard(context);
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
          }
          proc R.hash() { return 42; }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.type() && qt.type()->isIntType());
      assert(qt.kind() == QualifiedType::CONST_VAR);
    }
  }
}

// operators `==` and `hash` should disallow hash
static void test2() {
  for (auto& body : recordBodies) {
    Context* context;

    {
      context = buildStdContext();
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
            operator ==(lhs: R, rhs: R) { return true; }
          }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.isErroneousType());
    }

    {
      context = buildStdContext();
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
          }
          operator R.==(lhs: R, rhs: R) { return true; }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.isErroneousType());
    }

    {
      context = buildStdContext();
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
            operator !=(lhs: R, rhs: R) { return true; }
          }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.isErroneousType());
    }

    {
      context = buildStdContext();
      QualifiedType qt = resolveTypeOfXInit(context,
        R""""(
          record R {
            )"""" + body + R""""(
          }
          operator R.!=(lhs: R, rhs: R) { return true; }
          var myR: R;
          var x = myR.hash();
        )""""
      );
      assert(qt.isErroneousType());
    }
  }
}

int main() {
  test1();
  test2();
  return 0;
}
