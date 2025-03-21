/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

static void checkBuilderResultLocations(Context* context,
                                        const char* filename,
                                        const BuilderResult& parsed,
                                        const AstNode* parsedAst,
                                        const BuilderResult& loaded,
                                        const AstNode* loadedAst) {
  // preliminary: check that the IDs match
  ID parsedId = parsedAst->id();
  ID loadedId = loadedAst->id();
  assert(parsedId == loadedId);

  // preliminary: check that the number of children match
  assert(parsedAst->numChildren() == loadedAst->numChildren());

  ID id = parsedId;
  int numChildren = parsedAst->numChildren();

  // preliminary: check that idToAst works as expected
  printf("Checking location for ID %s\n", id.str().c_str());

  assert(parsed.idToAst(id) == parsedAst);
  assert(loaded.idToAst(id) == loadedAst);

  // now check the main locations
  auto path = UniqueString::get(context, filename);
  {
    Location parsedLoc = parsed.idToLocation(context, id, path);
    Location loadedLoc = loaded.idToLocation(context, id, path);
    assert(parsedLoc == loadedLoc);
  }

  // check also the locations of the additional maps
  #define LOCATION_MAP(ast__, location__) { \
    Location parsedLoc = parsed.idTo##location__##Location(context, id, path); \
    Location loadedLoc = loaded.idTo##location__##Location(context, id, path); \
    assert(parsedLoc == loadedLoc); \
  }
  #include "chpl/uast/all-location-maps.h"
  #undef LOCATION_MAP

  // recurse to check child nodes
  for (int i = 0; i < numChildren; i++) {
    checkBuilderResultLocations(context, filename,
                                parsed, parsedAst->child(i),
                                loaded, loadedAst->child(i));
  }
}


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
  const BuilderResult& parsed =
    parsing::parseFileToBuilderResult(context, path, UniqueString());

  const Module* parsedMod = parsed.singleModule();
  assert(guard.realizeErrors() == 0);
  assert(parsedMod != nullptr);

  printf("Writing uAST:\n");
  parsedMod->dump();

  // Use a LibraryWriter to create a library file
  LibraryFileWriter writer(context, libpath.str());
  writer.setSourcePaths(paths);
  writer.writeAllSections();

  // use a LibraryFile to read the serialized uAST
  const LibraryFile* lf = LibraryFile::load(context, libpath);

  const Module* loadedMod = lf->loadModuleAst(context, modpath);
  assert(loadedMod != nullptr && loadedMod->isModule());
  printf("Read uAST:\n");
  loadedMod->dump();
  assert(loadedMod->completeMatch(parsedMod));

  // check also getting the locations from the read ast
  const BuilderResult& loaded = lf->loadSourceAst(context, path);
  assert(loaded.singleModule() == loadedMod);

  checkBuilderResultLocations(context, filename.c_str(),
                              parsed, parsedMod, loaded, loadedMod);
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
