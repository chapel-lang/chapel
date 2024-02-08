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

#include "core-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/types/all-types.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "python-types.h"
#include "error-tracker.h"
#include "resolution.h"

using namespace chpl;
using namespace uast;

static PyMethodDef ContextObject_methods[] = {
  { "parse", (PyCFunction) ContextObject_parse, METH_VARARGS, "Parse a top-level AST node from the given file" },
  { "set_module_paths", (PyCFunction) ContextObject_set_module_paths, METH_VARARGS, "Set the module path arguments to the given lists of module paths and filenames" },
  { "introspect_parsed_files", (PyCFunction) ContextObject_introspect_parsed_files, METH_VARARGS, "Inspect the list of files that have been parsed by the Context" },
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
  new (&self->context_) Context(std::move(config));
  self->context_.installErrorHandler(owned<PythonErrorHandler>(new PythonErrorHandler((PyObject*) self)));

  return 0;
}

void ContextObject_dealloc(ContextObject* self) {
  self->context_.~Context();
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ContextObject_parse(ContextObject *self, PyObject* args) {
  auto context = &self->context_;
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

static void extractListOfStrings(PyObject* list, std::vector<std::string>& into) {
  for (int i = 0; i < PyList_Size(list); i++) {
    PyObject* pathObject = PyList_GetItem(list, i);
    if (!PyUnicode_Check(pathObject)) {
      PyErr_BadArgument();
      return;
    }
    into.push_back(PyUnicode_AsUTF8(pathObject));
  }
}

PyObject* ContextObject_set_module_paths(ContextObject *self, PyObject* args) {
  auto context = &self->context_;
  std::vector<std::string> paths;
  std::vector<std::string> filenames;

  PyObject* pathsObject = nullptr;
  PyObject* filenamesObject = nullptr;

  if (!PyArg_ParseTuple(args, "OO", &pathsObject, &filenamesObject)) {
    PyErr_BadArgument();
    return nullptr;
  }

  extractListOfStrings(pathsObject, paths);
  extractListOfStrings(filenamesObject, filenames);
  parsing::setupModuleSearchPaths(context, false, false, paths, filenames);

  Py_RETURN_NONE;
}

PyObject* ContextObject_introspect_parsed_files(ContextObject *self, PyObject* args) {
  auto context = &self->context_;

  auto parsedFiles = parsing::introspectParsedFiles(context);
  PyObject* destinationList = PyList_New(parsedFiles.size());

  for (size_t i = 0; i < parsedFiles.size(); i++) {
    PyList_SetItem(destinationList, i, Py_BuildValue("s", parsedFiles[i].c_str()));
  }

  return destinationList;
}

PyObject* ContextObject_is_bundled_path(ContextObject *self, PyObject* args) {
  auto context = &self->context_;
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
  auto context = &self->context_;
  bool prepareToGc;
  if (!PyArg_ParseTuple(args, "b", &prepareToGc)) {
    PyErr_BadArgument();
    return nullptr;
  }

  context->advanceToNextRevision(prepareToGc);
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
  // From there, we can use variadic template expansion to print the typeString
  // corresponding to each element / index of the tuple. If we just wanted to
  // print the typeStrings without spaces or punctuation, we could have used
  // (<<) with a fold expression[1]. However, we want to print a comma and
  // more, so it's more convenient to use a wrapper function printArg to handle
  // the formatting.
  //
  // [1]: https://en.cppreference.com/w/cpp/language/fold

  int counter = 0;
  auto printArg = [&](const std::string& arg) {
    ss << ", arg" << counter++ << ": " << arg;
  };

  (printArg(std::tuple_element<Indices, Tuple>::type::typeString()), ...);
}

template <typename T>
struct ParentTypeInfo {
  static PyTypeObject* parentTypeObject() { return nullptr; }
};

#define GENERATED_TYPE(ROOT, NAME, TAG, FLAGS) \
  template <> \
  struct ParentTypeInfo<NAME##Object> { \
    static PyTypeObject* parentTypeObject() { \
      return parentTypeFor(TAG); \
    } \
  };
#include "generated-types-list.h"

PyObject* ContextObject_get_pyi_file(ContextObject *self, PyObject* args) {
  std::ostringstream ss;

  ss << "\"\"\"" << std::endl;
  ss << "AUTOGENERATED FILE - DO NOT EDIT" << std::endl;
  ss << "\"\"\"" << std::endl << std::endl;

  ss << "import typing" << std::endl << std::endl;

  // these get replaced with `scripts/generate-pyi.py`
  ss << "class Context: pass" << std::endl << std::endl;
  ss << "class Location: pass" << std::endl << std::endl;
  ss << "class ErrorManager: pass" << std::endl << std::endl;
  ss << "class Error: pass" << std::endl << std::endl;
  ss << "class Scope: pass" << std::endl << std::endl;
  ss << "class AstNode: pass" << std::endl << std::endl;
  ss << "class ChapelType: pass" << std::endl << std::endl;
  ss << "class Param: pass" << std::endl << std::endl;

  // Here, use X-macros with the method-tables.h header file to generate
  // printing code for each AST node class. This uses the helper function
  // printTypedPythonFunctionArgs (explained in its body) for the arguments.
  //
  // We get a tuple of type information structs from the PythonFnHelper template
  // and each method's TYPEFN (a C++ type in the form R(Args...)). The documentation
  // for PythonFnHelper has some more information on this.

  std::unordered_set<std::string> generated;

  #define CLASS_BEGIN(NODE) \
    ss << "class " << NODE##Object::Name; \
    generated.insert(NODE##Object::Name); \
    if (auto parentType = ParentTypeInfo<NODE##Object>::parentTypeObject()) { \
      ss << "(" << parentType->tp_name << "):"; \
    } \
    ss << std::endl;
  #define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY) \
    ss << "    def " << #NAME << "(self"; \
    printTypedPythonFunctionArgs<PythonFnHelper<TYPEFN>::ArgTypeInfo>(ss, std::make_index_sequence<std::tuple_size<PythonFnHelper<TYPEFN>::ArgTypeInfo>::value>()); \
    ss << ") -> " << PythonFnHelper<TYPEFN>::ReturnTypeInfo::typeString() << ":" << std::endl;\
    ss << "        \"\"\"" << std::endl; \
    ss << "        " << DOCSTR << std::endl; \
    ss << "        \"\"\"" << std::endl; \
    ss << "        ..." << std::endl << std::endl;
  #define CLASS_END(NODE) \
    ss << std::endl;
  #include "method-tables.h"

  #define ENSURE_ALL_CLASSES(NODE, TAG) \
    if(generated.find(NODE##Object::Name) == generated.end()) { \
      ss << "class " << NODE##Object::Name; \
      if (auto parentType = ParentTypeInfo<NODE##Object>::parentTypeObject()) { \
        ss << parentTypeFor(TAG)->tp_name << "):"; \
      } \
      ss << std::endl; \
      ss << "    pass" << std::endl; \
    } \

  #define GENERATED_TYPE(ROOT, NAME, TAG, FLAGS) ENSURE_ALL_CLASSES(NAME, TAG)
  #include "generated-types-list.h"
  #undef ENSURE_ALL_CLASSES

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

static PyMethodDef ScopeObject_methods[] = {
  { "used_imported_modules", (PyCFunction) ScopeObject_used_imported_modules, METH_VARARGS, "Get the modules that were used or imported in this scope" },
  {NULL, NULL, 0, NULL}  /* Sentinel */
};

PyTypeObject ScopeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupScopeType() {
  ScopeType.tp_name = "Scope";
  ScopeType.tp_basicsize = sizeof(ScopeObject);
  ScopeType.tp_itemsize = 0;
  ScopeType.tp_dealloc = (destructor) ScopeObject_dealloc;
  ScopeType.tp_flags = Py_TPFLAGS_DEFAULT;
  ScopeType.tp_doc = PyDoc_STR("A scope in the Chapel program, such as a block.");
  ScopeType.tp_methods = ScopeObject_methods;
  ScopeType.tp_init = (initproc) ScopeObject_init;
  ScopeType.tp_new = PyType_GenericNew;
}

int ScopeObject_init(ScopeObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
      return -1;

  Py_INCREF(contextObjectPy);
  self->scope = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

void ScopeObject_dealloc(ScopeObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ScopeObject_used_imported_modules(ScopeObject* self, PyObject* Py_UNUSED(args)) {
  auto contextObject = ((ContextObject*) self->contextObject);
  auto context = &contextObject->context_;
  auto& moduleIds = resolution::findUsedImportedModules(context, self->scope);

  // Dyno sometimes reports duplicate IDs; ignore them using a set.
  std::set<ID> reportedIds;
  PyObject* modulesList = PyList_New(0);
  for (size_t i = 0; i < moduleIds.size(); i++) {
    auto& id = moduleIds[i];
    if (!reportedIds.insert(id).second) continue;

    auto ast = parsing::idToAst(context, id);
    PyObject* node = wrapAstNode(contextObject, ast);
    PyList_Append(modulesList, node);
  }

  return modulesList;
}

static PyMethodDef AstNodeObject_methods[] = {
  {"dump", (PyCFunction) AstNodeObject_dump, METH_NOARGS, "Dump the internal representation of the given AST node"},
  {"tag", (PyCFunction) AstNodeObject_tag, METH_NOARGS, "Get a string representation of the AST node's type"},
  {"attribute_group", (PyCFunction) AstNodeObject_attribute_group, METH_NOARGS, "Get the attribute group, if any, associated with this node"},
  {"location", (PyCFunction) AstNodeObject_location, METH_NOARGS, "Get the location of this AST node in its file"},
  {"parent", (PyCFunction) AstNodeObject_parent, METH_NOARGS, "Get the parent node of this AST node"},
  {"pragmas", (PyCFunction) AstNodeObject_pragmas, METH_NOARGS, "Get the pragmas of this AST node"},
  {"unique_id", (PyCFunction) AstNodeObject_unique_id, METH_NOARGS, "Get a unique identifier for this AST node"},
  {"scope", (PyCFunction) AstNodeObject_scope, METH_NOARGS, "Get the scope for this AST node"},
  {"type", (PyCFunction) AstNodeObject_type, METH_NOARGS, "Get the type of this AST node"},
  {"called_fn", (PyCFunction) AstNodeObject_called_fn, METH_NOARGS, "Get the function being invoked by this node"},
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
  self->ptr = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

void AstNodeObject_dealloc(AstNodeObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* AstNodeObject_dump(AstNodeObject *self) {
  self->ptr->dump();
  Py_RETURN_NONE;
}

PyObject* AstNodeObject_tag(AstNodeObject *self) {
  const char* nodeType = asttags::tagToString(self->ptr->tag());
  return Py_BuildValue("s", nodeType);
}

PyObject* AstNodeObject_unique_id(AstNodeObject *self) {
  std::stringstream ss;
  self->ptr->id().stringify(ss, CHPL_SYNTAX);
  auto uniqueID = ss.str();
  return Py_BuildValue("s", uniqueID.c_str());
}


PyObject* AstNodeObject_attribute_group(AstNodeObject *self) {
  return wrapAstNode((ContextObject*) self->contextObject,
                     self->ptr->attributeGroup());
}

PyObject* AstNodeObject_pragmas(AstNodeObject *self) {
  PyObject* elms = PySet_New(NULL);
  auto attrs = self->ptr->attributeGroup();
  if (attrs) {
    for (auto p: attrs->pragmas()) {
      PyObject* s = Py_BuildValue("s", pragmatags::pragmaTagToName(p));
      PySet_Add(elms, s);
    }
  }
  return elms;
}

PyObject* AstNodeObject_parent(AstNodeObject* self) {
  auto contextObject = (ContextObject*) self->contextObject;
  auto context = &contextObject->context_;

  return wrapAstNode(contextObject, parsing::parentAst(context, self->ptr));
}

PyObject* AstNodeObject_iter(AstNodeObject *self) {
  return wrapIterPair((ContextObject*) self->contextObject, self->ptr->children());
}

PyObject* AstNodeObject_location(AstNodeObject *self) {
  auto context = &((ContextObject*) self->contextObject)->context_;
  return wrapLocation(parsing::locateAst(context, self->ptr));
}

PyObject* AstNodeObject_scope(AstNodeObject *self) {
  PyObject* args = Py_BuildValue("(O)", self->contextObject);
  auto context = &((ContextObject*) self->contextObject)->context_;
  auto scope = resolution::scopeForId(context, self->ptr->id());

  if (scope == nullptr) {
    Py_RETURN_NONE;
  }

  auto scopeObjectPy = PyObject_CallObject((PyObject *) &ScopeType, args);
  auto scopeObject = (ScopeObject*) scopeObjectPy;
  scopeObject->scope = scope;

  return scopeObjectPy;
}

PyObject* AstNodeObject_type(AstNodeObject *self) {
  auto contextObject = (ContextObject*) self->contextObject;
  auto context = &contextObject->context_;

  auto qt = typeForNode(context, self->ptr);
  if (qt.isUnknown()) {
    Py_RETURN_NONE;
  }

  auto ret = Py_BuildValue("(sOO)", intentToString(qt.kind()),
                                    wrapType(contextObject, qt.type()),
                                    wrapParam(contextObject, qt.param()));

  return ret;
}

PyObject* AstNodeObject_called_fn(AstNodeObject *self) {
  auto contextObject = (ContextObject*) self->contextObject;
  auto context = &contextObject->context_;

  return wrapAstNode(contextObject, calledFnForNode(context, self->ptr));
}

static PyMethodDef ChapelTypeObject_methods[] = {
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyTypeObject ChapelTypeType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupChapelTypeType() {
  ChapelTypeType.tp_name = "ChapelType";
  ChapelTypeType.tp_basicsize = sizeof(ChapelTypeObject);
  ChapelTypeType.tp_itemsize = 0;
  ChapelTypeType.tp_dealloc = (destructor) ChapelTypeObject_dealloc;
  ChapelTypeType.tp_flags = Py_TPFLAGS_BASETYPE;
  ChapelTypeType.tp_doc = PyDoc_STR("The base type of Chapel AST nodes");
  ChapelTypeType.tp_methods = ChapelTypeObject_methods;
  ChapelTypeType.tp_init = (initproc) ChapelTypeObject_init;
  ChapelTypeType.tp_new = PyType_GenericNew;
  ChapelTypeType.tp_str = (reprfunc) ChapelTypeObject_str;
}

int ChapelTypeObject_init(ChapelTypeObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
      return -1;

  Py_INCREF(contextObjectPy);
  self->ptr = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

void ChapelTypeObject_dealloc(ChapelTypeObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ChapelTypeObject_str(ChapelTypeObject* self) {
  std::stringstream ss;
  self->ptr->stringify(ss, CHPL_SYNTAX);
  auto typeString = ss.str();
  return Py_BuildValue("s", typeString.c_str());
}

static PyMethodDef ParamObject_methods[] = {
  {NULL, NULL, 0, NULL} /* Sentinel */
};

PyTypeObject ParamType = {
  PyVarObject_HEAD_INIT(NULL, 0)
};

void setupParamType() {
  ParamType.tp_name = "Param";
  ParamType.tp_basicsize = sizeof(ParamObject);
  ParamType.tp_itemsize = 0;
  ParamType.tp_dealloc = (destructor) ParamObject_dealloc;
  ParamType.tp_flags = Py_TPFLAGS_BASETYPE;
  ParamType.tp_doc = PyDoc_STR("The base type of Chapel AST nodes");
  ParamType.tp_methods = ParamObject_methods;
  ParamType.tp_init = (initproc) ParamObject_init;
  ParamType.tp_new = PyType_GenericNew;
  ParamType.tp_str = (reprfunc) ParamObject_str;
}

int ParamObject_init(ParamObject* self, PyObject* args, PyObject* kwargs) {
  PyObject* contextObjectPy;
  if (!PyArg_ParseTuple(args, "O", &contextObjectPy))
      return -1;

  Py_INCREF(contextObjectPy);
  self->ptr = nullptr;
  self->contextObject = contextObjectPy;
  return 0;
}

void ParamObject_dealloc(ParamObject* self) {
  Py_XDECREF(self->contextObject);
  Py_TYPE(self)->tp_free((PyObject *) self);
}

PyObject* ParamObject_str(ParamObject* self) {
  std::stringstream ss;
  self->ptr->stringify(ss, CHPL_SYNTAX);
  auto typeString = ss.str();
  return Py_BuildValue("s", typeString.c_str());
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
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
  return &AstNodeType;
}

PyTypeObject* parentTypeFor(types::typetags::TypeTag tag) {
#define TYPE_NODE(NAME)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME)
#define TYPE_END_SUBCLASSES(NAME) \
  if (tag > types::typetags::START_##NAME && tag < types::typetags::END_##NAME) { \
    return &NAME##Type; \
  }
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
  return &ChapelTypeType;
}

PyTypeObject* parentTypeFor(chpl::types::paramtags::ParamTag tag) {
  return &ParamType;
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
      ((NAME##Object*) toReturn)->parent.ptr = node->to##NAME(); \
      break;
#define AST_NODE(NAME) CAST_TO(NAME)
#define AST_LEAF(NAME) CAST_TO(NAME)
/* No need to handle abstract parent classes. */
#define AST_BEGIN_SUBCLASSES(NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef CAST_TO
    default: break;
  }
  Py_XDECREF(args);
  return toReturn;
}

PyObject* wrapType(ContextObject* context, const types::Type* node) {
  PyObject* toReturn = nullptr;
  if (node == nullptr) {
    Py_RETURN_NONE;
  }
  PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
  switch (node->tag()) {
#define CAST_TO(NAME) \
    case types::typetags::NAME: \
      toReturn = PyObject_CallObject((PyObject*) &NAME##Type, args); \
      ((NAME##Object*) toReturn)->parent.ptr = (const types::Type*) node->to##NAME(); \
      break;
#define TYPE_NODE(NAME) CAST_TO(NAME)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME) CAST_TO(NAME)
/* No need to handle abstract parent classes. */
#define TYPE_BEGIN_SUBCLASSES(NAME)
#define TYPE_END_SUBCLASSES(NAME)
#include "chpl/types/type-classes-list.h"
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef CAST_TO
    default: break;
  }
  Py_XDECREF(args);
  return toReturn;
}

PyObject* wrapParam(ContextObject* context, const chpl::types::Param* node) {
  PyObject* toReturn = nullptr;
  if (node == nullptr) {
    Py_RETURN_NONE;
  }
  PyObject* args = Py_BuildValue("(O)", (PyObject*) context);
  switch (node->tag()) {
#define PARAM_NODE(NAME, TYPE) \
    case chpl::types::paramtags::NAME: \
      toReturn = PyObject_CallObject((PyObject*) &NAME##Type, args); \
      ((NAME##Object*) toReturn)->parent.ptr = node->to##NAME(); \
      break;
#include "chpl/types/param-classes-list.h"
#undef PARAM_NODE
  }
  Py_XDECREF(args);
  return toReturn;
}

PyObject* wrapLocation(Location loc) {
  auto locationObjectPy = PyObject_CallObject((PyObject *) &LocationType, nullptr);
  auto& location = ((LocationObject*) locationObjectPy)->location;

  location = std::move(loc);
  return locationObjectPy;
}
