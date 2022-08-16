/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/framework/UniqueString.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <cstdlib>

using namespace chpl;

static void test1() {
  Context ctx;
  Context* context = &ctx;

  auto t1 = ID(UniqueString::get(context, "\\#\\#\\#\\.\\.\\."), -1, 0);
  auto t2 = ID(UniqueString::get(context, "\\#\\#\\#\\.\\.\\."), -1, 0);
  assert(0 == t1.compare(t2));

  auto t1Parent = t1.parentSymbolId(context);
  assert(t1Parent.isEmpty());
  auto t1Name = t1.symbolName(context);
  assert(t1Name == "###...");

  auto exp1 = ID::expandSymbolPath(context, t1.symbolPath());
  assert(exp1.size() == 1);
  assert(exp1[0].first == "\\#\\#\\#\\.\\.\\.");
  assert(exp1[0].second == 0);

  auto t3 = ID(UniqueString::get(context, "ChapelRange.\\#"), -1, 0);
  auto t3Parent = t3.parentSymbolId(context);
  assert(t3Parent.symbolPath() == "ChapelRange");
  auto t3name = t3.symbolName(context);
  assert(t3name == "#");

  auto exp3 = ID::expandSymbolPath(context, t3.symbolPath());
  assert(exp3.size() == 2);
  assert(exp3[0].first == "ChapelRange");
  assert(exp3[0].second == 0);
  assert(exp3[1].first == "\\#");
  assert(exp3[1].second == 0);

  auto t4 = ID(UniqueString::get(context, "file\\.name\\#.submodule.x#3"),-1,0);
  auto t4Parent = t4.parentSymbolId(context);
  assert(t4Parent.symbolPath() == "file\\.name\\#.submodule");
  auto t4name = t4.symbolName(context);
  assert(t4name == "x");

  auto exp4 = ID::expandSymbolPath(context, t4.symbolPath());
  assert(exp4.size() == 3);
  assert(exp4[0].first == "file\\.name\\#");
  assert(exp4[0].second == 0);
  assert(exp4[1].first == "submodule");
  assert(exp4[1].second == 0);
  assert(exp4[2].first == "x");
  assert(exp4[2].second == 3);
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  const char* fname = "weird-file##.name.chpl";
  std::string esc = "weird-file\\#\\#\\.name#2";
  ID modId = ID(UniqueString::get(context, esc), -1, 2);
  context->setFilePathForModuleId(modId, UniqueString::get(context, fname));

  assert(modId.parentSymbolId(context).symbolPath() == "");
  assert(modId.symbolName(context) == "weird-file##.name");

  {
    UniqueString gotPath;
    UniqueString gotParentSymbol;
    bool got = context->filePathForId(modId, gotPath, gotParentSymbol);
    assert(got);
    assert(gotPath == fname);
    assert(gotParentSymbol == "");
  }

  auto escSub = esc + ".x#3";
  ID eltId = ID(UniqueString::get(context, escSub), -1, 0);
  ID eltId1 = ID(UniqueString::get(context, escSub), 1, 0);

  assert(eltId.parentSymbolId(context).symbolPath() == esc.c_str());
  assert(eltId.symbolName(context) == "x");

  assert(eltId1.parentSymbolId(context).symbolPath() == eltId1.symbolPath());
  assert(eltId1.symbolName(context) == "x");

  {
    UniqueString gotPath;
    UniqueString gotParentSymbol;
    bool got = context->filePathForId(eltId, gotPath, gotParentSymbol);
    assert(got);
    assert(gotPath == fname);
    assert(gotParentSymbol == "");
  }

  const char* subFname = "weird-file##.name/Submodule.chpl";
  auto escSubMod = esc + ".Submodule#4";
  ID subModId = ID(UniqueString::get(context, escSubMod), -1, 0);

  assert(subModId.parentSymbolId(context).symbolPath() == esc.c_str());
  assert(subModId.symbolName(context) == "Submodule");

  context->setFilePathForModuleId(subModId,
                                  UniqueString::get(context, subFname));

  {
    UniqueString gotPath;
    UniqueString gotParentSymbol;
    bool got = context->filePathForId(subModId, gotPath, gotParentSymbol);
    assert(got);
    assert(gotPath == subFname);
    assert(gotParentSymbol == modId.symbolPath());
  }

  auto escSubElt = escSubMod + ".y#5";
  ID subEltId = ID(UniqueString::get(context, escSubElt), -1, 0);

  assert(subEltId.parentSymbolId(context).symbolPath()==subModId.symbolPath());
  assert(subEltId.symbolName(context) == "y");

  {
    UniqueString gotPath;
    UniqueString gotParentSymbol;
    bool got = context->filePathForId(subEltId, gotPath, gotParentSymbol);
    assert(got);
    assert(gotPath == subFname);
    assert(gotParentSymbol == modId.symbolPath());
  }

}

static void test3() {
  Context ctx;
  Context* context = &ctx;

  ID id1 = ID(UniqueString::get(context, "IO"), -1, 6);
  ID id1a = ID(UniqueString::get(context, "IO"), 5, 0);
  ID id2 = ID(UniqueString::get(context, "IO.open"), -1, 8);
  ID id2a = ID(UniqueString::get(context, "IO.open"), 7, 0);
  ID id3 = ID(UniqueString::get(context, "IO.openHelper"), -1, 13);
  ID id3a = ID(UniqueString::get(context, "IO.openHelper"), 12, 0);

  assert(id1.contains(id1));
  assert(id1.contains(id1a));
  assert(id1.contains(id2));
  assert(id1.contains(id2a));
  assert(id1.contains(id3));
  assert(id1.contains(id3a));

  assert(id1a.contains(id1a));
  assert(!id1a.contains(id1));
  assert(!id1a.contains(id2));
  assert(!id1a.contains(id2a));
  assert(!id1a.contains(id3));
  assert(!id1a.contains(id3a));

  assert(id2.contains(id2));
  assert(id2.contains(id2a));
  assert(!id2.contains(id3));
  assert(!id2.contains(id3a));
  assert(!id2.contains(id1));
  assert(!id2.contains(id1a));

  assert(id2a.contains(id2a));
  assert(!id2a.contains(id1));
  assert(!id2a.contains(id1a));
  assert(!id2a.contains(id2));
  assert(!id2a.contains(id3));
  assert(!id2a.contains(id3a));

  assert(id3.contains(id3));
  assert(id3.contains(id3a));
  assert(!id3.contains(id2));
  assert(!id3.contains(id2a));
  assert(!id3.contains(id1));
  assert(!id3.contains(id1a));

  assert(id3a.contains(id3a));
  assert(!id3a.contains(id1));
  assert(!id3a.contains(id1a));
  assert(!id3a.contains(id2));
  assert(!id3a.contains(id2a));
  assert(!id3a.contains(id3));


}


int main() {
  test1();
  test2();
  test3();

  return 0;
}
