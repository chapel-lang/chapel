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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/Block.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Include.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

// helper functions

struct NameFinder {
  std::set<UniqueString> names;

  bool enter(const NamedDecl* named) {
    names.insert(named->name());
    return true;
  }

  bool enter(const Identifier* ident) {
    names.insert(ident->name());
    return true;
  }

  bool enter(const AstNode* node) {
    return true;
  }

  void exit(const AstNode* node) {}
};

static void checkScopeContents(Context* context, const Module* mod,
                               std::vector<std::string>&& expected) {
  auto scope = scopeForId(context, mod->id());

  std::vector<std::string> actual;
  for (auto& symPair : getSymbolsAvailableInScope(context, scope)) {
    actual.push_back(symPair.first.str());
  }
  std::sort(actual.begin(), actual.end());
  std::sort(expected.begin(), expected.end());

  assert(actual.size() == expected.size());
  for (size_t i = 0; i < expected.size(); i++) {
    assert(actual[i] == expected[i]);
  }
}

static void checkScopeContentsViaLookup(Context* context, const Module* mod,
                                        const std::set<UniqueString>& allNames) {
  // Verify that looking up identifiers in the scope and asking for
  // all identifiers matches up.
  //
  // The property should be:
  //
  //   for each symbol s,
  //     lookup(scope, s) nonempty iff s in getSymbolsAvailableInScope(scope)

  auto scope = scopeForId(context, mod->id());
  auto allSyms = getSymbolsAvailableInScope(context, scope);

  for (auto name : allNames) {
    // 'int' is visible from all scopes, but we don't list it in allSyms.
    if (name == USTR("int")) continue;
    // ditto for the module's name
    if (name == mod->name()) continue;

    auto result = lookupNameInScope(context, scope, {}, name, IDENTIFIER_LOOKUP_CONFIG);

    if (result.empty()) {
      assert(allSyms.find(name) == allSyms.end());
    } else {
      assert(allSyms.find(name) != allSyms.end());
    }
  }
}

static void test1() {
  printf("test1\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var x: int;

        private proc f1() {}
        proc f2() {}
      }
      module N {
        var y: int;

        private proc f3() {}
        proc f4() {}
      }
      module O {
        var z: int;

        private proc f5() {}
        proc f6() {}
      }
      module P {
        public use M;
        use N;
        public use O as O;
      }
      module Q {
        use P;
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  // Note: the function under test does not include the name of the module itself,
  // and nor does it include builtin names such as 'int'. The former is
  // because the intention of 'getSymbolsAvailableInScope' is to match the set
  // of identifiers that can be found using 'lookupNameInScope' -- and
  // 'lookupNameInScope' does not report the module's own name. The latter is
  // an implementation decision, and might need to be updated in the future.

  checkScopeContents(context, vec[0], {"x", "f1", "f2"});
  checkScopeContents(context, vec[1], {"y", "f3", "f4"});
  checkScopeContents(context, vec[2], {"z", "f5", "f6"});
  checkScopeContents(context, vec[3], {"x", "y", "z", "f2", "f4", "f6", "N", "O"});
  checkScopeContents(context, vec[4], {"x", "z", "f2", "f6", "O", "P"});

  NameFinder finder;
  for (auto mod : vec) mod->traverse(finder);
  for (auto mod : vec) checkScopeContentsViaLookup(context, mod, finder.names);
}

static void test2() {
  printf("test2\n");
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, "input.chpl");
  std::string contents = R""""(
      module M {
        var x: int;

        private proc f1() {}
        proc f2() {}
      }
      module N {
        var y: int;

        private proc f3() {}
        proc f4() {}
      }
      module O {
        var z: int;

        private proc f5() {}
        proc f6() {}
      }
      module P {
        public use M only x;
        use N only y as yy;
        import O.{z, f6 as ff6};
      }
   )"""";
  setFileText(context, path, contents);

  const ModuleVec& vec = parseToplevel(context, path);

  checkScopeContents(context, vec[0], {"x", "f1", "f2"});
  checkScopeContents(context, vec[1], {"y", "f3", "f4"});
  checkScopeContents(context, vec[2], {"z", "f5", "f6"});
  checkScopeContents(context, vec[3], {"x", "yy", "z", "ff6", "N"});

  NameFinder finder;
  for (auto mod : vec) mod->traverse(finder);
  for (auto mod : vec) checkScopeContentsViaLookup(context, mod, finder.names);
}

int main() {
  test1();
  test2();
  return 0;
}
