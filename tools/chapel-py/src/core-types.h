/*
 * Copyright 2021-2026 Hewlett Packard Enterprise Development LP
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

#include "PythonWrapper.h"
#include "chpl/framework/Context.h"
#include "chpl/uast/AstTag.h"
#include "error-tracker.h"
#include "python-class.h"

/* Tiny helper class to support marshaling to and from Python types.
   This wraps regular pointers. However, during marshaling, instead of
   throwing Python exceptions when the underlying pointer is 'null', it
   returns None. */
template <typename T>
struct Nilable {
  T value;

  Nilable() : value(nullptr) {}
  Nilable(T value) : value(value) {}
};


PyTypeObject* parentTypeFor(chpl::uast::asttags::AstTag tag);
PyTypeObject* parentTypeFor(chpl::types::typetags::TypeTag tag);
PyTypeObject* parentTypeFor(chpl::types::paramtags::ParamTag tag);
PyTypeObject* parentTypeFor(chpl::ErrorType tag);

using LineColumnPair = std::tuple<int, int>;

struct LocationObject : public PythonClass<LocationObject, chpl::Location> {
  static constexpr const char* QualifiedName = "chapel.Location";
  static constexpr const char* Name = "Location";
  static constexpr const char* DocStr = "An object that represents the location of an AST node in a source file.";

  static PyObject* add(LocationObject* self, PyObject* other) {
    if (other->ob_type != LocationObject::PythonType) {
      Py_RETURN_NOTIMPLEMENTED;
    }
    auto otherCast = (LocationObject*) other;

    if (self->value_.path() != otherCast->value_.path()) {
      // raise value error
      PyErr_SetString(PyExc_ValueError, "Cannot add locations from different files");
    }

    auto newLoc =
      chpl::Location(self->value_.path(),
                    std::min(self->value_.start(), otherCast->value_.start()),
                    std::max(self->value_.end(), otherCast->value_.end()));
    return (PyObject*) LocationObject::create(newLoc);
  }
  static PyObject* iadd(LocationObject* self, PyObject* other) {
    return add(self, other);
  }

  static PyObject* subtract(LocationObject* self, PyObject* other) {
    if (other->ob_type != LocationObject::PythonType) {
      Py_RETURN_NOTIMPLEMENTED;
    }
    auto otherCast = (LocationObject*) other;

    if (self->value_.path() != otherCast->value_.path()) {
      // raise value error
      PyErr_SetString(PyExc_ValueError, "Cannot subtract locations from different files");
    }

    /*
    if (B.end < A.start) and (B.start > A.end):
      return A
    elif (B.start <= A.start) and (A.start <= B.end):
      return B.end..A.end
    else
      return min(A.start, B.start)..min(A.end, B.start)
    */

    auto A_start = self->value_.start();
    auto A_end = self->value_.end();
    auto B_start = otherCast->value_.start();
    auto B_end = otherCast->value_.end();

    if (B_end < A_start && B_start > A_end) {
      return (PyObject*)self;
    } else if (B_start <= A_start && A_start <= B_end) {
      auto newLoc = chpl::Location(self->value_.path(), B_end, A_end);
      return (PyObject*) LocationObject::create(newLoc);
    } else {
      auto newLoc = chpl::Location(self->value_.path(), A_start, B_start);
      return (PyObject*) LocationObject::create(newLoc);
    }
  }
  static PyObject* isub(LocationObject* self, PyObject* other) {
    return subtract(self, other);
  }

  static PyObject* str(LocationObject* self) {
    return PyUnicode_FromFormat("%s:%d:%d-%d:%d",
      self->value_.path().c_str(),
      self->value_.firstLine(),
      self->value_.firstColumn(),
      self->value_.lastLine(),
      self->value_.lastColumn());
  }

