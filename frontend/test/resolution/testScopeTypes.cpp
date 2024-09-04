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

#include "chpl/resolution/scope-types.h"

const IdAndFlags::Flags pub = IdAndFlags::PUBLIC;
const IdAndFlags::Flags not_pub = IdAndFlags::NOT_PUBLIC;
const IdAndFlags::Flags method = IdAndFlags::METHOD_FIELD;
const IdAndFlags::Flags not_method = IdAndFlags::NOT_METHOD_FIELD;
const IdAndFlags::Flags parenful = IdAndFlags::PARENFUL_FUNCTION;
const IdAndFlags::Flags not_parenful = IdAndFlags::NOT_PARENFUL_FUNCTION;

using FlagSet = IdAndFlags::FlagSet;

template <typename ... Args>
static FlagSet anyOf(Args...args) {
  FlagSet toReturn;
  auto temp = {(toReturn.addDisjunction(args), 0)...,};
  (void) temp;
  return toReturn;
}

// test IdAndFlags::matchFilter
static void testMatchFilter() {
  // no filter provided -> it matches
  assert(IdAndFlags::matchFilter(pub, 0, FlagSet::empty()));
  assert(IdAndFlags::matchFilter(not_pub, 0, FlagSet::empty()));

  // same filter as what we have -> it matches
  assert(IdAndFlags::matchFilter(pub, pub, FlagSet::empty()));
  assert(IdAndFlags::matchFilter(not_pub, not_pub, FlagSet::empty()));

  // no filter provided but it is excluded -> it does not match
  assert(!IdAndFlags::matchFilter(pub, 0, FlagSet::singleton(pub)));
  assert(!IdAndFlags::matchFilter(not_pub, 0, FlagSet::singleton(not_pub)));

  // test some multi-bit situations
  IdAndFlags::Flags pm = pub | method;
  IdAndFlags::Flags pf = pub | not_method;

  assert(IdAndFlags::matchFilter(pm, 0, FlagSet::empty()));
  assert(IdAndFlags::matchFilter(pm, pub, FlagSet::empty()));
  assert(IdAndFlags::matchFilter(pm, method, FlagSet::empty()));

  assert(!IdAndFlags::matchFilter(pm, not_pub, FlagSet::empty()));
  assert(!IdAndFlags::matchFilter(pm, not_method, FlagSet::empty()));
  assert(!IdAndFlags::matchFilter(pm, pm, FlagSet::singleton(pm)));
  assert(!IdAndFlags::matchFilter(pm, pm, FlagSet::singleton(pub)));
  assert(!IdAndFlags::matchFilter(pm, pm, FlagSet::singleton(method)));

  assert(IdAndFlags::matchFilter(pm, pm, FlagSet::singleton(pf)));

  // test some multi-disjunct excludes.
  auto publicOrPrivateMethods = anyOf(pub, not_pub | method);
  assert(!IdAndFlags::matchFilter(pub | method, 0, publicOrPrivateMethods));
  assert(!IdAndFlags::matchFilter(pub | not_method, 0, publicOrPrivateMethods));
  assert(!IdAndFlags::matchFilter(not_pub | method, 0, publicOrPrivateMethods));
  assert(IdAndFlags::matchFilter(not_pub | not_method, 0, publicOrPrivateMethods));

  // Packing should reduce this to just 'pub'.
  auto justPublic = anyOf(pub | method, pub);
  assert(!IdAndFlags::matchFilter(pub | method, 0, justPublic));
  assert(!IdAndFlags::matchFilter(pub | not_method, 0, justPublic));
  assert(IdAndFlags::matchFilter(not_pub | method, 0, justPublic));
  assert(IdAndFlags::matchFilter(not_pub | not_method, 0, justPublic));

  // Try a three-disjunct condition.
  auto allExceptPrivateParenfulMethods = anyOf(pub, not_method, not_parenful);
  assert(!IdAndFlags::matchFilter(pub | method | parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(pub | method | not_parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(pub | not_method | parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(pub | not_method | not_parenful, 0, allExceptPrivateParenfulMethods));
  assert(IdAndFlags::matchFilter(not_pub | method | parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(not_pub | method | not_parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(not_pub | not_method | parenful, 0, allExceptPrivateParenfulMethods));
  assert(!IdAndFlags::matchFilter(not_pub | not_method | not_parenful, 0, allExceptPrivateParenfulMethods));
}

static IdAndFlags makeTestIdAndFlags(ID id, bool isPublic, bool isMethod) {
  return IdAndFlags(id,
                    /* isPublic */ isPublic,
                    /* isMethodOrField */ isMethod,
                    /* isParenfulFunction */ false,
                    /* isMethod */ isMethod,
                    /* isModule */ false,
                    /* isType */ false);
}

// test OwnedIdsWithName::borrow
static void testBorrowIds() {
  Context ctx;
  Context* context = &ctx;

  IdAndFlags privVar = makeTestIdAndFlags(ID::fromString(context, "M.x"),
                                          /* isPublic */ false,
                                          /* isMethod */ false);

  IdAndFlags pubMethod = makeTestIdAndFlags(ID::fromString(context, "M.y"),
                                            /* isPublic */ true,
                                            /* isMethod */ true);

  IdAndFlags pubVar = makeTestIdAndFlags(ID::fromString(context, "M.z"),
                                         /* isPublic */ true,
                                         /* isMethod */ false);


  {
    // check one id with no filtering
    OwnedIdsWithName ids(privVar);
    assert(ids.numIds() == 1);
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, 0, FlagSet::empty());
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == privVar.id());
    assert(foundIds.numIds() == 1);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == privVar.id());
      count++;
    }
    assert(count == 1);
  }

  {
    // check one id with filtering
    OwnedIdsWithName ids(privVar);
    IdAndFlags::Flags f = pub;
    auto e = FlagSet::empty();
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(foundIds.isEmpty());
    int count = 0;
    for (auto id : foundIds) {
      assert(false && id.str().c_str()); // really just avoiding unused var
      count++;
    }
    assert(count == 0);
  }
  {
    // check one id with filtering
    OwnedIdsWithName ids(privVar);
    IdAndFlags::Flags f = 0;
    auto e = FlagSet::singleton(not_method);
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(foundIds.isEmpty());
  }

  {
    // check two ids with filtering out the 1st
    OwnedIdsWithName ids(privVar);
    ids.appendIdAndFlags(pubMethod);
    assert(ids.numIds() == 2);
    IdAndFlags::Flags f = pub;
    auto e = FlagSet::empty();
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == pubMethod.id());
    assert(foundIds.numIds() == 1);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == pubMethod.id());
      count++;
    }
    assert(count == 1);
  }

  {
    // check two ids with filtering out the 2nd
    OwnedIdsWithName ids(pubMethod);
    ids.appendIdAndFlags(privVar);
    IdAndFlags::Flags f = IdAndFlags::PUBLIC;
    auto e = FlagSet::empty();
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == pubMethod.id());
    assert(foundIds.numIds() == 1);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == pubMethod.id());
      count++;
    }
    assert(count == 1);
  }

  {
    // check two ids with filtering out neither
    OwnedIdsWithName ids(pubMethod);
    ids.appendIdAndFlags(privVar);
    IdAndFlags::Flags f = 0;
    auto e = FlagSet::empty();
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == pubMethod.id());
    assert(foundIds.numIds() == 2);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == privVar.id() || id == pubMethod.id());
      count++;
    }
    assert(count == 2);
  }

  {
    // check two excluding one of them
    OwnedIdsWithName ids(pubMethod);
    ids.appendIdAndFlags(privVar);
    IdAndFlags::Flags f = 0;
    auto e = FlagSet::singleton(pub | method);
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == privVar.id());
    assert(foundIds.numIds() == 1);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == privVar.id());
      count++;
    }
    assert(count == 1);
  }

  {
    // check two different ones excluding one of them
    OwnedIdsWithName ids(pubMethod);
    ids.appendIdAndFlags(pubVar);
    IdAndFlags::Flags f = 0;
    auto e = FlagSet::singleton(pub | method);
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(!foundIds.isEmpty());
    assert(foundIds.firstId() == pubVar.id());
    assert(foundIds.numIds() == 1);
    int count = 0;
    for (auto id : foundIds) {
      assert(id == pubVar.id());
      count++;
    }
    assert(count == 1);
  }

  {
    // check two excluding both
    OwnedIdsWithName ids(pubMethod);
    ids.appendIdAndFlags(pubVar);
    IdAndFlags::Flags f = 0;
    auto e = FlagSet::singleton(pub);
    MatchingIdsWithName foundIds;
    ids.gatherMatches(foundIds, f, e);
    assert(foundIds.isEmpty());
  }
}


int main() {
  testMatchFilter();
  testBorrowIds();

  return 0;
}
