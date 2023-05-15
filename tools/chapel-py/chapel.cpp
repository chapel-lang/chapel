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
  int current;
  int num;
  const void* container;
  chpl::UniqueString (*nameGetter)(const void*, int);
  const chpl::uast::AstNode* (*childGetter)(const void*, int);
  PyObject* contextObject;
} AstCallIterObject;
extern PyTypeObject AstCallIterType;

typedef struct {
  PyObject_HEAD
  chpl::Context context;
  /* Type-specific fields go here. */
} ContextObject;
extern PyTypeObject ContextType;

typedef struct {
  PyObject_HEAD
  chpl::Location location;
} LocationObject;
extern PyTypeObject LocationType;

typedef struct {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::uast::AstNode* astNode;
} AstNodeObject;
extern PyTypeObject AstNodeType;

/**
  Declare a Python PyTypeObject that corresponds to an AST node with the given
  name and tag. The tag is not the same as the name because abstract base
  classes like NamedDecl have corresponding tags called START_NamedDecl
  and END_NamedDecl, but not NamedDecl.
 */
#define DEFINE_PY_OBJECT_FOR(NAME, TAG)\
  typedef struct { \
    AstNodeObject parent; \
  } NAME##Object; \
  \
  extern PyTypeObject NAME##Type; \
  static int NAME##Object_init(NAME##Object* self, PyObject* args, PyObject* kwargs) { \
    return parentTypeFor(chpl::uast::asttags::TAG)->tp_init((PyObject*) self, args, kwargs); \
  } \

/* Generate a Python object for reach AST node type. */
#define AST_NODE(NAME) DEFINE_PY_OBJECT_FOR(NAME, NAME)
#define AST_LEAF(NAME) DEFINE_PY_OBJECT_FOR(NAME, NAME)
#define AST_BEGIN_SUBCLASSES(NAME) DEFINE_PY_OBJECT_FOR(NAME, START_##NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES

/**
  Create a Python object of the class corresponding to the given AST node's
  type. For example, an Identifier node will be wrapped in a a chapel.Identifier.
 */
