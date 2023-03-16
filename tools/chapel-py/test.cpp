#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"

typedef struct {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::uast::AstNode* astNode;
} AstObject;

typedef struct {
  PyObject_HEAD
  chpl::uast::AstListIterator<chpl::uast::AstNode> current;
  chpl::uast::AstListIterator<chpl::uast::AstNode> end;
  PyObject* contextObject;
} AstIterObject;

typedef struct {
  PyObject_HEAD
  chpl::Context context;
  /* Type-specific fields go here. */
} ContextObject;

// Forward declaration for all the types.
extern PyTypeObject AstType;
extern PyTypeObject AstIterType;
extern PyTypeObject ContextType;

static int AstIterObject_init(AstIterObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* astObjectPy;
  if (!PyArg_ParseTuple(args, "O", &astObjectPy))
      return -1;
  auto astObject = (AstObject*) astObjectPy;
  auto contextObject = (ContextObject*) astObject->contextObject;

  auto iterPair = astObject->astNode->children();
  Py_INCREF(astObject->contextObject);
  new (&self->current) chpl::uast::AstListIterator<chpl::uast::AstNode>(iterPair.begin());
  new (&self->end) chpl::uast::AstListIterator<chpl::uast::AstNode>(iterPair.end());
  self->contextObject = astObject->contextObject;

  return 0;
}

static void AstIterObject_dealloc(AstIterObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AstIterObject_next(AstIterObject *self) {
  if (self->current == self->end) {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
  auto argList = Py_BuildValue("(O)", self->contextObject);
  auto astObjectPy = PyObject_CallObject((PyObject *) &AstType, argList);
  Py_XDECREF(argList);
  auto astObject = (AstObject*) astObjectPy;
  astObject->astNode = *self->current;
  self->current++;

  return astObjectPy;
}

PyTypeObject AstIterType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.AstIter",
  .tp_doc = PyDoc_STR("An iterator over Chapel AST nodes"),
  .tp_basicsize = sizeof(AstIterObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyType_GenericNew,
  .tp_init = (initproc) AstIterObject_init,
  .tp_dealloc = (destructor) AstIterObject_dealloc,
  .tp_iternext = (iternextfunc) AstIterObject_next,
};

static int AstObject_init(AstObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  const char* filePath = nullptr;
  if (!PyArg_ParseTuple(args, "O|s", &contextObjectPy, &filePath))
      return -1;
  auto contextObject = (ContextObject*) contextObjectPy;
  auto context = &contextObject->context;

  const chpl::uast::AstNode* containedNode;
  if (filePath) {
    auto pathUS = chpl::UniqueString::get(context, filePath);
    auto parentPathUS = chpl::UniqueString();
    auto& buildResult = chpl::parsing::parseFileToBuilderResult(context, pathUS, parentPathUS);
    // TODO: clearly not the right API
    containedNode = buildResult.topLevelExpression(0);
  } else {
    // Path omitted; return null-valued `Ast` object and expect for something
    // else to initialize the value.
    containedNode = nullptr;
  }

  Py_INCREF(contextObjectPy);
  self->astNode = containedNode;
  self->contextObject = contextObjectPy;

  return 0;
}

static void AstObject_dealloc(AstObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AstObject_dump(AstObject *self, PyObject *Py_UNUSED(ignored)) {
  self->astNode->dump();
  Py_RETURN_NONE;
}

static PyObject* AstObject_tag(AstObject *self, PyObject *Py_UNUSED(ignored)) {
  const char* nodeType = chpl::uast::asttags::tagToString(self->astNode->tag());
  return Py_BuildValue("s", nodeType);
}

static PyObject* AstObject_iter(AstObject *self) {
  auto argList = Py_BuildValue("(O)", (PyObject*) self);
  auto astIterObjectPy = PyObject_CallObject((PyObject *) &AstIterType, argList);
  Py_XDECREF(argList);
  return astIterObjectPy;
}

static PyMethodDef AstObject_methods[] = {
  {"dump", (PyCFunction) AstObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {"tag", (PyCFunction) AstObject_tag, METH_NOARGS, "Get a string representation of the AST node's type"},
  {NULL}  /* Sentinel */
};

PyTypeObject AstType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.Ast",
  .tp_doc = PyDoc_STR("An opaque reference to a Chapel AST node"),
  .tp_basicsize = sizeof(AstObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyType_GenericNew,
  .tp_init = (initproc) AstObject_init,
  .tp_dealloc = (destructor) AstObject_dealloc,
  .tp_methods = AstObject_methods,
  .tp_iter = (getiterfunc) AstObject_iter,
};

static int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->context) chpl::Context(getenv("CHPL_HOME"));
  return 0;
}

static void ContextObject_dealloc(ContextObject* self) {
  self->context.~Context();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyTypeObject ContextType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.Context",
  .tp_doc = PyDoc_STR("The Chapel context object that tracks various frontend state"),
  .tp_basicsize = sizeof(ContextObject),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_new = PyType_GenericNew,
  .tp_init = (initproc) ContextObject_init,
  .tp_dealloc = (destructor) ContextObject_dealloc,
};

static PyMethodDef ChapelMethods[] = {
  { NULL, NULL, 0, NULL } /* Sentinel */
};

static PyModuleDef ChapelModule {
  PyModuleDef_HEAD_INIT,
  .m_name="chapel",
  .m_doc="A Python bridge for the Chapel frontend library",
  .m_size=-1 /* Per-interpreter memory (not used currently) */,
  .m_methods=ChapelMethods,
};

extern "C" {

PyMODINIT_FUNC PyInit_chapel() {
  PyObject* chapelModule = nullptr;
  if (PyType_Ready(&ContextType) < 0) return nullptr;
  if (PyType_Ready(&AstType) < 0) return nullptr;
  if (PyType_Ready(&AstIterType) < 0) return nullptr;
  chapelModule = PyModule_Create(&ChapelModule);
  if (!chapelModule) return nullptr;

  if (PyModule_AddObject(chapelModule, "Context", (PyObject *) &ContextType) < 0) {
    Py_DECREF(&ContextType);
    Py_DECREF(chapelModule);
    return NULL;
  }
  if (PyModule_AddObject(chapelModule, "Ast", (PyObject *) &AstType) < 0) {
    Py_DECREF(&ContextType);
    Py_DECREF(&AstType);
    Py_DECREF(chapelModule);
    return NULL;
  }
  if (PyModule_AddObject(chapelModule, "AstIter", (PyObject *) &AstIterType) < 0) {
    Py_DECREF(&ContextType);
    Py_DECREF(&AstType);
    Py_DECREF(&AstIterType);
    Py_DECREF(chapelModule);
    return NULL;
  }
  return chapelModule;
}

}

int main() {
  chpl::Context myContext;
}
