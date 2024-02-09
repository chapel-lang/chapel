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

PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);
PyTypeObject* parentTypeFor(chpl::types::typetags::TypeTag tag);
PyTypeObject* parentTypeFor(chpl::types::paramtags::ParamTag tag);

struct ContextObject {
  PyObject_HEAD
  chpl::Context context_;

  static constexpr const char* Name = "Context";

  chpl::Context* unwrap() { return &context_; }
  ContextObject* context() { return this; }
};
extern PyTypeObject ContextType;
void setupContextType();

int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs);
void ContextObject_dealloc(ContextObject* self);
PyObject* ContextObject_parse(ContextObject *self, PyObject* args);
PyObject* ContextObject_set_module_paths(ContextObject *self, PyObject* args);
PyObject* ContextObject_introspect_parsed_files(ContextObject *self, PyObject* args);
PyObject* ContextObject_is_bundled_path(ContextObject *self, PyObject* args);
PyObject* ContextObject_advance_to_next_revision(ContextObject *self, PyObject* args);
PyObject* ContextObject_get_pyi_file(ContextObject *self, PyObject* args);
PyObject* ContextObject_track_errors(ContextObject *self, PyObject* args);

struct LocationObject {
  PyObject_HEAD
  chpl::Location location;

  static constexpr const char* Name = "Location";

  chpl::Location* unwrap() { return &location; }
  ContextObject* context() { return nullptr; }
};
extern PyTypeObject LocationType;
void setupLocationType();

using LineColumnPair = std::tuple<int, int>;

int LocationObject_init(LocationObject* self, PyObject* args, PyObject* kwargs);
void LocationObject_dealloc(LocationObject* self);
PyObject* LocationObject_start(LocationObject *self, PyObject* Py_UNUSED(args));
PyObject* LocationObject_end(LocationObject *self, PyObject* Py_UNUSED(args));
PyObject* LocationObject_path(LocationObject *self, PyObject* Py_UNUSED(args));

struct ScopeObject {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::resolution::Scope* scope;

  static constexpr const char* Name = "Scope";

  const chpl::resolution::Scope* unwrap() { return scope; }
  ContextObject* context() { return (ContextObject*) contextObject; }
};
extern PyTypeObject ScopeType;
void setupScopeType();

int ScopeObject_init(ScopeObject* self, PyObject* args, PyObject* kwargs);
void ScopeObject_dealloc(ScopeObject* self);
PyObject* ScopeObject_used_imported_modules(ScopeObject *self, PyObject* Py_UNUSED(args));

struct AstNodeObject {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::uast::AstNode* ptr;

  static constexpr const char* Name = "AstNode";

  const chpl::uast::AstNode* unwrap() { return ptr; }
  ContextObject* context() { return (ContextObject*) contextObject; }
};
extern PyTypeObject AstNodeType;
void setupAstNodeType();

using QualifiedTypeTuple = std::tuple<const char*, const chpl::types::Type*, const chpl::types::Param*>;

int AstNodeObject_init(AstNodeObject* self, PyObject* args, PyObject* kwargs);
void AstNodeObject_dealloc(AstNodeObject* self);
PyObject* AstNodeObject_dump(AstNodeObject *self);
PyObject* AstNodeObject_tag(AstNodeObject *self);
PyObject* AstNodeObject_unique_id(AstNodeObject *self);
PyObject* AstNodeObject_attribute_group(AstNodeObject *self);
PyObject* AstNodeObject_pragmas(AstNodeObject *self);
PyObject* AstNodeObject_parent(AstNodeObject* self);
PyObject* AstNodeObject_iter(AstNodeObject *self);
PyObject* AstNodeObject_location(AstNodeObject *self);
PyObject* AstNodeObject_scope(AstNodeObject *self);
PyObject* AstNodeObject_type(AstNodeObject *self);
PyObject* AstNodeObject_called_fn(AstNodeObject *self);

struct ChapelTypeObject {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::types::Type* ptr;
};
extern PyTypeObject ChapelTypeType;
void setupChapelTypeType();

int ChapelTypeObject_init(ChapelTypeObject* self, PyObject* args, PyObject* kwargs);
void ChapelTypeObject_dealloc(ChapelTypeObject* self);
PyObject* ChapelTypeObject_str(ChapelTypeObject* self);

struct ParamObject {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::types::Param* ptr;
};
extern PyTypeObject ParamType;
void setupParamType();

int ParamObject_init(ParamObject* self, PyObject* args, PyObject* kwargs);
void ParamObject_dealloc(ParamObject* self);
PyObject* ParamObject_str(ParamObject* self);

template<typename IntentType>
const char* intentToString(IntentType intent) {
  return qualifierToString(chpl::uast::Qualifier(int(intent)));
}

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

/**
  Create a Python object from the given Location.
 */
PyObject* wrapLocation(chpl::Location loc);

#endif