static PyObject* wrapAstNode(ContextObject* context, const chpl::uast::AstNode* node) {
  PyObject* toReturn = Py_None;
  if (node == nullptr) return toReturn;
  PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
  switch (node->tag()) {
#define CAST_TO(NAME) \
    case chpl::uast::asttags::NAME: \
      toReturn = PyObject_CallObject((PyObject*) &NAME##Type, args); \
      ((NAME##Object*) toReturn)->parent.astNode = node->to##NAME(); \
      break;
#define AST_NODE(NAME) CAST_TO(NAME)
#define AST_LEAF(NAME) CAST_TO(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) /* No need to handle abstract parent classes. */
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
  .tp_name = "AstIter",
  .tp_basicsize = sizeof(AstIterObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) AstIterObject_dealloc,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("An iterator over Chapel AST nodes"),
  .tp_iternext = (iternextfunc) AstIterObject_next,
  .tp_init = (initproc) AstIterObject_init,
  .tp_new = PyType_GenericNew,
};

static int AstCallIterObject_init(AstCallIterObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* astObjectPy;
  if (!PyArg_ParseTuple(args, "O", &astObjectPy))
      return -1;
  auto astObject = (AstNodeObject*) astObjectPy;

  Py_INCREF(astObject->contextObject);
  self->contextObject = astObject->contextObject;

  return 0;
}

static void AstCallIterObject_dealloc(AstCallIterObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* AstCallIterObject_iter(AstCallIterObject *self) {
  Py_INCREF(self);
  return (PyObject*) self;
}

static PyObject* AstCallIterObject_next(AstCallIterObject *self) {
  if (self->current == self->num) {
    PyErr_SetNone(PyExc_StopIteration);
    return nullptr;
  }
  auto argName = self->nameGetter(self->container, self->current);
  auto child = wrapAstNode((ContextObject*) self->contextObject,
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

PyTypeObject AstCallIterType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "AstCallIter",
  .tp_basicsize = sizeof(AstCallIterObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) AstCallIterObject_dealloc,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("An iterator over Chapel function call actuals"),
  .tp_iter = (getiterfunc) AstCallIterObject_iter,
  .tp_iternext = (iternextfunc) AstCallIterObject_next,
  .tp_init = (initproc) AstCallIterObject_init,
  .tp_new = PyType_GenericNew,
};

static int LocationObject_init(LocationObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->location) chpl::Location();
  return 0;
}

static void LocationObject_dealloc(LocationObject* self) {
  self->location.~Location();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject* LocationObject_start(LocationObject *self, PyObject* Py_UNUSED(args)) {
  auto& location = self->location;
  return Py_BuildValue("ii", location.firstLine(), location.firstColumn());
}

static PyObject* LocationObject_end(LocationObject *self, PyObject* Py_UNUSED(args)) {
  auto& location = self->location;
  return Py_BuildValue("ii", location.lastLine(), location.lastColumn());
}

static PyObject* LocationObject_path(LocationObject *self, PyObject* Py_UNUSED(args)) {
  return Py_BuildValue("s", self->location.path().c_str());
}

static PyMethodDef LocationObject_methods[] = {
  { "start", (PyCFunction) LocationObject_start, METH_VARARGS, "Get the start of a Location object" },
  { "end", (PyCFunction) LocationObject_end, METH_VARARGS, "Get the end of a Location object" },
  { "path", (PyCFunction) LocationObject_path, METH_VARARGS, "Get the path of a Location object" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject LocationType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "Location",
  .tp_basicsize = sizeof(LocationObject),
  .tp_itemsize = 0,
  .tp_dealloc = (destructor) LocationObject_dealloc,
  .tp_flags = Py_TPFLAGS_DEFAULT,
  .tp_doc = PyDoc_STR("The Chapel context object that tracks various frontend state"),
  .tp_methods = LocationObject_methods,
  .tp_init = (initproc) LocationObject_init,
  .tp_new = PyType_GenericNew,
};

static int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs) {
  chpl::Context::Configuration config;
  config.chplHome = getenv("CHPL_HOME");
  new (&self->context) chpl::Context(std::move(config));
  chpl::parsing::setupModuleSearchPaths(&self->context, false, false, {}, {});
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
  .tp_name = "Context",
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

static PyObject* AstNodeObject_uniqueID(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  auto uniqueID = (intptr_t)self->astNode;
  return Py_BuildValue("K", uniqueID);
}


static PyObject* AstNodeObject_attribute_group(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  auto attributeGroup = self->astNode->attributeGroup();
  if (attributeGroup != nullptr) {
    return wrapAstNode((ContextObject*) self->contextObject, attributeGroup);
  }
  Py_RETURN_NONE;
}

static PyObject* AstNodeObject_parent(AstNodeObject* self, PyObject *Py_UNUSED(ignored)) {
  auto contextObject = (ContextObject*) self->contextObject;
  auto context = &contextObject->context;

  return wrapAstNode(contextObject, chpl::parsing::parentAst(context, self->astNode));
}

static PyObject* AstNodeObject_iter(AstNodeObject *self) {
  auto argList = Py_BuildValue("(O)", (PyObject*) self);
  auto astIterObjectPy = PyObject_CallObject((PyObject *) &AstIterType, argList);
  Py_XDECREF(argList);
  return astIterObjectPy;
}

static PyObject* AstNodeObject_location(AstNodeObject *self) {
  auto locationObjectPy = PyObject_CallObject((PyObject *) &LocationType, nullptr);
  auto& location = ((LocationObject*) locationObjectPy)->location;
  auto context = &((ContextObject*) self->contextObject)->context;

  location = chpl::parsing::locateAst(context, self->astNode);
  return locationObjectPy;
}

static PyMethodDef AstNodeObject_methods[] = {
  {"dump", (PyCFunction) AstNodeObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {"tag", (PyCFunction) AstNodeObject_tag, METH_NOARGS, "Get a string representation of the AST node's type"},
  {"attribute_group", (PyCFunction) AstNodeObject_attribute_group, METH_NOARGS, "Get the attribute group, if any, associated with this node"},
  {"location", (PyCFunction) AstNodeObject_location, METH_NOARGS, "Get the location of this AST node in its file"},
  {"parent", (PyCFunction) AstNodeObject_parent, METH_NOARGS, "Get the parent node of this AST node"},
  {"uniqueID", (PyCFunction) AstNodeObject_uniqueID, METH_NOARGS, "Get a unique identifer for this AST node"},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyTypeObject AstNodeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  .tp_name = "AstNode",
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

static const char* blockStyleToString(chpl::uast::BlockStyle blockStyle) {
  switch (blockStyle) {
    case chpl::uast::BlockStyle::EXPLICIT: return "explicit";
    case chpl::uast::BlockStyle::IMPLICIT: return "implicit";
    case chpl::uast::BlockStyle::UNNECESSARY_KEYWORD_AND_BLOCK: return "unnecessary";
  }
}

template <typename T>
const char* toCString(T& t) { throw std::invalid_argument("Invalid conversion into C string"); }

template <>
const char* toCString<const char*>(const char*& t) { return t; }

template <>
const char* toCString<chpl::UniqueString>(chpl::UniqueString& us) { return us.c_str(); }

#define PLAIN_GETTER(NODE, NAME, TYPESTR, BODY)\
  static PyObject* NODE##Object_##NAME(PyObject *self, PyObject *Py_UNUSED(ignored)) {\
    auto cast = ((NODE##Object*) self)->parent.astNode->to##NODE(); \
    auto result = [](const chpl::uast::NODE* node, ContextObject* contextObject) { \
      BODY; \
    }(cast, (ContextObject*) ((NODE##Object*) self)->parent.contextObject); \
    if constexpr (std::string_view(TYPESTR) == std::string_view("s")) { \
      return Py_BuildValue(TYPESTR, toCString(result)); \
    } else { \
      return Py_BuildValue(TYPESTR, result); \
    } \
  }

PLAIN_GETTER(VarLikeDecl, storage_kind, "s", return chpl::uast::qualifierToString(node->storageKind()))
PLAIN_GETTER(NamedDecl, name, "s", return node->name());
PLAIN_GETTER(Variable, is_config, "b", return node->isConfig());
PLAIN_GETTER(Attribute, name, "s", return node->name());
PLAIN_GETTER(Comment, text, "s", return node->c_str());

PLAIN_GETTER(BoolLiteral, value, "s",
             return (node->value() ? "true" : "false"));
PLAIN_GETTER(ImagLiteral, text, "s", return node->text());
PLAIN_GETTER(IntLiteral, text, "s", return node->text());
PLAIN_GETTER(RealLiteral, text, "s", return node->text());
PLAIN_GETTER(UintLiteral, text, "s", return node->text());
PLAIN_GETTER(StringLikeLiteral, value, "s", return node->value());

PLAIN_GETTER(Function, kind, "s", return chpl::uast::Function::kindToString(node->kind()));
PLAIN_GETTER(Function, is_method, "b", return node->isMethod());
PLAIN_GETTER(Function, is_primary_method, "b", return node->isPrimaryMethod());
PLAIN_GETTER(FnCall, used_square_brackets, "b", return node->callUsedSquareBrackets());
PLAIN_GETTER(FnCall, called_expression, "O", return wrapAstNode(contextObject, node->calledExpression()));
PLAIN_GETTER(Dot, field, "s", return node->field());
PLAIN_GETTER(VisibilityClause, symbol, "O", return wrapAstNode(contextObject, node->symbol()));
PLAIN_GETTER(Identifier, name, "s", return node->name());
PLAIN_GETTER(SimpleBlockLike, block_style, "s", return blockStyleToString(node->blockStyle()));
PLAIN_GETTER(Loop, block_style, "s", return blockStyleToString(node->blockStyle()));
PLAIN_GETTER(Class, parentClass, "O", return wrapAstNode(contextObject, node->parentClass()));
PLAIN_GETTER(EnumElement, initExpression, "O", return wrapAstNode(contextObject, node->initExpression()));


static PyObject* AttributeObject_actuals(PyObject *self, PyObject *Py_UNUSED(ignored)) {
  auto attributeNode = ((AttributeObject*) self)->parent.astNode->toAttribute();

  auto argList = Py_BuildValue("(O)", (PyObject*) self);
  auto astCallIterObjectPy = PyObject_CallObject((PyObject *) &AstCallIterType, argList);
  Py_XDECREF(argList);

  auto astCalliterObject = (AstCallIterObject*) astCallIterObjectPy;
  astCalliterObject->current = 0;
  astCalliterObject->num = attributeNode->numActuals();
  astCalliterObject->container = attributeNode;
  astCalliterObject->childGetter = [](const void* node, int child) {
    return ((chpl::uast::Attribute*) node)->actual(child);
  };
  astCalliterObject->nameGetter = [](const void* node, int child) {
    return ((chpl::uast::Attribute*) node)->actualName(child);
  };

  return astCallIterObjectPy;
}

template <chpl::uast::asttags::AstTag tag>
struct PerNodeInfo {
  static constexpr PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}  /* Sentinel */
  };
};

#define METHOD_TABLE(TAG, ...)\
  template <> \
  struct PerNodeInfo<chpl::uast::asttags::TAG> {\
    static constexpr PyMethodDef methods[] = {\
      __VA_ARGS__ \
      {NULL, NULL, 0, NULL}  /* Sentinel */ \
    };\
  }

METHOD_TABLE(START_VarLikeDecl,
  {"storage_kind", VarLikeDeclObject_storage_kind, METH_NOARGS, "Get the storage kind of this VarLikeDecl node"},
);

METHOD_TABLE(START_NamedDecl,
  {"name", NamedDeclObject_name, METH_NOARGS, "Get the name of this NamedDecl node"},
);

METHOD_TABLE(Variable,
  {"is_config", VariableObject_is_config, METH_NOARGS, "Check if the given Variable node is a config variable"},
);

METHOD_TABLE(Comment,
  {"text", CommentObject_text, METH_NOARGS, "Get the text of the Comment node"},
);

METHOD_TABLE(BoolLiteral,
             {"value", BoolLiteralObject_value, METH_NOARGS,
              "Get the value of the BoolLiteral node"}, );
METHOD_TABLE(ImagLiteral,
             {"text", ImagLiteralObject_text, METH_NOARGS,
              "Get the value of the ImagLiteral node"}, );
METHOD_TABLE(IntLiteral,
             {"text", IntLiteralObject_text, METH_NOARGS,
              "Get the value of the IntLiteral node"}, );
METHOD_TABLE(RealLiteral,
             {"text", RealLiteralObject_text, METH_NOARGS,
              "Get the value of the RealLiteral node"}, );
METHOD_TABLE(UintLiteral,
             {"text", UintLiteralObject_text, METH_NOARGS,
              "Get the value of the UintLiteral node"}, );
METHOD_TABLE(START_StringLikeLiteral,
  {"value", StringLikeLiteralObject_value, METH_NOARGS, "Get the value of the StringLikeLiteral node"},
);

METHOD_TABLE(Function,
  {"kind", FunctionObject_kind, METH_NOARGS, "Get the kind of this Function node"},
  {"is_method", FunctionObject_is_method, METH_NOARGS, "Check if this function is a method"},
  {"is_primary_method", FunctionObject_is_primary_method, METH_NOARGS, "Check if this function is a primary method"},
);

METHOD_TABLE(FnCall,
  {"used_square_brackets", FnCallObject_used_square_brackets, METH_NOARGS, "Check whether or not this function call was made using square brackets"},
  {"called_expression", FnCallObject_called_expression, METH_NOARGS, "Get the expression invoked by this FnCall node"},
);

METHOD_TABLE(Dot,
  {"field", DotObject_field, METH_NOARGS, "Get the field accessed in the Dot node"},
);

METHOD_TABLE(Attribute,
  {"actuals", AttributeObject_actuals, METH_NOARGS, "Get the actuals for this Attribute node"},
  {"name", AttributeObject_name, METH_NOARGS, "Get the name of this Attribute node"},
);

METHOD_TABLE(VisibilityClause,
  {"symbol", VisibilityClauseObject_symbol, METH_NOARGS, "Get the symbol referenced by this VisibilityClause node"},
);

METHOD_TABLE(Identifier,
  {"name", IdentifierObject_name, METH_NOARGS, "Get the name of this Identifier node"},
);

METHOD_TABLE(START_SimpleBlockLike,
  {"block_style", SimpleBlockLikeObject_block_style, METH_NOARGS, "Get the style of this block-like AST node"},
);

METHOD_TABLE(START_Loop,
  {"block_style", LoopObject_block_style, METH_NOARGS, "Get the style of this loop AST node"},
);

METHOD_TABLE(Class,
  {"parentClass", ClassObject_parentClass, METH_NOARGS, "Get the parent class of this class AST node"},
);
METHOD_TABLE(EnumElement,
  {"initExpression", EnumElementObject_initExpression, METH_NOARGS, "Get the initExpression of this enum element AST node"},
);

#define DEFINE_PY_TYPE_FOR(NAME, TAG, FLAGS)\
  static PyMethodDef NAME##Object_methods[] = { \
    {NULL}  /* Sentinel */ \
  }; \
  \
  PyTypeObject NAME##Type = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    .tp_name = #NAME, \
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
  .m_name="core",
  .m_doc="A Python bridge for the Chapel frontend library",
  .m_size=-1 /* Per-interpreter memory (not used currently) */,
  .m_methods=ChapelMethods,
};

extern "C" {

PyMODINIT_FUNC PyInit_core() {
  PyObject* chapelModule = nullptr;

  if (PyType_Ready(&ContextType) < 0) return nullptr;
  if (PyType_Ready(&LocationType) < 0) return nullptr;
  if (PyType_Ready(&AstIterType) < 0) return nullptr;
  if (PyType_Ready(&AstCallIterType) < 0) return nullptr;
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

#define ADD_TYPE(NAME) if (PyModule_AddObject(chapelModule, #NAME, (PyObject*) &NAME##Type) < 0) return nullptr;
#define AST_NODE(NAME) ADD_TYPE(NAME)
#define AST_LEAF(NAME) ADD_TYPE(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) ADD_TYPE(NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
  ADD_TYPE(AstNode);
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
