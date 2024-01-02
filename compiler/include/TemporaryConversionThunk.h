/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _THUNK_H_
#define _THUNK_H_

#include "stmt.h"

/**
  "Thunk" node, used only for converting from the untyped AST to the production
  compiler's AST. The production compiler AST node builders do some mangling of
  their arguments, in some cases cloning them. However, cloning breaks the
  convert-uast pass' "fixups". A "fixup" might be used to point a SymExpr
  to a Symbol that's only syntactically defined later, and thus not available
  during the initial conversion pass.

  To work around this, the thunk represents a not-yet-built AST node that
  nevertheless already has all the arguments required for it to build. Its
  children are accessible using normal iteration, but only the `force` method
  will actually cause the proper builder to be invoked. This way, the
  mangling builders can be deferred until after fixups are complete.

  This Thunk is a base class, extended by various templated versions to make
  the API more ergonomic.

  TODO: In the long term, we'd rather not have this wrapper class, and modify
  the various builders to avoid copying, or at least delay it. Why would we
  have a whole AST node just to work around the copying? For the time being,
  though, this is an easy-to-apply fix (just wrap your code in a buildThunk!)
  that will help make progress.
 */
class TemporaryConversionThunk : public Expr {
 public:
  AList children;

  TemporaryConversionThunk();

  virtual Expr* force() = 0;

  // Required methods for Expr

  void  verify()                                          override;
  void  accept(AstVisitor* visitor)                       override;
  DECLARE_COPY(TemporaryConversionThunk);
  TemporaryConversionThunk* copyInner(SymbolMap* map)                        override;
  Expr* getFirstExpr()                                    override;
  GenRet codegen()                                        override;
};

template <typename F, typename...Args>
class SpecThunk : public TemporaryConversionThunk {
 protected:
  F builder;
  std::tuple<Args...> argTuple;

  template <typename T, typename std::enable_if<std::is_base_of<Expr, typename std::remove_pointer<T>::type>::value>::type* = nullptr>
  void process(T expr) {
    auto child = static_cast<Expr*>(expr);
    if (child) children.insertAtTailWithoutFlattening(child);
  }

  template <typename T, typename std::enable_if<!std::is_base_of<Expr, typename std::remove_pointer<T>::type>::value>::type* = nullptr>
  void process(T t) {
    // Nothing to do for non-AST nodes.
  }

  template <size_t ...Is>
  void processArguments(std::index_sequence<Is...>) {
    auto dummy = { (process(std::get<Is>(argTuple)), 0)... };
    (void) dummy;
  }

  template <size_t ...Is>
  Expr* doApplyFunction(std::index_sequence<Is...>) {
    return builder(std::get<Is>(argTuple)...);
  }
 public:
  SpecThunk(F f, Args...args) :
    builder(std::forward<F>(f)),
    argTuple(std::forward<Args>(args)...) {
    processArguments(std::index_sequence_for<Args...>());
  }

  Expr* force() override {
    // Remove children from the thunk so they can be added to the actual node
    // being built.
    while (auto child = children.first()) child->remove();

    return doApplyFunction(std::index_sequence_for<Args...>());
  }

  void replaceChild(Expr* old_ast, Expr* new_ast) override {
    // Should not be called because all our children are in a list.
  }
};

template <typename F, typename...Args>
TemporaryConversionThunk* buildThunk(F f, Args...args) {
  return new SpecThunk<F, Args...>(std::forward<F>(f), std::forward<Args>(args)...);
}

template <typename C, typename...Args>
TemporaryConversionThunk* buildClassThunk(Args...args) {
  return buildThunk([](Args...args) {
    return new C(std::forward<Args>(args)...);
  }, std::forward<Args>(args)...);
}

#endif
