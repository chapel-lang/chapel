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

const IdAndFlags::SymbolTypeFlags pub = IdAndFlags::PUBLIC;
const IdAndFlags::SymbolTypeFlags not_pub = IdAndFlags::NOT_PUBLIC;
const IdAndFlags::SymbolTypeFlags method = IdAndFlags::METHOD_OR_FIELD;
const IdAndFlags::SymbolTypeFlags not_method = IdAndFlags::NOT_METHOD_OR_FIELD;

// test IdAndFlags::matchFilter
static void testMatchFilter() {
  // no filter provided -> it matches
  assert(IdAndFlags::matchFilter(pub, 0, 0));
  assert(IdAndFlags::matchFilter(not_pub, 0, 0));

  // same filter as what we have -> it matches
  assert(IdAndFlags::matchFilter(pub, pub, 0));
  assert(IdAndFlags::matchFilter(not_pub, not_pub, 0));

  // no filter provided but it is excluded -> it does not match
  assert(!IdAndFlags::matchFilter(pub, 0, pub));
  assert(!IdAndFlags::matchFilter(not_pub, 0, not_pub));

  // test some multi-bit situations
  IdAndFlags::SymbolTypeFlags pm = pub | method;
  IdAndFlags::SymbolTypeFlags pf = pub | not_method;

  assert(IdAndFlags::matchFilter(pm, 0, 0));
  assert(IdAndFlags::matchFilter(pm, pub, 0));
  assert(IdAndFlags::matchFilter(pm, method, 0));

  assert(!IdAndFlags::matchFilter(pm, not_pub, 0));
  assert(!IdAndFlags::matchFilter(pm, not_method, 0));
  assert(!IdAndFlags::matchFilter(pm, pm, pm));
  assert(!IdAndFlags::matchFilter(pm, pm, pub));
  assert(!IdAndFlags::matchFilter(pm, pm, method));

  assert(IdAndFlags::matchFilter(pm, pm, pf));
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
    IdAndFlags::SymbolTypeFlags f = pub;
    IdAndFlags::SymbolTypeFlags e = 0;
    auto foundIds = ids.borrow(f, e);
    assert(!foundIds.hasValue());
  }
  {
    // check one id with filtering
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = 0;
    IdAndFlags::SymbolTypeFlags e = not_method;
    auto foundIds = ids.borrow(f, e);
    assert(!foundIds.hasValue());
  }

  {
    // check two ids with filtering out the 1st
    OwnedIdsWithName ids(x, Decl::PRIVATE, /* method */ false);
    ids.appendIdAndFlags(y, Decl::PUBLIC, /* method */ true);
    IdAndFlags::SymbolTypeFlags f = pub;
    IdAndFlags::SymbolTypeFlags e = 0;
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
    ids.appendIdAndFlags(x, Decl::PRIVATE, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = IdAndFlags::PUBLIC;
    IdAndFlags::SymbolTypeFlags e = 0;
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
    ids.appendIdAndFlags(x, Decl::PRIVATE, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = 0;
    IdAndFlags::SymbolTypeFlags e = 0;
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

  {
    // check two excluding one of them
    OwnedIdsWithName ids(y, Decl::PUBLIC, /* method */ true);
    ids.appendIdAndFlags(x, Decl::PRIVATE, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = 0;
    IdAndFlags::SymbolTypeFlags e = pub | method;
    auto foundIds = ids.borrow(f, e);
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
    // check two different ones excluding one of them
    OwnedIdsWithName ids(y, Decl::PUBLIC, /* method */ true);
    ids.appendIdAndFlags(x, Decl::PUBLIC, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = 0;
    IdAndFlags::SymbolTypeFlags e = pub | method;
    auto foundIds = ids.borrow(f, e);
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
    // check two excluding both
    OwnedIdsWithName ids(y, Decl::PUBLIC, /* method */ true);
    ids.appendIdAndFlags(x, Decl::PUBLIC, /* method */ false);
    IdAndFlags::SymbolTypeFlags f = 0;
    IdAndFlags::SymbolTypeFlags e = pub;
    auto foundIds = ids.borrow(f, e);
    assert(!foundIds.hasValue());
  }
}


int main() {
  testMatchFilter();
  testBorrowIds();

  return 0;
}
