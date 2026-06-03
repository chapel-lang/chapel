/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

#include "core-types-gen.h"
#include "resolution.h"
#include "chpl/uast/all-uast.h"
#include "python-types.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/util/version-info.h"
#include "python-type-helper.h"

using namespace chpl;
using namespace uast;

template <typename T> struct InvokeHelper {};

template <typename Ret, typename... Args>
struct InvokeHelper<Ret(Args...)> {
  template <typename F>
  static PyObject* invoke(ContextObject* contextObject, F&& fn) {
    auto result = fn();
    return PythonFnHelper<Ret(Args...)>::ReturnTypeInfo::wrap(contextObject, std::move(result));
  }
};

template <typename... Args>
struct InvokeHelper<void(Args...)> {
  template <typename F>
  static PyObject* invoke(ContextObject* contextObject, F&& fn) {
    fn();
    chpl_PY_RETURN_NONE;
  }
};

/* The METHOD macro is overridden here to actually create a Python-compatible
   function to insert into the method table. Each such function retrieves
   a node's context object, calls the method body, and wraps the result
   in a Python-compatible type.
 */
#define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY)\
  PyObject* NODE##Object_##NAME(PyObject *self, PyObject *argsTup) {\
    auto* node = ((NODE##Object*) self)->unwrap(); \
    if (!node) return nullptr; \
    auto contextObject = ((NODE##Object*) self)->context(); \
    auto context = &contextObject->value_; \
    auto args = PythonFnHelper<TYPEFN>::unwrapArgs(contextObject, argsTup); \
    return InvokeHelper<TYPEFN>::invoke(contextObject, \
      [&node, context, contextObject, &args]() -> PythonFnHelper<TYPEFN>::ReturnType { \
        (void) context, contextObject; \
        BODY; \
      }); \
  }

#define CLASS_BEGIN(TAG)
#define CLASS_END(TAG)
#define PLAIN_GETTER(NODE, NAME, DOCSTR, RETTYPE, BODY) \
  METHOD(NODE, NAME, DOCSTR, RETTYPE(), (void) args; BODY)
#define ITER_PROTOTYPE(NODE, TYPE)
#define METHOD_PROTOTYPE(NODE, NAME, DOCSTR)
#define OPERATOR_PROTOTYPE(NODE, NAME, DOCSTR, TYPEFN)
