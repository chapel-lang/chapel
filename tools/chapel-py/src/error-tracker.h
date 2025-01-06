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

#ifndef CHAPEL_PY_ERROR_TRACKER_H
#define CHAPEL_PY_ERROR_TRACKER_H

#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorBase.h"
#include "python-class.h"

struct ContextObject;

struct ErrorObject : public PythonClassWithContext<ErrorObject, chpl::owned<chpl::ErrorBase>> {
  static constexpr const char* QualifiedName = "chapel.Error";
  static constexpr const char* Name = "Error";
  static constexpr const char* DocStr = "An error that occurred as part of processing a file with the Chapel compiler frontend";
};

using LocationAndNote = std::tuple<chpl::Location, std::string>;

struct ErrorManagerObject : public PythonClassWithContext<ErrorManagerObject, std::tuple<>> {
  static constexpr const char* QualifiedName = "chapel.ErrorManager";
  static constexpr const char* Name = "ErrorManager";
  static constexpr const char* DocStr = "A wrapper container to help track the errors from a Context.";
};

class PythonErrorHandler : public chpl::Context::ErrorHandler {
 private:
  std::vector<PyObject*> errorLists;
  PyObject* contextObject; // weak: ContextObject should own the DefaultErrorHandler
 public:
  PythonErrorHandler(PyObject* contextObject) : contextObject(contextObject) {}
  ~PythonErrorHandler() = default;

  PyObject* pushList();
  void popList();
  virtual void report(chpl::Context* context, const chpl::ErrorBase* err) override;
};

#endif
