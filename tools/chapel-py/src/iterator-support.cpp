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

#include "iterator-support.h"
#include "core-types-gen.h"

PyTypeObject AstIterType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupAstIterType() {
  AstIterType.tp_name = "chapel.AstIter";
  AstIterType.tp_basicsize = sizeof(AstIterObject);
  AstIterType.tp_itemsize = 0;
  AstIterType.tp_dealloc = (destructor) AstIterObject_dealloc;
  AstIterType.tp_flags = Py_TPFLAGS_DEFAULT;
  AstIterType.tp_doc = PyDoc_STR("An iterator over Chapel AST nodes");
  AstIterType.tp_iter = (getiterfunc) AstIterObject_iter;
  AstIterType.tp_iternext = (iternextfunc) AstIterObject_next;
  AstIterType.tp_init = (initproc) AstIterObject_init;
  AstIterType.tp_new = PyType_GenericNew;
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
  Py_TYPE(self)->tp_free((PyObject *) self);
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

PyTypeObject AstCallIterType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupAstCallIterType() {
  AstCallIterType.tp_name = "chapel.AstCallIter";
  AstCallIterType.tp_basicsize = sizeof(AstCallIterObject);
  AstCallIterType.tp_itemsize = 0;
  AstCallIterType.tp_dealloc = (destructor) AstCallIterObject_dealloc;
  AstCallIterType.tp_flags = Py_TPFLAGS_DEFAULT;
  AstCallIterType.tp_doc = PyDoc_STR("An iterator over Chapel function call actuals");
  AstCallIterType.tp_iter = (getiterfunc) AstCallIterObject_iter;
  AstCallIterType.tp_iternext = (iternextfunc) AstCallIterObject_next;
  AstCallIterType.tp_init = (initproc) AstCallIterObject_init;
  AstCallIterType.tp_new = PyType_GenericNew;
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
  Py_TYPE(self)->tp_free((PyObject *) self);
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
  auto astIterObjectPy = PyObject_CallObject((PyObject *) &AstIterType, argList);
  auto astIterObject = (AstIterObject*) astIterObjectPy;

  astIterObject->iterAdapter = iterAdapter;

  Py_XDECREF(argList);
  return astIterObjectPy;
}
