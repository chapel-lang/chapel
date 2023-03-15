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
  chpl::Context context;
  /* Type-specific fields go here. */
} ContextObject;

static int AstObject_init(AstObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  const char* filePath;
  if (!PyArg_ParseTuple(args, "Os", &contextObjectPy, &filePath))
      return -1;
  auto contextObject = (ContextObject*) contextObjectPy;
  auto context = &contextObject->context;
  auto pathUS = chpl::UniqueString::get(context, filePath);
  auto parentPathUS = chpl::UniqueString();
  auto& buildResult = chpl::parsing::parseFileToBuilderResult(context, pathUS, parentPathUS);
  auto firstAst = buildResult.topLevelExpression(0);

  // TODO: clearly not the right API
  Py_INCREF(contextObjectPy);
  self->astNode = firstAst;
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

static PyMethodDef AstObject_methods[] = {
  {"dump", (PyCFunction) AstObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {NULL}  /* Sentinel */
};

static PyTypeObject AstType = {
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
};

static int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->context) chpl::Context(getenv("CHPL_HOME"));
  return 0;
}

static void ContextObject_dealloc(ContextObject* self) {
  self->context.~Context();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyTypeObject ContextType = {
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
  return chapelModule;
}

}

int main() {
  chpl::Context myContext;
}
