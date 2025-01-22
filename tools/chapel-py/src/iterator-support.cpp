/*
 * Copyright 2023-2025 Hewlett Packard Enterprise Development LP
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

#include "iterator-support.h"
#include "core-types-gen.h"
#include "python-type-helper.h"

PyTypeObject* AstIterType = NULL;

bool setupAstIterType() {
  PyType_Slot slots[] = {
      {Py_tp_dealloc, (void*) AstIterObject_dealloc},
      {Py_tp_doc, (void*) PyDoc_STR("An iterator over Chapel AST nodes")},
      {Py_tp_iter, (void*) AstIterObject_iter},
      {Py_tp_iternext, (void*) AstIterObject_next},
      {Py_tp_init, (void*) AstIterObject_init},
      {Py_tp_new, (void*) PyType_GenericNew},
      {0, nullptr}
    };
    PyType_Spec spec = {
      /*name*/ "chapel.AstIter",
      /*basicsize*/ sizeof(AstIterObject),
      /*itemsize*/ 0,
      /*flags*/ Py_TPFLAGS_DEFAULT,
      /*slots*/ slots
    };
    AstIterType = (PyTypeObject*)PyType_FromSpec(&spec);
    if (!AstIterType || PyType_Ready(AstIterType) < 0) return false;
    return true;
}

int AstIterObject_init(AstIterObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* astObjectPy;
  if (!PyArg_ParseTuple(args, "O", &astObjectPy))
      return -1;

  auto contextObject = (ContextObject*) astObjectPy;

  Py_INCREF(contextObject);
  self->contextObject = (PyObject*) contextObject;
  self->iterAdapter = nullptr;

  return 0;
}

void AstIterObject_dealloc(AstIterObject* self) {
  delete self->iterAdapter;
  Py_XDECREF(self->contextObject);
  call_tp_free(AstIterType, (PyObject*) self);
}

PyObject* AstIterObject_iter(AstIterObject *self) {
  Py_INCREF(self);
  return (PyObject*) self;
}

PyObject* AstIterObject_next(AstIterObject *self) {
  if (self->iterAdapter) {
    if (auto nextNode = self->iterAdapter->next((ContextObject*) self->contextObject)) {
      return nextNode;
    }
  }
  PyErr_SetNone(PyExc_StopIteration);
  return nullptr;
}

PyTypeObject* AstCallIterType = NULL;

bool setupAstCallIterType() {
  PyType_Slot slots[] = {
    {Py_tp_dealloc, (void*) AstCallIterObject_dealloc},
    {Py_tp_doc, (void*) PyDoc_STR("An iterator over Chapel function call actuals")},
    {Py_tp_iter, (void*) AstCallIterObject_iter},
    {Py_tp_iternext, (void*) AstCallIterObject_next},
    {Py_tp_init, (void*) AstCallIterObject_init},
    {Py_tp_new, (void*) PyType_GenericNew},
    {0, nullptr}
  };
  PyType_Spec spec = {
    /*name*/ "chapel.AstCallIter",
    /*basicsize*/ sizeof(AstCallIterObject),
    /*itemsize*/ 0,
    /*flags*/ Py_TPFLAGS_DEFAULT,
    /*slots*/ slots
  };
  AstCallIterType = (PyTypeObject*)PyType_FromSpec(&spec);
  if (!AstCallIterType || PyType_Ready(AstCallIterType) < 0) return false;
  return true;
}

int AstCallIterObject_init(AstCallIterObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* astObjectPy;
  if (!PyArg_ParseTuple(args, "O", &astObjectPy))
      return -1;
  auto astObject = (AstNodeObject*) astObjectPy;

  Py_INCREF(astObject->contextObject);
  self->contextObject = astObject->contextObject;

  return 0;
}

void AstCallIterObject_dealloc(AstCallIterObject* self) {
  Py_XDECREF(self->contextObject);
  call_tp_free(AstCallIterType, (PyObject*) self);
}

PyObject* AstCallIterObject_iter(AstCallIterObject *self) {
  Py_INCREF(self);
  return (PyObject*) self;
}

PyObject* AstCallIterObject_next(AstCallIterObject *self) {
  if (self->current == self->num) {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
  auto argName = self->nameGetter(self->container, self->current);
  auto child = wrapGeneratedType((ContextObject*) self->contextObject,
                           self->childGetter(self->container, self->current));
  PyObject* toReturn = nullptr;
  if (!argName.isEmpty()) {
    toReturn = Py_BuildValue("sO", argName.c_str(), child);
    Py_XDECREF(child);
  } else {
    toReturn = child;
  }
  self->current++;
  return toReturn;
}

PyObject* wrapIterAdapter(ContextObject* context, IterAdapterBase* iterAdapter) {
  auto argList = Py_BuildValue("(O)", (PyObject*) context);
  auto astIterObjectPy = PyObject_CallObject((PyObject *) AstIterType, argList);
  auto astIterObject = (AstIterObject*) astIterObjectPy;

  astIterObject->iterAdapter = iterAdapter;

  Py_XDECREF(argList);
  return astIterObjectPy;
}
