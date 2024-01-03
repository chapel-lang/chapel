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

#include "test-common.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/Module.h"
#include "chpl/util/filesystem.h"

#include <fstream>
#include <sstream>

using namespace chpl;
using namespace uast;

const char* output = "";
const bool verbose = true;

static void testSerializeDeserialize(const char* test, const char* program) {
  printf("%s\n", test);

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string filename = std::string(test) + ".chpl";
  auto libname = UniqueString::get(context, std::string(test) + ".dyno");

  owned<std::ofstream> outFile;
  owned<std::ifstream> inFile;
  owned<std::stringstream> ss;

  std::ostream* out = nullptr;

  if (output[0] != '\0') {
    // if 'output' is set, work with that file.
    outFile = toOwned(new std::ofstream(filename,
                                        std::ios::out | std::ios::binary));
    out = outFile.get();
  } else {
    // otherwise, work with a string stream
    ss = toOwned(new std::stringstream());
    out = ss.get();
  }

  auto path = UniqueString::get(context, filename);
  parsing::setFileText(context, path, program);

  const parsing::ModuleVec& vec = parsing::parseToplevel(context, path);
  assert(guard.realizeErrors() == 0);
  size_t nToplevelModules = vec.size();

  Serializer::stringCacheType stringCache;

  {
    auto ser = Serializer(*out, /*LibraryFileAstRegistration*/ nullptr);
    for (size_t i = 0; i < nToplevelModules; i++) {
      if (verbose) {
        printf("Serializing %s %s\n", test, vec[i]->name().c_str());
        vec[i]->dump();
      }

      vec[i]->serialize(ser);
    }
    out->flush();

    stringCache = ser.stringCache();
  }

  std::string serializedData;

  // Now deserialize!
  if (output[0] != '\0') {
    // if 'output' is set, work with that file.
    std::string error;
    bool ok = readFile(filename.c_str(), serializedData, error);
    assert(ok);
  } else {
    // get the data we write to the string stream
    serializedData = ss->str();
  }

  {
    // This code uses Builder::createForLibraryFileModule,
    // rather than just creating a Deserializer, in order
    // to assign IDs

    UniqueString parentSymbolPath;

    auto builder =
      Builder::createForLibraryFileModule(context,
                                          libname,
                                          parentSymbolPath,
                                          /*LibraryFile*/nullptr);

    auto des = Deserializer(context,
                            serializedData.c_str(), serializedData.size(),
                            stringCache);

    for (size_t i = 0; i < nToplevelModules; i++) {
      if (verbose) {
        printf("Deserializing %s %s\n", test, vec[i]->name().c_str());
      }

      builder->addToplevelExpression(AstNode::deserializeWithoutIds(des));
    }

    // avoid an assertion
    builder->noteSymbolTableSymbols(Builder::SymbolTableVec());

    // assign IDs
    BuilderResult r = builder->result();
    int i = 0;
    for (auto ast : r.topLevelExpressions()) {
      const Module* mod = ast->toModule();
      assert(mod);

      if (verbose) {
        printf("got a module:\n");
        if (mod == nullptr) {
          printf("nullptr\n");
        } else {
          mod->dump();
        }
      }

      assert(mod->completeMatch(vec[i]));
      i++;
    }
  }
}

int main(int argc, char** argv) {
  // use the passed file if provided
  if (argc > 1) output = argv[1];

  testSerializeDeserialize("test1", "var i: int;");
  testSerializeDeserialize("test2", "module X { var i: int; }");
  testSerializeDeserialize("test3",
                           R""""(
                              module A {
                                module B {
                                  proc main() {
                                    writeln("hello world");
                                  }
                                }
                              }
                           )"""");
  testSerializeDeserialize("test4",
                           R""""(
                              class C { var x: int; }
                              record R { var y: int; }
                              proc R.secondary(arg: string) { return 1; }
                              var cc = new C(1);
                              var rr = new R(2);
                              var zz = rr.secondary("actual");
                           )"""");
  testSerializeDeserialize("test5",
                           R""""(
                              module SomeVeryLongIdentifierName {
                                var anEvenLongerIntegerVariableName: int;
                              }
                           )"""");

  return 0;
}
