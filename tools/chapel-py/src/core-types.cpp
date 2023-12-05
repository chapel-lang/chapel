/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "core-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/parsing/parsing-queries.h"
#include "python-types.h"
#include "error-tracker.h"

using namespace chpl;
using namespace uast;

static PyMethodDef ContextObject_methods[] = {
  { "parse", (PyCFunction) ContextObject_parse, METH_VARARGS, "Parse a top-level AST node from the given file" },
  { "is_bundled_path", (PyCFunction) ContextObject_is_bundled_path, METH_VARARGS, "Check if the given file path is within the bundled (built-in) Chapel files" },
  { "advance_to_next_revision", (PyCFunction) ContextObject_advance_to_next_revision, METH_VARARGS, "Advance the context to the next revision" },
  { "_get_pyi_file", (PyCFunction) ContextObject_get_pyi_file, METH_NOARGS, "Generate a stub file for the Chapel AST nodes" },
  { "track_errors", (PyCFunction) ContextObject_track_errors, METH_NOARGS, "Return a context manager that tracks errors emitted by this Context" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject ContextType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupContextType() {
  ContextType.tp_name = "Context";
  ContextType.tp_basicsize = sizeof(ContextObject);
  ContextType.tp_itemsize = 0;
  ContextType.tp_dealloc = (destructor) ContextObject_dealloc;
  ContextType.tp_flags = Py_TPFLAGS_DEFAULT;
  ContextType.tp_doc = PyDoc_STR("The Chapel context object that tracks various frontend state");
  ContextType.tp_methods = ContextObject_methods;
  ContextType.tp_init = (initproc) ContextObject_init;
  ContextType.tp_new = PyType_GenericNew;
}

int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs) {
  Context::Configuration config;
  config.chplHome = getenv("CHPL_HOME");
  new (&self->context) Context(std::move(config));
  self->context.installErrorHandler(owned<PythonErrorHandler>(new PythonErrorHandler((PyObject*) self)));

  parsing::setupModuleSearchPaths(&self->context, false, false, {}, {});

  return 0;
}

void ContextObject_dealloc(ContextObject* self) {
  self->context.~Context();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ContextObject_parse(ContextObject *self, PyObject* args) {
  auto context = &self->context;
  const char* fileName;
  if (!PyArg_ParseTuple(args, "s", &fileName)) {
    PyErr_BadArgument();
    return nullptr;
  }
  auto fileNameUS = UniqueString::get(context, fileName);
  auto parentPathUS = UniqueString();
  auto& builderResult = parsing::parseFileToBuilderResultAndCheck(context, fileNameUS, parentPathUS);

  int listSize = builderResult.numTopLevelExpressions();
  PyObject* topExprs = PyList_New(listSize);
  for (auto i = 0; i < listSize; i++) {
    PyObject* node = wrapAstNode(self, builderResult.topLevelExpression(i));
    PyList_SetItem(topExprs, i, node);
  }
  return topExprs;
}

PyObject* ContextObject_is_bundled_path(ContextObject *self, PyObject* args) {
  auto context = &self->context;
  const char* fileName;
  if (!PyArg_ParseTuple(args, "s", &fileName)) {
    PyErr_BadArgument();
    return nullptr;
  }
  auto pathUS = UniqueString::get(context, fileName);

  bool isInternalPath =
    parsing::filePathIsInInternalModule(context, pathUS) ||
    parsing::filePathIsInStandardModule(context, pathUS) ||
    parsing::filePathIsInBundledModule(context, pathUS);

  return PyBool_FromLong(isInternalPath);
}

PyObject* ContextObject_advance_to_next_revision(ContextObject *self, PyObject* args) {
  auto context = &self->context;
  bool prepareToGc;
  if (!PyArg_ParseTuple(args, "b", &prepareToGc)) {
    PyErr_BadArgument();
    return nullptr;
  }

  context->advanceToNextRevision(prepareToGc);
  parsing::setupModuleSearchPaths(&self->context, false, false, {}, {});

  Py_RETURN_NONE;
}

template <typename Tuple, size_t ... Indices>
static void printTypedPythonFunctionArgs(std::ostringstream& ss, std::index_sequence<Indices...>) {
  // std::index_sequence is an empty object that only serves to contain a list
  // of size_ts in a parameter back. By writing template functions with
  // template <size_t ... Indices>, and accepting a std::index_sequence argument,
  // we are able to get a compile-time handle on a variadic list of indices --
  // in this case, a variadic list of tuple indices. That's what this
  // function does.
  //
  // From there, we can use variadic template expansion to print the TypeString
  // corresponding to each element / index of the tuple. If we just wanted to
  // print the TypeStrings without spaces or punctuation, we could have used
  // (<<) with a fold expression[1]. However, we want to print a comma and
  // more, so it's more convenient to use a wrapper function printArg to handle
  // the formatting.
  //
  // [1]: https://en.cppreference.com/w/cpp/language/fold

  int counter = 0;
  auto printArg = [&](const char* arg) {
    ss << ", arg" << counter++ << ": " << arg;
  };

  (printArg(std::tuple_element<Indices, Tuple>::type::TypeString), ...);
}


/** Same as the table in AstTag.cpp, except this one doesn't print
    the START_ and END_ prefixes for tags. This way, we can get user-readable
    names for abstract base classes. */
static const char* tagToUserFacingStringTable[asttags::NUM_AST_TAGS] = {
// define tag to string conversion
#define AST_NODE(NAME) #NAME,
#define AST_LEAF(NAME) #NAME,
#define AST_BEGIN_SUBCLASSES(NAME) #NAME,
#define AST_END_SUBCLASSES(NAME) #NAME,
// Apply the above macros to uast-classes-list.h
#include "chpl/uast/uast-classes-list.h"
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef NAMESTR
};

PyObject* ContextObject_get_pyi_file(ContextObject *self, PyObject* args) {
  std::ostringstream ss;

  ss << "from typing import *" << std::endl << std::endl;

  ss << "class AstNode:" << std::endl;
  ss << "    pass" << std::endl << std::endl;

  // Here, use X-macros with the method-tables.h header file to generate
  // printing code for each AST node class. This uses the helper function
  // printTypedPythonFunctionArgs (explained in its body) for the arguments.
  //
  // We get a tuple of type information structs from the PythonFnHelper template
  // and each method's TYPEFN (a C++ type in the form R(Args...)). The documentation
  // for PythonFnHelper has some more information on this.

  #define CLASS_BEGIN(NODE) \
    ss << "class " << tagToUserFacingStringTable[asttags::NODE] << "("; \
    ss << parentTypeFor(asttags::NODE)->tp_name; \
    ss << "):" << std::endl;
  #define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY) \
    ss << "    def " << #NAME << "(self"; \
    printTypedPythonFunctionArgs<PythonFnHelper<TYPEFN>::ArgTypeInfo>(ss, std::make_index_sequence<std::tuple_size<PythonFnHelper<TYPEFN>::ArgTypeInfo>::value>()); \
    ss << ") -> " << PythonFnHelper<TYPEFN>::ReturnTypeInfo::TypeString << ":" << std::endl;\
    ss << "        \"\"\"" << std::endl; \
    ss << "        " << DOCSTR << std::endl; \
    ss << "        \"\"\"" << std::endl; \
    ss << "        ..." << std::endl << std::endl;
  #define CLASS_END(NODE) \
    ss << std::endl;
  #include "method-tables.h"

  return Py_BuildValue("s", ss.str().c_str());
}

PyObject* ContextObject_track_errors(ContextObject *self, PyObject* args) {
  auto errorManagerObjectPy = PyObject_CallObject((PyObject *) &ErrorManagerType, nullptr);
  auto errorManagerObject = (ErrorManagerObject*) errorManagerObjectPy;
  Py_INCREF(self);
  errorManagerObject->contextObject = (PyObject*) self;
  return errorManagerObjectPy;
}

static PyMethodDef LocationObject_methods[] = {
  { "start", (PyCFunction) LocationObject_start, METH_VARARGS, "Get the start of a Location object" },
  { "end", (PyCFunction) LocationObject_end, METH_VARARGS, "Get the end of a Location object" },
  { "path", (PyCFunction) LocationObject_path, METH_VARARGS, "Get the path of a Location object" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject LocationType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupLocationType() {
  LocationType.tp_name = "Location";
  LocationType.tp_basicsize = sizeof(LocationObject);
  LocationType.tp_itemsize = 0;
  LocationType.tp_dealloc = (destructor) LocationObject_dealloc;
  LocationType.tp_flags = Py_TPFLAGS_DEFAULT;
  LocationType.tp_doc = PyDoc_STR("The Chapel context object that tracks various frontend state");
  LocationType.tp_methods = LocationObject_methods;
  LocationType.tp_init = (initproc) LocationObject_init;
  LocationType.tp_new = PyType_GenericNew;
}

int LocationObject_init(LocationObject* self, PyObject* args, PyObject* kwargs) {
  new (&self->location) Location();
  return 0;
}

void LocationObject_dealloc(LocationObject* self) {
  self->location.~Location();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* LocationObject_start(LocationObject *self, PyObject* Py_UNUSED(args)) {
  auto& location = self->location;
  return Py_BuildValue("ii", location.firstLine(), location.firstColumn());
}

PyObject* LocationObject_end(LocationObject *self, PyObject* Py_UNUSED(args)) {
  auto& location = self->location;
  return Py_BuildValue("ii", location.lastLine(), location.lastColumn());
}

PyObject* LocationObject_path(LocationObject *self, PyObject* Py_UNUSED(args)) {
  return Py_BuildValue("s", self->location.path().c_str());
}

static PyMethodDef AstNodeObject_methods[] = {
  {"dump", (PyCFunction) AstNodeObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {"tag", (PyCFunction) AstNodeObject_tag, METH_NOARGS, "Get a string representation of the AST node's type"},
  {"attribute_group", (PyCFunction) AstNodeObject_attribute_group, METH_NOARGS, "Get the attribute group, if any, associated with this node"},
  {"location", (PyCFunction) AstNodeObject_location, METH_NOARGS, "Get the location of this AST node in its file"},
  {"parent", (PyCFunction) AstNodeObject_parent, METH_NOARGS, "Get the parent node of this AST node"},
  {"pragmas", (PyCFunction) AstNodeObject_pragmas, METH_NOARGS, "Get the pragmas of this AST node"},
  {"unique_id", (PyCFunction) AstNodeObject_unique_id, METH_NOARGS, "Get a unique identifer for this AST node"},
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyTypeObject AstNodeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupAstNodeType() {
  AstNodeType.tp_name = "AstNode";
  AstNodeType.tp_basicsize = sizeof(AstNodeObject);
  AstNodeType.tp_itemsize = 0;
  AstNodeType.tp_dealloc = (destructor) AstNodeObject_dealloc;
  AstNodeType.tp_flags = Py_TPFLAGS_BASETYPE;
  AstNodeType.tp_doc = PyDoc_STR("The base type of Chapel AST nodes");
  AstNodeType.tp_iter = (getiterfunc) AstNodeObject_iter;
  AstNodeType.tp_methods = AstNodeObject_methods;
  AstNodeType.tp_init = (initproc) AstNodeObject_init;
  AstNodeType.tp_new = PyType_GenericNew;
}

int AstNodeObject_init(AstNodeObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
      return -1;

  Py_INCREF(contextObjectPy);
  self->astNode = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

void AstNodeObject_dealloc(AstNodeObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* AstNodeObject_dump(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  self->astNode->dump();
  Py_RETURN_NONE;
}

PyObject* AstNodeObject_tag(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  const char* nodeType = asttags::tagToString(self->astNode->tag());
  return Py_BuildValue("s", nodeType);
}

PyObject* AstNodeObject_unique_id(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  auto uniqueID = (intptr_t)self->astNode;
  return Py_BuildValue("K", uniqueID);
}


PyObject* AstNodeObject_attribute_group(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  return wrapAstNode((ContextObject*) self->contextObject,
                     self->astNode->attributeGroup());
}

PyObject* AstNodeObject_pragmas(AstNodeObject *self, PyObject *Py_UNUSED(ignored)) {
  PyObject* elms = PySet_New(NULL);
  auto attrs = self->astNode->attributeGroup();
  if (attrs) {
    for (auto p: attrs->pragmas()) {
      PyObject* s = Py_BuildValue("s", pragmatags::pragmaTagToName(p));
      PySet_Add(elms, s);
    }
  }
  return elms;
}

PyObject* AstNodeObject_parent(AstNodeObject* self, PyObject *Py_UNUSED(ignored)) {
  auto contextObject = (ContextObject*) self->contextObject;
  auto context = &contextObject->context;

  return wrapAstNode(contextObject, parsing::parentAst(context, self->astNode));
}

PyObject* AstNodeObject_iter(AstNodeObject *self) {
  return wrapIterPair((ContextObject*) self->contextObject, self->astNode->children());
}

PyObject* AstNodeObject_location(AstNodeObject *self) {
  auto locationObjectPy = PyObject_CallObject((PyObject *) &LocationType, nullptr);
  auto& location = ((LocationObject*) locationObjectPy)->location;
  auto context = &((ContextObject*) self->contextObject)->context;

  location = parsing::locateAst(context, self->astNode);
  return locationObjectPy;
}


PyTypeObject* parentTypeFor(asttags::AstTag tag) {
#define AST_NODE(NAME)
#define AST_LEAF(NAME)
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME) \
  if (tag > asttags::START_##NAME && tag < asttags::END_##NAME) { \
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

PyObject* wrapAstNode(ContextObject* context, const AstNode* node) {
  PyObject* toReturn = nullptr;
  if (node == nullptr) {
    Py_RETURN_NONE;
  }
  PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
  switch (node->tag()) {
#define CAST_TO(NAME) \
    case asttags::NAME: \
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
