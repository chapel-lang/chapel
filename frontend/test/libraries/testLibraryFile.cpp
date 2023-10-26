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

#include "chpl/libraries/LibraryFile.h"
#include "chpl/libraries/LibraryFileWriter.h"
#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/Module.h"
#include "chpl/util/filesystem.h"

using namespace chpl;
using namespace uast;
using namespace libraries;

const char* output = "test.dyno";

static void testStoreLoadAst(const char* test,
                             const char* modname,
                             const char* program) {
  printf("%s\n", test);

  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string filename = std::string(test) + ".chpl";
  auto libpath = UniqueString::get(context, output);
  auto path = UniqueString::get(context, filename);
  std::vector<UniqueString> paths;
  paths.push_back(path);
  auto modpath = UniqueString::get(context, modname);

  parsing::setFileText(context, path, program);
  const parsing::ModuleVec& parsedMods = parsing::parseToplevel(context, path);
  assert(guard.realizeErrors() == 0);
  size_t nToplevelModules = parsedMods.size();
  assert(nToplevelModules == 1); // otherwise, need to update this test code

  printf("Writing uAST:\n");
  parsedMods[0]->dump();

  // Use a LibraryWriter to create a library file
  LibraryFileWriter writer(context, paths, libpath.str());
  writer.writeAllSections();

  // use a LibraryFile to read the serialized uAST
  const LibraryFile* lf = LibraryFile::load(context, libpath);

  const Module* mod = lf->loadModuleAst(context, modpath);
  assert(mod != nullptr && mod->isModule());
  printf("Read uAST:\n");
  mod->dump();
  assert(mod->completeMatch(parsedMods[0]));
}

static void testStrings() {
  std::vector<const char*> strings =
    {"a",
     "hello",
     "abcdefghijklmnopqrstuvwxyz",
     "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
     "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ",
     // each of these concatenated strings is 50 characters long,
     // so the resulting string is 200 characters long.
     "a0123456789b0123456789c0123456789d0123456789a0a0a0"
     "a0123456789b0123456789c0123456789d0123456789b1b1b1"
     "a0123456789b0123456789c0123456789d0123456789c2c2c2"
     "a0123456789b0123456789c0123456789d0123456789d3d3d3"};

  for (auto s : strings) {
    std::string str = std::string(s);
    std::string code = "module M { var " + str + ": int; }";
    testStoreLoadAst("string-test", "M", code.c_str());
  }
}



int main(int argc, char** argv) {
  // use the passed file if provided
  if (argc > 1) output = argv[1];

  testStoreLoadAst("test1", "test1", "var i: int;");

  testStrings();

  testStoreLoadAst("test2", "X", "module X { var i: int; }");
  testStoreLoadAst("test3", "A", R""""(
                                        module A {
                                          module B {
                                            proc main() {
                                              writeln("hello world");
                                            }
                                          }
                                        }
                                      )"""");
  testStoreLoadAst("test4",  "test4",
                   R""""(
                          class C { var x: int; }
                          record R { var y: int; }
                          proc R.secondary(arg: string) { return 1; }
                          var cc = new C(1);
                          var rr = new R(2);
                          var zz = rr.secondary("actual");
                        )"""");
  testStoreLoadAst("test5", "SomeVeryLongIdentifierName",
                   R""""(
                           module SomeVeryLongIdentifierName {
                             var anEvenLongerIntegerVariableName: int;
                           }
                         )"""");

  return 0;
}
