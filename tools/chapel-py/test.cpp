#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"

static PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);

typedef struct {
  PyObject_HEAD
  chpl::uast::AstListIterator<chpl::uast::AstNode> current;
  chpl::uast::AstListIterator<chpl::uast::AstNode> end;
  PyObject* contextObject;
} AstIterObject;
extern PyTypeObject AstIterType;

typedef struct {
  PyObject_HEAD
  chpl::Context context;
  /* Type-specific fields go here. */
} ContextObject;
extern PyTypeObject ContextType;

typedef struct {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::uast::AstNode* astNode;
} AstNodeObject;
extern PyTypeObject AstNodeType;

#define DEFINE_PY_CLASS_FOR(NAME, TAG)\
  typedef struct { \
    AstNodeObject parent; \
  } NAME##Object; \
  \
  extern PyTypeObject NAME##Type; \
  static int NAME##Object_init(NAME##Object* self, PyObject* args, PyObject* kwargs) { \
    return parentTypeFor(chpl::uast::asttags::TAG)->tp_init((PyObject*) self, args, kwargs); \
  } \

#define AST_NODE(NAME) DEFINE_PY_CLASS_FOR(NAME, NAME)
#define AST_LEAF(NAME) DEFINE_PY_CLASS_FOR(NAME, NAME)
#define AST_BEGIN_SUBCLASSES(NAME) DEFINE_PY_CLASS_FOR(NAME, START_##NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

static PyObject* wrapAstNode(ContextObject* context, const chpl::uast::AstNode* node) {
  PyObject* toReturn = nullptr;
  PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
  switch (node->tag()) {
#define CAST_TO(NAME) \
    case chpl::uast::asttags::NAME: \
      toReturn = PyObject_CallObject((PyObject*) &NAME##Type, args); \
      ((NAME##Object*) toReturn)->parent.astNode = node->to##NAME(); \
      break;
#define AST_NODE(NAME) CAST_TO(NAME)
#define AST_LEAF(NAME) CAST_TO(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
    default: break;
  }
  Py_XDECREF(args);
  return toReturn;
}

static PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag) {
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME) \
  if (tag > chpl::uast::asttags::START_##NAME && tag < chpl::uast::asttags::END_##NAME) { \
    return &NAME##Type; \
  }
#include "chpl/uast/uast-classes-list.h"
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
  return &AstNodeType;
}

static int AstIterObject_init(AstIterObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* astObjectPy;
  if (!PyArg_ParseTuple(args, "O", &astObjectPy))
      return -1;
  // TODO: unsafe cast! We're just lucky because we generate the same "shape" for each node type
  auto astObject = (AstNodeObject*) astObjectPy;
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
  auto toReturn = wrapAstNode((ContextObject*) self->contextObject, *self->current);
  self->current++;
  return toReturn;
}

PyTypeObject AstIterType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.AstIter",
  .tp_basicsize = sizeof(AstIterObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) AstIterObject_dealloc,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("An iterator over Chapel AST nodes"),
  .tp_iternext = (iternextfunc) AstIterObject_next,
  .tp_init = (initproc) AstIterObject_init,
  .tp_new = PyType_GenericNew,
};

static int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->context) chpl::Context(getenv("CHPL_HOME"));
  return 0;
}

