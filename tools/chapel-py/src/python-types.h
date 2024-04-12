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

#ifndef CHAPEL_PY_PYTHON_TYPES_H
#define CHAPEL_PY_PYTHON_TYPES_H

#include "core-types.h"
#include "iterator-support.h"
#include <optional>
#include <vector>
#include <set>
#include <tuple>

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
    toReturn.push_back(PythonReturnTypeInfo<T>::unwrap(CONTEXT, PyList_GetItem(vec, i)));
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
    Py_RETURN_NONE;
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
  return detail::unwrapArgsHelper(context, tup, std::make_index_sequence<sizeof...(Elems)>());
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
  toReturn += "]";

  return toReturn;
}

template <typename ... Elems>
std::string tupleTypeString() {
  return tupleTypeStringImpl<Elems...>(std::make_index_sequence<sizeof...(Elems)>());
}

template <typename T>
std::string iteratorTypeString() {
  return std::string("typing.Iterator[") + PythonReturnTypeInfo<T>::typeString() + "]";
}

template <typename T>
PyObject* wrapNilable(ContextObject* context, const Nilable<T>& opt) {
  if (opt.value) {
    return PythonReturnTypeInfo<T>::wrap(context, opt.value);
  } else {
    Py_RETURN_NONE;
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


/* Invoke the DEFINE_INOUT_TYPE macro for each type we want to support.
   New types should be added here. We might consider performing these invocations
   using X-macros for the entire AST class hierarchy if we wanted to be
   more specific. */

DEFINE_INOUT_TYPE(bool, "bool", PyBool_FromLong(TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_INOUT_TYPE(int, "int", Py_BuildValue("i", TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_INOUT_TYPE(const char*, "str", Py_BuildValue("s", TO_WRAP), PyUnicode_AsUTF8(TO_UNWRAP));
DEFINE_INOUT_TYPE(chpl::UniqueString, "str", Py_BuildValue("s", TO_WRAP.c_str()), chpl::UniqueString::get(&CONTEXT->value_, PyUnicode_AsUTF8(TO_UNWRAP)));
DEFINE_INOUT_TYPE(std::string, "str", Py_BuildValue("s", TO_WRAP.c_str()), std::string(PyUnicode_AsUTF8(TO_UNWRAP)));
DEFINE_INOUT_TYPE(const chpl::uast::AstNode*, "AstNode", wrapGeneratedType(CONTEXT, TO_WRAP), ((AstNodeObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(const chpl::types::Type*, "ChapelType", wrapGeneratedType(CONTEXT, TO_WRAP), ((ChapelTypeObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(const chpl::types::Param*, "Param", wrapGeneratedType(CONTEXT, TO_WRAP), ((ParamObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(chpl::Location, "Location", (PyObject*) LocationObject::create(TO_WRAP), ((LocationObject*) TO_UNWRAP)->value_);
DEFINE_INOUT_TYPE(IterAdapterBase*, "typing.Iterator[AstNode]", wrapIterAdapter(CONTEXT, TO_WRAP), ((AstIterObject*) TO_UNWRAP)->iterAdapter);
DEFINE_INOUT_TYPE(PyObject*, "typing.Any", TO_WRAP, TO_UNWRAP);
DEFINE_INOUT_TYPE(const chpl::resolution::Scope*, "Scope", (PyObject*) ScopeObject::create(CONTEXT, TO_WRAP), ((ScopeObject*) TO_UNWRAP)->value_);

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