  static PyTypeObject* configurePythonType() {
    // Configure the necessary methods to make inserting into sets working:

    std::array<PyType_Slot, 5> extraSlots = {
      PyType_Slot{Py_tp_str, (void*) str},
      {Py_nb_add, (void*) add},
      {Py_nb_inplace_add, (void*) iadd},
      {Py_nb_subtract, (void*) subtract},
      {Py_nb_inplace_subtract, (void*) isub},
    };
    PyTypeObject* configuring = PythonClassWithContext<LocationObject, chpl::Location>::configurePythonType(Py_TPFLAGS_DEFAULT, extraSlots);
    return configuring;
  }
};

struct ScopeObject : public PythonClassWithContext<ScopeObject, const chpl::resolution::Scope*> {
  static constexpr const char* QualifiedName = "chapel.Scope";
  static constexpr const char* Name = "Scope";
  static constexpr const char* DocStr = "A scope in the Chapel program, such as a block.";
};

using VisibleSymbol = std::tuple<chpl::UniqueString, std::vector<const chpl::uast::AstNode*>>;

struct AstNodeObject : public PythonClassWithContext<AstNodeObject, const chpl::uast::AstNode*> {
  static constexpr const char* QualifiedName = "chapel.AstNode";
  static constexpr const char* Name = "AstNode";
  static constexpr const char* DocStr = "The base type of Chapel AST nodes";

  static PyObject* iter(AstNodeObject *self);
  static PyObject* str(AstNodeObject *self);
  static PyObject* repr(AstNodeObject *self);
  static Py_hash_t hash(AstNodeObject *self);
  static PyObject* richcompare(AstNodeObject *self, PyObject *other, int op);

  static PyTypeObject* configurePythonType() {
    std::array<PyType_Slot, 5> extraSlots = {
      PyType_Slot{Py_tp_str, (void*) AstNodeObject::str},
      PyType_Slot{Py_tp_repr, (void*) AstNodeObject::repr},
      PyType_Slot{Py_tp_iter, (void*) AstNodeObject::iter},
      PyType_Slot{Py_tp_hash, (void*) AstNodeObject::hash},
      PyType_Slot{Py_tp_richcompare, (void*) AstNodeObject::richcompare},
    };
    PyTypeObject* configuring = PythonClassWithContext<AstNodeObject, const chpl::uast::AstNode*>::configurePythonType(Py_TPFLAGS_BASETYPE, extraSlots);
    return configuring;
  }
};

using QualifiedTypeTuple = std::tuple<const char*, Nilable<const chpl::types::Type*>, Nilable<const chpl::types::Param*>>;

struct ChapelTypeObject  : public PythonClassWithContext<ChapelTypeObject, const chpl::types::Type*> {
  static constexpr const char* QualifiedName = "chapel.ChapelType";
  static constexpr const char* Name = "ChapelType";
  static constexpr const char* DocStr = "The base type of Chapel types";

  static PyObject* str(ChapelTypeObject* self);

  static PyTypeObject* configurePythonType() {
    std::array<PyType_Slot, 1> extraSlots = {
      PyType_Slot{Py_tp_str, (void*) ChapelTypeObject::str},
    };
    PyTypeObject* configuring = PythonClassWithContext<ChapelTypeObject, const chpl::types::Type*>::configurePythonType(Py_TPFLAGS_BASETYPE, extraSlots);
    return configuring;
  }
};

struct ParamObject : public PythonClassWithContext<ParamObject, const chpl::types::Param*> {
  static constexpr const char* QualifiedName = "chapel.Param";
  static constexpr const char* Name = "Param";
  static constexpr const char* DocStr = "The base type of Chapel parameters (compile-time known values)";

  static PyObject* str(ParamObject* self);

  static PyTypeObject* configurePythonType() {
    std::array<PyType_Slot, 1> extraSlots = {
      PyType_Slot{Py_tp_str, (void*) ParamObject::str},
    };
    PyTypeObject* configuring = PythonClassWithContext<ParamObject, const chpl::types::Param*>::configurePythonType(Py_TPFLAGS_BASETYPE, extraSlots);
    return configuring;
  }
};

struct ResolvedExpressionObject : public PythonClassWithContext<ResolvedExpressionObject, const chpl::resolution::ResolvedExpression*> {
  static constexpr const char* QualifiedName = "chapel.ResolvedExpression";
  static constexpr const char* Name = "ResolvedExpression";
  static constexpr const char* DocStr = "Container for type information about a particular AST node.";
};

