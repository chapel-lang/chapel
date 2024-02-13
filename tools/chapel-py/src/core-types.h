/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHAPEL_PY_CORE_TYPES_H
#define CHAPEL_PY_CORE_TYPES_H

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstTag.h"
#include "error-tracker.h"
#include "python-class.h"

PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);
PyTypeObject* parentTypeFor(chpl::types::typetags::TypeTag tag);
PyTypeObject* parentTypeFor(chpl::types::paramtags::ParamTag tag);

struct LocationObject : public PythonClass<LocationObject, chpl::Location> {
  static constexpr const char* Name = "Location";
  static constexpr const char* DocStr = "An object that represents the location of an AST node in a source file.";
};

using LineColumnPair = std::tuple<int, int>;

struct ScopeObject : public PythonClassWithObject<ScopeObject, const chpl::resolution::Scope*> {
  static constexpr const char* Name = "Scope";
  static constexpr const char* DocStr = "A scope in the Chapel program, such as a block.";
};

struct AstNodeObject : public PythonClassWithObject<AstNodeObject, const chpl::uast::AstNode*> {
  static constexpr const char* Name = "AstNode";
  static constexpr const char* DocStr = "The base type of Chapel AST nodes";

  static PyObject* iter(AstNodeObject *self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<AstNodeObject, const chpl::uast::AstNode*>::configurePythonType();
    configuring.tp_iter = (getiterfunc) AstNodeObject::iter;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

using QualifiedTypeTuple = std::tuple<const char*, const chpl::types::Type*, const chpl::types::Param*>;

struct ChapelTypeObject  : public PythonClassWithObject<ChapelTypeObject, const chpl::types::Type*> {
  static constexpr const char* Name = "ChapelType";
  static constexpr const char* DocStr = "The base type of Chapel types";

  static PyObject* str(ChapelTypeObject* self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<ChapelTypeObject, const chpl::types::Type*>::configurePythonType();
    configuring.tp_str = (reprfunc) ChapelTypeObject::str;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

struct ParamObject : public PythonClassWithObject<ParamObject, const chpl::types::Param*> {
  static constexpr const char* Name = "Param";
  static constexpr const char* DocStr = "The base type of Chapel parameters (compile-time known values)";

  static PyObject* str(ParamObject* self);

  static PyTypeObject configurePythonType() {
    PyTypeObject configuring = PythonClassWithObject<ParamObject, const chpl::types::Param*>::configurePythonType();
    configuring.tp_str = (reprfunc) ParamObject::str;
    configuring.tp_flags = Py_TPFLAGS_BASETYPE;
    return configuring;
  }
};

template<typename IntentType>
const char* intentToString(IntentType intent) {
  return qualifierToString(chpl::uast::Qualifier(int(intent)));
}

/**
 Using the various definitions, templates, and method tables we have, generate
 a Python .pyi file describing the various methods and types we provide.
 */
std::string generatePyiFile();

/**
  Create a Python object of the class corresponding to the given AST node's
  type. For example, an Identifier node will be wrapped in a chapel.Identifier.
 */
PyObject* wrapAstNode(ContextObject* context, const chpl::uast::AstNode* node);

/**
  Create a Python object of the class corresponding to the given Type*.
  For example, an ArrayType type will be wrapped in a chapel.ArrayType.
 */
PyObject* wrapType(ContextObject* context, const chpl::types::Type* node);

/**
  Creates a Python object of the class corresponding to the given Param*.
 */
PyObject* wrapParam(ContextObject* context, const chpl::types::Param* node);

#endif
