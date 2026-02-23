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

#ifndef CHAPEL_PY_PYTHON_TYPES_H
#define CHAPEL_PY_PYTHON_TYPES_H

#include "core-types.h"
#include "iterator-support.h"
#include <optional>
#include <vector>
#include <set>
#include <tuple>
#include "python-type-helper.h"

/* The general idea here is the following: we want to add an allowlist
   of C++ types that have defined conversions to and from Python types.
   We can then use this information to save some boilerplate when generating
   methods (the method author can accept and return the C++ types and we'll
   take care of converting them to/from Python), as well as to generate
   documentation (we know the Python type string for each allowed C++ type).

   To achieve this 'allow list', we use template specialization. By default,
   the PythonReturnTypeInfo struct has no fields, so it would issue a compilation
   error if we tried to access the `wrap` or `unwrap` method (as would happen
   if someone tried to add a method that uses an unsupported type). However,
   we also specialize the template for types that we do support, providing
   the Python type string and `wrap`/`unwrap`. */

template <typename CppType>
struct PythonReturnTypeInfo {};

template <typename CppType, typename = void>
struct HasPythonType : std::false_type {};
template <typename CppType>
struct HasPythonType<CppType, std::void_t<decltype(std::remove_pointer_t<std::remove_reference_t<std::remove_cv_t<CppType>>>::PythonType)>> : std::true_type {};

// Helper template to safely get PythonType when it exists
template<typename T, bool HasType = HasPythonType<T>::value>
struct SafePythonTypeGetter {
    static PyTypeObject* get() { return nullptr; }
};

template<typename T>
struct SafePythonTypeGetter<T, true> {
    using CleanType = std::remove_pointer_t<std::remove_reference_t<std::remove_cv_t<T>>>;
    static PyTypeObject* get() { return CleanType::PythonType; }
};

/** This macro is used to help define template specializations for PythonReturnTypeInfo.
    It just hides some of the boilerplate. */
#define T_DEFINE_INOUT_TYPE(TYPE, TYPESTR, WRAP, UNWRAP) \
  struct PythonReturnTypeInfo<TYPE> { \
    static std::string typeString() { return TYPESTR; } \
  \
    static PyObject* wrap(ContextObject* CONTEXT, const typename std::remove_const<TYPE>::type& TO_WRAP) { \
      return WRAP; \
    } \
  \
    static TYPE unwrap(ContextObject* CONTEXT, PyObject* TO_UNWRAP) { \
      return UNWRAP; \
    } \
  \
    static bool objIsType(PyObject* obj) { \
      if constexpr (HasPythonType<TYPE>::value) { \
        return obj->ob_type == SafePythonTypeGetter<TYPE>::get(); \
      } else { \
        return false; \
      } \
    } \
  }

#define DEFINE_INOUT_TYPE(TYPE, TYPESTR, WRAP, UNWRAP) \
  template <> \
  T_DEFINE_INOUT_TYPE(TYPE, TYPESTR, WRAP, UNWRAP)

namespace detail {
  template <typename Tuple, size_t idx>
  static void unwrapArgHelper(ContextObject* context, PyObject* argTup, Tuple& toModify) {
    using Unwrapper = PythonReturnTypeInfo<typename std::tuple_element<idx, Tuple>::type>;
    std::get<idx>(toModify) = Unwrapper::unwrap(context, PyTuple_GetItem(argTup, idx));
  }

  template <typename Tuple, size_t ... Indices>
  static Tuple unwrapArgsHelper(ContextObject* context, PyObject* argTup, std::index_sequence<Indices...>) {
    Tuple toReturn;
    (unwrapArgHelper<Tuple, Indices>(context, argTup, toReturn), ...);
    return toReturn;
  }
}

template <typename T>
PyObject* wrapVector(ContextObject* CONTEXT, const std::vector<T>& vec) {
  PyObject* toReturn = PyList_New(vec.size());
  for (size_t i = 0; i < vec.size(); i++) {
    PyList_SetItem(toReturn, i, PythonReturnTypeInfo<T>::wrap(CONTEXT, vec[i]));
  }
  return toReturn;
}

