#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"

static PyMethodDef ChapelMethods[] = {
  { NULL, NULL, 0, NULL } /* Sentinel */
};


typedef struct {
    PyObject_HEAD
    chpl::Context context;
    /* Type-specific fields go here. */
} ContextObject;

static PyObject* ChapelContext_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
  ContextObject* self;
  self = (ContextObject*) type->tp_alloc(type, 0);
  if (!self) return nullptr;
  new (&self->context) chpl::Context(getenv("CHPL_HOME"));
  return (PyObject*) self;
}

static void ChapelContext_dealloc(ContextObject* self) {
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
  .tp_new = ChapelContext_new,
  .tp_dealloc = (destructor) ChapelContext_dealloc,
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
  chapelModule = PyModule_Create(&ChapelModule);
  if (!chapelModule) return nullptr;

  if (PyModule_AddObject(chapelModule, "Context", (PyObject *) &ContextType) < 0) {
    Py_DECREF(&ContextType);
    Py_DECREF(chapelModule);
    return NULL;
  }
  return chapelModule;
}

}

int main() {
  chpl::Context myContext;
}
