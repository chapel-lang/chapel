/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

#include "error-tracker.h"
#include "core-types.h"
#include "chpl/framework/ErrorBase.h"
#include "core-types-gen.h"

PyObject* PythonErrorHandler::pushList() {
  PyObject* newList = PyList_New(0);
  errorLists.push_back(newList);
  return newList;
}

void PythonErrorHandler::popList() {
  if (errorLists.empty()) return;
  auto last = errorLists.back();
  Py_DECREF(last);
  errorLists.pop_back();
}

void PythonErrorHandler::report(chpl::Context* context, const chpl::ErrorBase* err) {
  if (errorLists.empty()) {
    // No error list tracking errors, so just print to the console.
    chpl::Context::defaultReportError(context, err);
    return;
  }

  // There's an error list! Create an error object and store it into the list.
  auto errObj = wrapGeneratedType((ContextObject*) contextObject, err);
  PyList_Append(errorLists.back(), (PyObject*) errObj);
}
