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

#ifndef CHAPEL_PY_PYTHON_TYPES_H
#define CHAPEL_PY_PYTHON_TYPES_H

#include "core-types.h"
#include "iterator-support.h"

template <typename CppType>
struct PythonReturnTypeInfo {};

#define DEFINE_RETURN_TYPE(TYPE, TYPESTR, WRAP, UNWRAP) \
  template <> \
  struct PythonReturnTypeInfo<TYPE> { \
    static constexpr const char* TypeString = TYPESTR; \
  \
    static PyObject* wrap(ContextObject* CONTEXT, const std::remove_const<TYPE>::type& TO_WRAP) { \
      return WRAP; \
    } \
  \
    static TYPE unwrap(ContextObject* CONTEXT, PyObject* TO_UNWRAP) { \
      return UNWRAP; \
    } \
  \
  }

DEFINE_RETURN_TYPE(bool, "bool", Py_BuildValue("b", TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_RETURN_TYPE(int, "int", Py_BuildValue("i", TO_WRAP), PyLong_AsLong(TO_UNWRAP));
DEFINE_RETURN_TYPE(const char*, "str", Py_BuildValue("s", TO_WRAP), PyUnicode_AsUTF8(TO_UNWRAP));
DEFINE_RETURN_TYPE(chpl::UniqueString, "str", Py_BuildValue("s", TO_WRAP.c_str()), chpl::UniqueString::get(&CONTEXT->context, PyUnicode_AsUTF8(TO_UNWRAP)));
DEFINE_RETURN_TYPE(std::string, "str", Py_BuildValue("s", TO_WRAP.c_str()), std::string(PyUnicode_AsUTF8(TO_UNWRAP)));
DEFINE_RETURN_TYPE(const chpl::uast::AstNode*, "AstNode", wrapAstNode(CONTEXT, TO_WRAP), ((AstNodeObject*) TO_UNWRAP)->astNode);
DEFINE_RETURN_TYPE(IterAdapterBase*, "Iterator[AstNode]", wrapIterAdapter(CONTEXT, TO_WRAP), ((AstIterObject*) TO_UNWRAP)->iterAdapter);

template<typename T> struct PythonFnHelper{};
template<typename R, typename ...Args>
struct PythonFnHelper<R(Args...)> {
 public:
  using ReturnTypeInfo = PythonReturnTypeInfo<R>;
  using ArgTypeInfo = std::tuple<PythonReturnTypeInfo<Args>...>;

  static constexpr int PyArgTag = std::tuple_size<ArgTypeInfo>::value == 0 ? METH_NOARGS : METH_VARARGS;

 private:
  template <typename Tuple, size_t idx>
  static void unwrapArgHelper(ContextObject* context, PyObject* argTup, Tuple& toModify) {
    std::get<idx>(toModify) = std::tuple_element<idx, ArgTypeInfo>::type::unwrap(context, PyTuple_GetItem(argTup, idx));
  }

  template <typename Tuple, size_t ... Indices>
  static Tuple unwrapArgsHelper(ContextObject* context, PyObject* argTup, std::index_sequence<Indices...>) {
    Tuple toReturn;
    (unwrapArgHelper<Tuple, Indices>(context, argTup, toReturn), ...);
    return toReturn;
  }

 public:
  static std::tuple<Args...> unwrapArgs(ContextObject* context, PyObject* argsTup) {
    return unwrapArgsHelper<std::tuple<Args...>>(context, argsTup, std::make_index_sequence<sizeof...(Args)>());
  }
};

#endif