template <typename T>
std::vector<T> unwrapVector(ContextObject* CONTEXT, PyObject* vec) {
  std::vector<T> toReturn(PyList_Size(vec));
  for (ssize_t i = 0; i < PyList_Size(vec); i++) {
    toReturn[i] = PythonReturnTypeInfo<T>::unwrap(CONTEXT, PyList_GetItem(vec, i));
  }
  return toReturn;
}

template <typename T>
std::string vectorTypeString() {
  return std::string("typing.List[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

template <typename T>
PyObject* wrapSet(ContextObject* CONTEXT, const std::set<T>& set) {
  PyObject* toReturn = PySet_New(nullptr);
  for (const auto& elem : set) {
    PySet_Add(toReturn, PythonReturnTypeInfo<T>::wrap(CONTEXT, elem));
  }
  return toReturn;
}

template <typename T>
std::set<T> unwrapSet(ContextObject* CONTEXT, PyObject* set) {
  std::set<T> toReturn;
  PyObject* iter = PyObject_GetIter(set);
  PyObject* item;
  while ((item = PyIter_Next(iter))) {
    toReturn.insert(PythonReturnTypeInfo<T>::unwrap(CONTEXT, item));
    Py_DECREF(item);
  }
  Py_DECREF(iter);
  return toReturn;
}

template <typename T>
std::string setTypeString() {
  return std::string("typing.Set[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

/* Same but for std::optional<T>, with 'None' as default value. */
template <typename T>
PyObject* wrapOptional(ContextObject* context, const std::optional<T>& opt) {
  if (opt) {
    return PythonReturnTypeInfo<T>::wrap(context, *opt);
  } else {
    chpl_PY_RETURN_NONE;
  }
}

template <typename T>
std::optional<T> unwrapOptional(ContextObject* context, PyObject* opt) {
  if (opt == Py_None) {
    return {};
  } else {
    return PythonReturnTypeInfo<T>::unwrap(context, opt);
  }
}

template <typename T>
std::string optionalTypeString() {
  return std::string("typing.Optional[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

template <typename ... Elems, size_t ... Is>
PyObject* wrapTupleImpl(ContextObject* context, const std::tuple<Elems...>& tup, std::index_sequence<Is...>) {
  PyObject* tuple = PyTuple_New(sizeof...(Elems));

  ssize_t idx = 0;
  auto wrapElem = [&idx, tuple, context](const auto& elem) {
    using T = std::remove_const_t<std::remove_reference_t<decltype(elem)>>;
    PyTuple_SetItem(tuple, idx++, PythonReturnTypeInfo<T>::wrap(context, elem));
  };

  (wrapElem(std::get<Is>(tup)), ...);
  return tuple;
}

template <typename ... Elems>
PyObject* wrapTuple(ContextObject* context, const std::tuple<Elems...>& tup) {
  return wrapTupleImpl<Elems...>(context, tup, std::make_index_sequence<sizeof...(Elems)>());
}

template <typename ... Elems>
std::tuple<Elems...> unwrapTuple(ContextObject* context, PyObject* tup) {
  return detail::unwrapArgsHelper<std::tuple<Elems...>>(context, tup, std::make_index_sequence<sizeof...(Elems)>());
}

template <typename ... Elems, size_t ... Is>
std::string tupleTypeStringImpl(std::index_sequence<Is...>) {
  std::string toReturn = "typing.Tuple[";

  bool needsComma = false;
  auto printElem = [&needsComma, &toReturn](const auto& elem) {
    using T = std::remove_const_t<std::remove_reference_t<decltype(elem)>>;

    if (needsComma) toReturn += ", ";
    toReturn += PythonReturnTypeInfo<T>::typeString();
    needsComma = true;
  };

  (printElem(std::get<Is>(std::tuple<Elems...>())), ...);
  if constexpr (sizeof...(Elems) == 0) {
    toReturn += "()";
  }
  toReturn += "]";

  return toReturn;
}

template <typename ... Elems>
std::string tupleTypeString() {
  return tupleTypeStringImpl<Elems...>(std::make_index_sequence<sizeof...(Elems)>());
}

template <typename T>
struct TupleTypeStringify {};

template <typename ... Ts>
struct TupleTypeStringify<std::tuple<Ts...>> {
  static std::string typeString() {
    return tupleTypeString<Ts...>();
  }
};

template <typename T>
std::string iteratorTypeString() {
  return std::string("typing.Iterator[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

template <typename T>
PyObject* wrapNilable(ContextObject* context, const Nilable<T>& opt) {
  if (opt.value) {
    return PythonReturnTypeInfo<T>::wrap(context, opt.value);
  } else {
    chpl_PY_RETURN_NONE;
  }
}

template <typename T>
Nilable<T> unwrapNilable(ContextObject* context, PyObject* opt) {
  if (opt == Py_None) {
    return {nullptr};
  } else {
    return PythonReturnTypeInfo<T>::unwrap(context, opt);
  }
}

template <typename T>
std::string nilableTypeString() {
  return std::string("typing.Optional[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

/** Different error types have different arguments in their '->info()' tuple,
    not all of which can be converetd to Python. Also, the C preprocessor
    doesn't allow us to use commas in macro arguments, so we can't just write
    std::tuple<EINFO> to get a tuple of an error's info. Instead, use template
    specialization here to define a ErrorInfoBundle with a type alias that
    is fully convertible.

    It seems to be a good idea to keep the tuple returned by Python's
    version of `->info()` always the same size as the original tuple. This
    way, as more components of the error info become convertible,
    they can be added to the returned tuple without changing user code that
    unpacks the tuple. Thus, we pad the tuple with 'dummy' elements for
    non-convertible types.

    To try to decide if a type is convertable to Python, and then use that
    decision to define a Python conversion, causes infinite recursion
    and 'explicit specialization after instantiation' errors. Thus, manually
    list compatible types via `CanConvert` template specialization.

    All, because error info tuples don't have many guarantees about
    nullability, we treat all pointer types as nilable, which requires
    a recursive transformation of the tuple to replace pointer types with their
    nilable equivalents.
  */

namespace detail {

using namespace chpl;

template <typename T>
struct CanConvert {
  // For unsupported types, just return an empty tuple.
  static auto transform(const T& node) { return std::make_tuple(); }
};

template <> struct CanConvert<const chpl::uast::AstNode*> {
  // Most error message arguments (as AST nodes) can be null. Mark them
  // as such so that they can be converted to Python (as None) even when null.
  static auto transform(const chpl::uast::AstNode* node) {
    return Nilable(node);
  }
};
template <> struct CanConvert<const chpl::types::Type*> {
  static auto transform(const chpl::types::Type* typ) { return Nilable(typ);}
};
template <> struct CanConvert<const chpl::types::Param*> {
  static auto transform(const chpl::types::Param* param) { return Nilable(param); }
};
template <> struct CanConvert<chpl::Location> {
  static auto transform(const chpl::Location& loc) { return loc; }
};
template <> struct CanConvert<chpl::resolution::ApplicabilityResult> {
  static auto transform(const chpl::resolution::ApplicabilityResult& ar) { return ar; }
};
template <> struct CanConvert<chpl::resolution::CallInfo> {
  static auto transform(const chpl::resolution::CallInfo& ar) { return ar; }
};
template <typename T> struct CanConvert<std::vector<T>> : CanConvert<T> {
  static auto transform(const std::vector<T>& vec) {
    std::vector<decltype(CanConvert<T>::transform(std::declval<T>()))> toReturn;
    for (const auto& elem : vec) {
      toReturn.push_back(CanConvert<T>::transform(elem));
    }
    return toReturn;
  }
};
#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) \
  template <> struct CanConvert<const TYPE*> { \
    static auto transform(const TYPE* val) { return Nilable(val); } \
  };
#include "generated-types-list.h"

template <typename Tuple, size_t Idx>
auto getOrReturnDummyAtIdx(const Tuple& tup) {
  using ElementType = std::decay_t<decltype(std::get<Idx>(tup))>;
  return CanConvert<ElementType>::transform(std::get<Idx>(tup));
}

template <typename Tuple, size_t ... Idx>
auto getDummyfiedImpl(const Tuple& tup, std::index_sequence<Idx...>) {
  return std::make_tuple(getOrReturnDummyAtIdx<Tuple, Idx>(tup)...);
}

template <typename Tuple>
auto getDummyfied(const Tuple& tup) {
  return getDummyfiedImpl(tup, std::make_index_sequence<std::tuple_size_v<Tuple>>());
}

template <typename Tuple>
using DummifiedType = decltype(getDummyfied(std::declval<Tuple>()));

template <typename ErrorType>
struct ErrorInfoBundleImpl {};

#define DIAGNOSTIC_CLASS(NAME, KIND, EINFO...) \
  template <> \
  struct ErrorInfoBundleImpl<chpl::Error##NAME> { \
    using ExtractedData = DummifiedType<std::tuple<EINFO>>; \
    ExtractedData data; \
    ErrorInfoBundleImpl(const std::tuple<EINFO>& originalData) : data(getDummyfied(originalData)) {} \
\
    static std::string typeString() { \
      return TupleTypeStringify<ExtractedData>::typeString(); \
    } \
  };
#include "chpl/framework/error-classes-list.h"
#undef DIAGNOSTIC_CLASS

} // end namespace detail

template <typename ErrorType>
using ErrorInfoBundle = detail::ErrorInfoBundleImpl<ErrorType>;

template <typename T>
static T unwrapUnsupported() {
  assert(false && "attempt to unwrap type from Python object that doesn't support unwrapping");
  // there's no way for a user to trigger this issue if the implementation
  // of chapel-py is correct. This is because "unwrapping" is always
  // triggered when a chapel-py developer attempts to define a method in
  // method-tables.h that accepts a Python argument of an unsupported type.
  // If the developer doesn't do so, there's no way for this function to be
  // called. Seems reasonable to exit.
  exit(1);
}

/* Invoke the DEFINE_INOUT_TYPE macro for each type we want to support.
   New types should be added here. We might consider performing these invocations
   using X-macros for the entire AST class hierarchy if we wanted to be
   more specific. */

DEFINE_INOUT_TYPE(bool, "bool", PyBool_FromLong(TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_INOUT_TYPE(int, "int", Py_BuildValue("i", TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_INOUT_TYPE(unsigned int, "int", Py_BuildValue("I", TO_WRAP), PyLong_AsUnsignedLong(TO_UNWRAP));
DEFINE_INOUT_TYPE(const char*, "str", Py_BuildValue("s", TO_WRAP), getCStringFromPyObjString(TO_UNWRAP));
DEFINE_INOUT_TYPE(chpl::UniqueString, "str", Py_BuildValue("s", TO_WRAP.c_str()), chpl::UniqueString::get(&CONTEXT->value_, getCStringFromPyObjString(TO_UNWRAP)));
DEFINE_INOUT_TYPE(std::string, "str", Py_BuildValue("s", TO_WRAP.c_str()), std::string(getCStringFromPyObjString(TO_UNWRAP)));
DEFINE_INOUT_TYPE(const chpl::uast::AstNode*, "AstNode", wrapGeneratedType(CONTEXT, TO_WRAP), ((AstNodeObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(const chpl::types::Type*, "ChapelType", wrapGeneratedType(CONTEXT, TO_WRAP), ((ChapelTypeObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(const chpl::types::Param*, "Param", wrapGeneratedType(CONTEXT, TO_WRAP), ((ParamObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(chpl::Location, "Location", (PyObject*) LocationObject::create(TO_WRAP), ((LocationObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(IterAdapterBase*, "typing.Iterator[AstNode]", wrapIterAdapter(CONTEXT, TO_WRAP), ((AstIterObject*) TO_UNWRAP)->iterAdapter);
DEFINE_INOUT_TYPE(PyObject*, "typing.Any", TO_WRAP, TO_UNWRAP);
DEFINE_INOUT_TYPE(const chpl::resolution::Scope*, "Scope", (PyObject*) ScopeObject::create(CONTEXT, TO_WRAP), ((ScopeObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(chpl::resolution::ApplicabilityResult, "ApplicabilityResult", (PyObject*) ApplicabilityResultObject::create(CONTEXT, TO_WRAP), ((ApplicabilityResultObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(chpl::resolution::CallInfo, "CallInfo", (PyObject*) CallInfoObject::create(CONTEXT, TO_WRAP), ((CallInfoObject*) TO_UNWRAP)->value_);

template <typename T>
T_DEFINE_INOUT_TYPE(T*, T::Name, (PyObject*) TO_WRAP, (T*) TO_UNWRAP);
template <typename T>
T_DEFINE_INOUT_TYPE(std::vector<T>, vectorTypeString<T>(), wrapVector(CONTEXT, TO_WRAP), unwrapVector<T>(CONTEXT, TO_UNWRAP));
template <typename T>
T_DEFINE_INOUT_TYPE(std::set<T>, setTypeString<T>(), wrapSet(CONTEXT, TO_WRAP), unwrapSet<T>(CONTEXT, TO_UNWRAP));
template <typename T>
T_DEFINE_INOUT_TYPE(std::optional<T>, optionalTypeString<T>(), wrapOptional(CONTEXT, TO_WRAP), unwrapOptional<T>(CONTEXT, TO_UNWRAP));
template <typename T>
T_DEFINE_INOUT_TYPE(Nilable<T>, nilableTypeString<T>(), wrapNilable(CONTEXT, TO_WRAP), unwrapNilable<T>(CONTEXT, TO_UNWRAP));
template <typename ... Elems>
T_DEFINE_INOUT_TYPE(std::tuple<Elems...>, tupleTypeString<Elems...>(), wrapTuple(CONTEXT, TO_WRAP), unwrapTuple<Elems...>(CONTEXT, TO_UNWRAP));
template <typename ElemType>
T_DEFINE_INOUT_TYPE(TypedIterAdapterBase<ElemType>*, iteratorTypeString<ElemType>(), wrapIterAdapter(CONTEXT, TO_WRAP), (TypedIterAdapterBase<ElemType>*) ((AstIterObject*) TO_UNWRAP)->iterAdapter);
template <typename ErrorType>
T_DEFINE_INOUT_TYPE(ErrorInfoBundle<ErrorType>, ErrorInfoBundle<ErrorType>::typeString(), wrapTuple(CONTEXT, TO_WRAP.data), unwrapUnsupported<ErrorInfoBundle<ErrorType>>());

#define GENERATED_TYPE(ROOT, ROOT_TYPE, NAME, TYPE, TAG, FLAGS) \
  DEFINE_INOUT_TYPE(const TYPE*, #NAME, wrapGeneratedType(CONTEXT, (ROOT_TYPE*) TO_WRAP), ((ROOT##Object*) TO_UNWRAP)->value_->to##NAME());
#include "generated-types-list.h"

#undef T_DEFINE_INOUT_TYPE
#undef DEFINE_INOUT_TYPE

/* Specialize for void, but don't include 'wrap' and 'unwrap' methods:
   unwrapping isn't possible since we can't accept 'void' arguments, and
   'wrapping' isn't possible because 'void' is not a traditional value.
 */
template <>
struct PythonReturnTypeInfo<void> {
  static std::string typeString() { return "None"; }
};

/* In the `method-tables.h` file, we encode a method signature using the C++
   function type in the form `R(Args...)`. This template, PythonFnHelper, is
   used to extract information back out of this function type, including
   a full argument unwrapping function that applies the corresponding 'unwrap'
   call to each element of the argument tuple. */

template<typename T> struct PythonFnHelper{};
template<typename R, typename ...Args>
struct PythonFnHelper<R(Args...)> {
 public:
  using ReturnTypeInfo = PythonReturnTypeInfo<R>;
  using ArgTypeInfo = std::tuple<PythonReturnTypeInfo<Args>...>;
  using ReturnType = R;

  static constexpr int PyArgTag = std::tuple_size<ArgTypeInfo>::value == 0 ? METH_NOARGS : METH_VARARGS;

 public:
  static std::tuple<Args...> unwrapArgs(ContextObject* context, PyObject* argsTup) {
    return detail::unwrapArgsHelper<std::tuple<Args...>>(context, argsTup, std::make_index_sequence<sizeof...(Args)>());
  }
};

#endif
