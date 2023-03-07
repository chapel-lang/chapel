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

#include "test-resolution.h"

#include "chpl/resolution/scope-types.h"

// test IdAndVis::matchFilter
static void testMatchFilter() {
  // no filter provided -> it matches
  assert(IdAndVis::matchFilter(IdAndVis::PUBLIC, 0, 0));
  assert(IdAndVis::matchFilter(IdAndVis::NOT_PUBLIC, 0, 0));

  // same filter as what we have -> it matches
  assert(IdAndVis::matchFilter(IdAndVis::PUBLIC, IdAndVis::PUBLIC, 0));
  assert(IdAndVis::matchFilter(IdAndVis::NOT_PUBLIC, IdAndVis::NOT_PUBLIC, 0));

  // no filter provided but it is excluded -> it does not match
  assert(!IdAndVis::matchFilter(IdAndVis::PUBLIC, 0, IdAndVis::PUBLIC));
  assert(!IdAndVis::matchFilter(IdAndVis::NOT_PUBLIC, 0, IdAndVis::NOT_PUBLIC));

  // test some multi-bit situations
  IdAndVis::SymbolTypeFlags pm = IdAndVis::PUBLIC | IdAndVis::METHOD_OR_FIELD;
  IdAndVis::SymbolTypeFlags pf = IdAndVis::PUBLIC |
                                 IdAndVis::NOT_METHOD_OR_FIELD;

  assert(IdAndVis::matchFilter(pm, 0, 0));
  assert(IdAndVis::matchFilter(pm, IdAndVis::PUBLIC, 0));
  assert(IdAndVis::matchFilter(pm, IdAndVis::METHOD_OR_FIELD, 0));

  assert(!IdAndVis::matchFilter(pm, IdAndVis::NOT_PUBLIC, 0));
  assert(!IdAndVis::matchFilter(pm, IdAndVis::NOT_METHOD_OR_FIELD, 0));
  assert(!IdAndVis::matchFilter(pm, pm, pm));
  assert(!IdAndVis::matchFilter(pm, pm, IdAndVis::PUBLIC));
  assert(!IdAndVis::matchFilter(pm, pm, IdAndVis::METHOD_OR_FIELD));

  assert(IdAndVis::matchFilter(pm, pm, pf));
}

// test OwnedIdsWithName::borrow
static void testBorrowIds() {
  Context ctx;
  Context* context = &ctx;

  ID x = ID::fromString(context, "M.x");
  ID y = ID::fromString(context, "M.y");

  {
    // check one id with no filtering
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    auto foundIds = ids.borrow(0, 0);
    assert(foundIds.hasValue());
    auto b = foundIds.getValue();
    assert(b.firstId() == x);
    assert(b.numIds() == 1);
    int count = 0;
    for (auto id : b) {
      assert(id == x);
      count++;
    }
    assert(count == 1);
  }

  {
    // check one id with filtering
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    IdAndVis::SymbolTypeFlags f = IdAndVis::PUBLIC;
    IdAndVis::SymbolTypeFlags e = 0;
    auto foundIds = ids.borrow(f, e);
    assert(!foundIds.hasValue());
  }
  {
    // check one id with filtering
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    IdAndVis::SymbolTypeFlags f = 0;
    IdAndVis::SymbolTypeFlags e = IdAndVis::NOT_METHOD_OR_FIELD;
    auto foundIds = ids.borrow(f, e);
    assert(!foundIds.hasValue());
  }

  {
    // check two ids with filtering out the 1st
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    ids.appendIdAndVis(y, Decl::PUBLIC, /* method */ true);
    IdAndVis::SymbolTypeFlags f = IdAndVis::PUBLIC;
    IdAndVis::SymbolTypeFlags e = 0;
    auto foundIds = ids.borrow(f, e);
    assert(foundIds.hasValue());
    auto b = foundIds.getValue();
    assert(b.firstId() == y);
    assert(b.numIds() == 1);
    int count = 0;
    for (auto id : b) {
      assert(id == y);
      count++;
    }
    assert(count == 1);
  }

  {
    // check two ids with filtering out the 2nd
    OwnedIdsWithName ids(y, Decl::PUBLIC, /* method */ true);
    ids.appendIdAndVis(x, Decl::PRIVATE, /* method */ false);
    IdAndVis::SymbolTypeFlags f = IdAndVis::PUBLIC;
    IdAndVis::SymbolTypeFlags e = 0;
    auto foundIds = ids.borrow(f, e);
    assert(foundIds.hasValue());
    auto b = foundIds.getValue();
    assert(b.firstId() == y);
    assert(b.numIds() == 1);
    int count = 0;
    for (auto id : b) {
      assert(id == y);
      count++;
    }
    assert(count == 1);
  }

  {
    // check two ids with filtering out neither
    OwnedIdsWithName ids(y, Decl::PUBLIC, /* method */ true);
    ids.appendIdAndVis(x, Decl::PRIVATE, /* method */ false);
    IdAndVis::SymbolTypeFlags f = 0;
    IdAndVis::SymbolTypeFlags e = 0;
    auto foundIds = ids.borrow(f, e);
    assert(foundIds.hasValue());
    auto b = foundIds.getValue();
    assert(b.firstId() == y);
    assert(b.numIds() == 2);
    int count = 0;
    for (auto id : b) {
      assert(id == x || id == y);
      count++;
    }
    assert(count == 2);
  }
}


int main() {
  testMatchFilter();
  testBorrowIds();

  return 0;
}
