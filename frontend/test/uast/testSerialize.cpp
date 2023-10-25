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

#include "test-common.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/all-uast.h"

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

  owned<std::ofstream> outFile;
  owned<std::ifstream> inFile;
  owned<std::stringstream> ss;

  std::ostream* out = nullptr;
  std::istream* in = nullptr;

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
    auto ser = Serializer(*out);
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

  // Now deserialize!
  if (output[0] != '\0') {
    // if 'output' is set, work with that file.
    inFile = toOwned(new std::ifstream(filename,
                                        std::ios::in | std::ios::binary));
    in = inFile.get();
  } else {
    // get an istream for the string stream
    in = ss.get();
  }

  {
    auto des = Deserializer(context, *in, stringCache);

    for (size_t i = 0; i < nToplevelModules; i++) {
      if (verbose) {
        printf("Deserializing %s %s\n", test, vec[i]->name().c_str());
      }

      owned<AstNode> mod = AstNode::deserialize(des);
      assert(mod->completeMatch(vec[i]));

      if (verbose) {
        mod->dump();
      }
    }
  }
}

int main(int argc, char** argv) {
  // use the passed file if provided
  if (argc > 1) output = argv[1];

  // TODO: these programs are relying on the short string behavior,
  // but we need to test the long strings too.

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
