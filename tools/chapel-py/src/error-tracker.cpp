/*
 * Copyright 2023-2024 Hewlett Packard Enterprise Development LP
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

static PyMethodDef ErrorObject_methods[] = {
  { "location", (PyCFunction) ErrorObject_location, METH_NOARGS, "Get the location at which this error occurred" },
  { "message", (PyCFunction) ErrorObject_message, METH_NOARGS, "Retrieve the contents of this error message" },
  { "kind", (PyCFunction) ErrorObject_kind, METH_NOARGS, "Retrieve the kind ('error', 'warning') of this type of error" },
  { "type", (PyCFunction) ErrorObject_type, METH_NOARGS, "Retrieve the unique name of this type of error" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject ErrorType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupErrorType() {
  ErrorType.tp_name = "Error";
  ErrorType.tp_basicsize = sizeof(ErrorObject);
  ErrorType.tp_itemsize = 0;
  ErrorType.tp_dealloc = (destructor) ErrorObject_dealloc;
  ErrorType.tp_flags = Py_TPFLAGS_DEFAULT;
  ErrorType.tp_doc = PyDoc_STR("An error that occurred as part of processing a file with the Chapel compiler frontend");
  ErrorType.tp_methods = ErrorObject_methods;
  ErrorType.tp_init = (initproc) ErrorObject_init;
  ErrorType.tp_new = PyType_GenericNew;
}

int ErrorObject_init(ErrorObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->error) chpl::owned<chpl::ErrorBase>();
  self->contextObject = nullptr;
  return 0;
}

void ErrorObject_dealloc(ErrorObject* self) {
  self->error.~unique_ptr(); // TODO: leaks type alias, but ~chpl::owned doesn't work.
  Py_DECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ErrorObject_location(ErrorObject* self, PyObject* args) {
  auto locationObjectPy = PyObject_CallObject((PyObject *) &LocationType, nullptr);
  auto& location = ((LocationObject*) locationObjectPy)->location;
  auto context = &((ContextObject*) self->contextObject)->context_;

  location = self->error->location(context);
  return locationObjectPy;
}

PyObject* ErrorObject_message(ErrorObject* self, PyObject* args) {
  return Py_BuildValue("s", self->error->message().c_str());
}

PyObject* ErrorObject_kind(ErrorObject* self, PyObject* args) {
  return Py_BuildValue("s", chpl::ErrorBase::getKindName(self->error->kind()));
}

PyObject* ErrorObject_type(ErrorObject* self, PyObject* args) {
  return Py_BuildValue("s", chpl::ErrorBase::getTypeName(self->error->type()));
}

static PyMethodDef ErrorManagerObject_methods[] = {
  { "__enter__", (PyCFunction) ErrorManagerObject_enter, METH_NOARGS, "The context manager 'enter' method for this ErrorManager object" },
  { "__exit__", (PyCFunction) ErrorManagerObject_exit, METH_VARARGS, "The context manager 'exit' method for this ErrorManager object" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject ErrorManagerType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupErrorManagerType() {
  ErrorManagerType.tp_name = "ErrorManager";
  ErrorManagerType.tp_basicsize = sizeof(ErrorManagerObject);
  ErrorManagerType.tp_itemsize = 0;
  ErrorManagerType.tp_dealloc = (destructor) ErrorManagerObject_dealloc;
  ErrorManagerType.tp_flags = Py_TPFLAGS_DEFAULT;
  ErrorManagerType.tp_doc = PyDoc_STR("A wrapper container to help track the errors from a Context.");
  ErrorManagerType.tp_methods = ErrorManagerObject_methods;
  ErrorManagerType.tp_init = (initproc) ErrorManagerObject_init;
  ErrorManagerType.tp_new = PyType_GenericNew;
}

int ErrorManagerObject_init(ErrorManagerObject* self, PyObject* args, PyObject* kwargs) {
  self->contextObject = nullptr;
  return 0;
}

void ErrorManagerObject_dealloc(ErrorManagerObject* self) {
  Py_DECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ErrorManagerObject_enter(ErrorManagerObject* self, PyObject* args) {
  auto& context = ((ContextObject*) self->contextObject)->context_;
  auto list = ((PythonErrorHandler*) context.errorHandler())->pushList();
  Py_INCREF(list);
  return list;
}

PyObject* ErrorManagerObject_exit(ErrorManagerObject* self, PyObject* args) {
  auto& context = ((ContextObject*) self->contextObject)->context_;
  ((PythonErrorHandler*) context.errorHandler())->popList();

  Py_RETURN_NONE;
}

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
  auto errorObjectPy = PyObject_CallObject((PyObject *) &ErrorType, nullptr);
  auto errorObject = (ErrorObject*) errorObjectPy;
  errorObject->error = err->clone();
  Py_INCREF(contextObject);
  errorObject->contextObject = contextObject;

  PyList_Append(errorLists.back(), errorObjectPy);
}