// Return a MostSpecificCandidate with its POI scope, so that we can call 'resolve'
// on it and get the correct result.
struct MostSpecificCandidateAndPoiScope {
  const chpl::resolution::MostSpecificCandidate* candidate;
  const chpl::resolution::PoiScope* poiScope;
};

struct MostSpecificCandidateObject : public PythonClassWithContext<MostSpecificCandidateObject, MostSpecificCandidateAndPoiScope> {
  static constexpr const char* QualifiedName = "chapel.MostSpecificCandidate";
  static constexpr const char* Name = "MostSpecificCandidate";
  static constexpr const char* DocStr = "A candidate function returned from call resolution that represents the most specific overload matching the call.";
};

// Same as MostSpecificCandidate: include the POI scope in the bundle, so that
// we immediately have all the info to resolve the function.
struct TypedSignatureAndPoiScope {
  const chpl::resolution::TypedFnSignature* signature;
  const chpl::resolution::PoiScope* poiScope;
};

struct TypedSignatureObject : public PythonClassWithContext<TypedSignatureObject, TypedSignatureAndPoiScope> {
  static constexpr const char* QualifiedName = "chapel.TypedSignature";
  static constexpr const char* Name = "TypedSignature";
  static constexpr const char* DocStr = "The signature of a particular function. Could include types gathered when instantiating the function";

  static PyObject* str(TypedSignatureObject* self);
  static PyObject* repr(TypedSignatureObject* self);
  static Py_hash_t hash(TypedSignatureObject* self);
  static PyObject* richcompare(TypedSignatureObject* self, PyObject* other, int op);

  static PyTypeObject* configurePythonType() {
    // Configure the necessary methods to make inserting into sets working:
    std::array<PyType_Slot, 4> extraSlots = {
      PyType_Slot{Py_tp_str, (void*) TypedSignatureObject::str},
      PyType_Slot{Py_tp_repr, (void*) TypedSignatureObject::repr},
      PyType_Slot{Py_tp_hash, (void*) TypedSignatureObject::hash},
      PyType_Slot{Py_tp_richcompare, (void*) TypedSignatureObject::richcompare},
    };
    PyTypeObject* configuring = PythonClassWithContext<TypedSignatureObject, TypedSignatureAndPoiScope>::configurePythonType(Py_TPFLAGS_DEFAULT, extraSlots);
    return configuring;
  }
};

struct ApplicabilityResultObject : public PythonClassWithContext<ApplicabilityResultObject, chpl::resolution::ApplicabilityResult> {
  static constexpr const char* QualifiedName = "chapel.ApplicabilityResult";
  static constexpr const char* Name = "ApplicabilityResult";
  static constexpr const char* DocStr = "The result of checking whether a particular function candidate is applicable to a call.";
};

struct CallInfoObject : public PythonClassWithContext<CallInfoObject, chpl::resolution::CallInfo> {
  static constexpr const char* QualifiedName = "chapel.CallInfo";
  static constexpr const char* Name = "CallInfo";
  static constexpr const char* DocStr = "Information about a particular call, including the actuals usded for the resolution";
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
PyObject* wrapGeneratedType(ContextObject* context, const chpl::uast::AstNode* node);

/**
  Create a Python object of the class corresponding to the given Type*.
  For example, an ArrayType type will be wrapped in a chapel.ArrayType.
 */
PyObject* wrapGeneratedType(ContextObject* context, const chpl::types::Type* node);

/**
  Creates a Python object of the class corresponding to the given Param*.
 */
PyObject* wrapGeneratedType(ContextObject* context, const chpl::types::Param* node);

/**
  Creates a Python object of the class corresponding to the given ErrorBase*.

  Note: unlike all other generated objects at the type of writing, Errors
  aren't owned by the context (and borrowed by Python). Instead, they are
  owned by the Python object that contains them. So, here, we clone them.
 */
PyObject* wrapGeneratedType(ContextObject* context, const chpl::ErrorBase* node);

#endif