static void ContextObject_dealloc(ContextObject* self) {
  self->context.~Context();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* ContextObject_parse(ContextObject *self, PyObject* args) {
  auto context = &self->context;
  const char* fileName;
  if (!PyArg_ParseTuple(args, "s", &fileName)) {
    PyErr_BadArgument();
    return nullptr;
  }
  auto fileNameUS = chpl::UniqueString::get(context, fileName);
  auto parentPathUS = chpl::UniqueString();
  auto& builderResult = chpl::parsing::parseFileToBuilderResult(context, fileNameUS, parentPathUS);
  return wrapAstNode(self, builderResult.topLevelExpression(0));
}

static PyMethodDef ContextObject_methods[] = {
  { "parse", (PyCFunction) ContextObject_parse, METH_VARARGS, "Parse a top-level AST node from the given file" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject ContextType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.Context",
  .tp_basicsize = sizeof(ContextObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) ContextObject_dealloc,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("The Chapel context object that tracks various frontend state"),
  .tp_methods = ContextObject_methods,
  .tp_init = (initproc) ContextObject_init,
  .tp_new = PyType_GenericNew,
};

static int AstNodeObject_init(AstNodeObject* self, PyObject* args, PyObject* kwargs) { \
  PyObject* contextObjectPy;
  if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
      return -1;
  auto contextObject = (ContextObject*) contextObjectPy;
  auto context = &contextObject->context;

  Py_INCREF(contextObjectPy);
  self->astNode = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

static void AstNodeObject_dealloc(AstNodeObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AstNodeObject_dump(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  self->astNode->dump();
  Py_RETURN_NONE;
}

static PyObject* AstNodeObject_tag(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  const char* nodeType = chpl::uast::asttags::tagToString(self->astNode->tag());
  return Py_BuildValue("s", nodeType);
}

static PyObject* AstNodeObject_iter(AstNodeObject *self) {
  auto argList = Py_BuildValue("(O)", (PyObject*) self);
  auto astIterObjectPy = PyObject_CallObject((PyObject *) &AstIterType, argList);
  Py_XDECREF(argList);
  return astIterObjectPy;
}

static PyMethodDef AstNodeObject_methods[] = {
  {"dump", (PyCFunction) AstNodeObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {"tag", (PyCFunction) AstNodeObject_tag, METH_NOARGS, "Get a string representation of the AST node's type"},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyTypeObject AstNodeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "chapel.AstNode",
  .tp_basicsize = sizeof(AstNodeObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) AstNodeObject_dealloc,
  .tp_flags = Py_TPFLAGS_BASETYPE,
  .tp_doc = PyDoc_STR("The base type of Chapel AST nodes"),
  .tp_iter = (getiterfunc) AstNodeObject_iter,
  .tp_methods = AstNodeObject_methods,
  .tp_init = (initproc) AstNodeObject_init,
  .tp_new = PyType_GenericNew,
};


static PyObject* NamedDeclObject_name(PyObject *self, PyObject *Py_UNUSED(ignored)) {
  auto namedDecl = ((NamedDeclObject*) self)->parent.astNode->toNamedDecl();
  return Py_BuildValue("s", namedDecl->name().c_str());
}

static PyObject* CommentObject_text(PyObject *self, PyObject *Py_UNUSED(ignored)) {
  auto namedDecl = ((CommentObject*) self)->parent.astNode->toComment();
  return Py_BuildValue("s", namedDecl->c_str());
}

template <chpl::uast::asttags::AstTag tag>
struct PerNodeInfo {
  static constexpr PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

template <>
struct PerNodeInfo<chpl::uast::asttags::START_NamedDecl> {
  static constexpr PyMethodDef methods[] = {
    {"name", NamedDeclObject_name, METH_NOARGS, "Get the name of this NamedDecl node"},
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

template <>
struct PerNodeInfo<chpl::uast::asttags::Comment> {
  static constexpr PyMethodDef methods[] = {
    {"text", CommentObject_text, METH_NOARGS, "Get the text of the Comment node"},
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

#define DEFINE_PY_TYPE_FOR(NAME, TAG, FLAGS)\
  static PyMethodDef NAME##Object_methods[] = { \
    {NULL}  /* Sentinel */ \
  }; \
  \
  PyTypeObject NAME##Type = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    .tp_name = "chapel." #NAME, \
    .tp_basicsize = sizeof(NAME##Object), \
    .tp_itemsize = 0, \
    .tp_flags = FLAGS, \
    .tp_doc = PyDoc_STR("A Chapel " #NAME " AST node"), \
    .tp_methods = (PyMethodDef*) PerNodeInfo<TAG>::methods, \
    .tp_base = parentTypeFor(TAG), \
    .tp_init = (initproc) NAME##Object_init, \
    .tp_new = PyType_GenericNew, \
  }; \

#define AST_NODE(NAME) DEFINE_PY_TYPE_FOR(NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_LEAF(NAME) DEFINE_PY_TYPE_FOR(NAME, chpl::uast::asttags::NAME, Py_TPFLAGS_DEFAULT)
#define AST_BEGIN_SUBCLASSES(NAME) DEFINE_PY_TYPE_FOR(NAME, chpl::uast::asttags::START_##NAME, Py_TPFLAGS_BASETYPE)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

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
  if (PyType_Ready(&AstIterType) < 0) return nullptr;
  if (PyType_Ready(&AstNodeType) < 0) return nullptr;
#define READY_TYPE(NAME) if (PyType_Ready(&NAME##Type) < 0) return nullptr;
#define AST_NODE(NAME) READY_TYPE(NAME)
#define AST_LEAF(NAME) READY_TYPE(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) READY_TYPE(NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

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
