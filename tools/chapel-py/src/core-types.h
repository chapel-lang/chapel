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

#ifndef CHAPEL_PY_CORE_TYPES_H
#define CHAPEL_PY_CORE_TYPES_H

#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstTag.h"
#include "error-tracker.h"

PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);

typedef struct {
  PyObject_HEAD
  chpl::Context context;
  /* Type-specific fields go here. */
} ContextObject;
extern PyTypeObject ContextType;
void setupContextType();

int ContextObject_init(ContextObject* self, PyObject* args, PyObject* kwargs);
void ContextObject_dealloc(ContextObject* self);
PyObject* ContextObject_parse(ContextObject *self, PyObject* args);
PyObject* ContextObject_is_bundled_path(ContextObject *self, PyObject* args);
PyObject* ContextObject_advance_to_next_revision(ContextObject *self, PyObject* args);
PyObject* ContextObject_get_pyi_file(ContextObject *self, PyObject* args);
PyObject* ContextObject_track_errors(ContextObject *self, PyObject* args);

typedef struct {
  PyObject_HEAD
  chpl::Location location;
} LocationObject;
extern PyTypeObject LocationType;
void setupLocationType();

int LocationObject_init(LocationObject* self, PyObject* args, PyObject* kwargs);
void LocationObject_dealloc(LocationObject* self);
PyObject* LocationObject_start(LocationObject *self, PyObject* Py_UNUSED(args));
PyObject* LocationObject_end(LocationObject *self, PyObject* Py_UNUSED(args));
PyObject* LocationObject_path(LocationObject *self, PyObject* Py_UNUSED(args));

typedef struct {
  PyObject_HEAD
  PyObject* contextObject;
  const chpl::uast::AstNode* astNode;
} AstNodeObject;
extern PyTypeObject AstNodeType;
void setupAstNodeType();

int AstNodeObject_init(AstNodeObject* self, PyObject* args, PyObject* kwargs);
void AstNodeObject_dealloc(AstNodeObject* self);
PyObject* AstNodeObject_dump(AstNodeObject *self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_tag(AstNodeObject *self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_unique_id(AstNodeObject *self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_attribute_group(AstNodeObject *self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_pragmas(AstNodeObject *self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_parent(AstNodeObject* self, PyObject *Py_UNUSED(ignored));
PyObject* AstNodeObject_iter(AstNodeObject *self);
PyObject* AstNodeObject_location(AstNodeObject *self);

/**
  Declare a Python PyTypeObject that corresponds to an AST node with the given
  name and tag. The tag is not the same as the name because abstract base
  classes like NamedDecl have corresponding tags called START_NamedDecl
  and END_NamedDecl, but not NamedDecl.
 */
#define DECLARE_PY_OBJECT_FOR(NAME, TAG)\
  typedef struct { \
    AstNodeObject parent; \
  } NAME##Object; \
  \
  extern PyTypeObject NAME##Type;

/* Generate a Python object for reach AST node type. */
#define AST_NODE(NAME) DECLARE_PY_OBJECT_FOR(NAME, NAME)
#define AST_LEAF(NAME) DECLARE_PY_OBJECT_FOR(NAME, NAME)
#define AST_BEGIN_SUBCLASSES(NAME) DECLARE_PY_OBJECT_FOR(NAME, START_##NAME)
#define AST_END_SUBCLASSES(NAME)
#include "chpl/uast/uast-classes-list.h"
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef DECLARE_PY_OBJECT_FOR

void setupPerNodeTypes();


/**
  Create a Python object of the class corresponding to the given AST node's
  type. For example, an Identifier node will be wrapped in a a chapel.Identifier.
 */
PyObject* wrapAstNode(ContextObject* context, const chpl::uast::AstNode* node);

#endif
