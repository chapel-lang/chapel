/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/* Attempt at starting a distributed map type */
module DistributedMap {
  private use Map;
  private use ChapelHashtable;
  private use CyclicDist;
  private use IO;

  // TODO: document type, methods
  record distributedMap {
    /* Type of map keys. */
    type keyType;
    /* Type of map values. */
    type valType;

    pragma "no doc"
    const targetLocales = Locales;

    pragma "no doc"
    const locDom = {0..<targetLocales.size}
      dmapped Cyclic(startIdx=0, targetLocales=targetLocales);

    pragma "no doc"
    var tables: [locDom] chpl__hashtable(keyType, valType);

    // TODO: I seem to recall someone not liking the idea of a lock per locale,
    // remind myself of why and maybe do something different.
    pragma "no doc"
    var locks: [locDom] owned _LockWrapper =
      [i in locDom] new owned _LockWrapper();

    // NOTE: this means that distributed maps that rely on the default hashing
    // strategy will not be able to be stored in another data structure with
    // distributed maps that specified their own hashing strategy (but that any
    // distributed maps that specify their own hashing strategy *can* be stored
    // in the same data structure as those that specified a different one)
    // Is that something we're okay with?
    pragma "no doc"
    type funcType;
    pragma "no doc"
    var localeHasher: funcType;

    /* Create a map with the specified key and value type, relying on the
       default hasher across locales */
    proc init(type keyType, type valType) {
      this.keyType = keyType;
      this.valType = valType;
      funcType = nothing;
      localeHasher = none;
    }

    /* Create a map with the specified key type, value type, and hash function
       across locales.
    */
    proc init(type keyType, type valType, hasher: func(keyType, int, int)) {
      this.keyType = keyType;
      this.valType = valType;
      funcType = func(keyType, int, int);
      localeHasher = hasher;
    }
    // TODO: initializer that uses only a subset of the locales?

    // Note: no need to warn about invalidating references, we don't intend to
    // return any
    /*
      Clears the contents of this map.
    */
    proc clear() {
      for i in locDom {
        locks[i].lock();
      }

      // Maybe tune the parallelism in this.  After all, we know that no one
      // else is doing anything in the data structure because of the previous
      // locking ...
      forall i in locDom {
        on i {
          for slot in tables[i].allSlots() {
            if tables[i].isSlotFull(slot) {
              var key: keyType;
              var val: valType;
              tables[i].clearSlot(slot, key, val);
            }
          }
          tables[i].maybeShrinkAfterRemove();
        }
      }

      for i in locDom {
        locks[i].unlock();
      }
    }

    /*
      The current number of keys contained in this map.
    */
    inline proc const size {
      for i in locDom {
        locks[i].lock();
      }

      var res = + reduce tables.tableNumFullSlots;

      for i in locDom {
        locks[i].unlock();
      }

      return res;
    }

    /*
      Returns `true` if this map contains zero keys.

      :returns: `true` if this map is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
      // TODO: size will lock in its body, is that sufficient protection?  It's
      // not like the result can't be out of date once it's returned there
      // either.
      return size == 0;
    }

    /*
      Returns `true` if the given key is a member of this map, and `false`
      otherwise.

      :arg k: The key to test for membership.
      :type k: keyType

      :returns: Whether or not the given key is a member of this map.
      :rtype: `bool`
    */
    proc const contains(const k: keyType): bool {
      var loc: int;
      if (funcType != nothing) {
        loc = localeHasher(k, targetLocales.size);
      } else {
        loc = hashAcrossLocales(k, targetLocales.size);
      }

      var res: bool;
      on loc {
        locks[loc].lock();

        var (result, _) = tables[loc].findFullSlot(k);

        locks[loc].unlock();
        res = result;
      }
      return res;
    }

    // TODO: Is it necessary to lock everything?  Maybe can do something fancy
    // like figure out all the locales needed and then lock?
    proc extend(pragma "intent ref maybe const formal"
                m: map(keyType, valType)) {
      for i in locDom {
        locks[i].lock();
      }

      if !isCopyableType(keyType) || !isCopyableType(valType) then
        compilerError("extending map with non-copyable type");

      for key in m.keys() {
        var loc: int;
        if (funcType != nothing) {
          loc = localeHasher(key, targetLocales.size);
        } else {
          loc = hashAcrossLocales(key, targetLocales.size);
        }

        var (_, slot) = tables[loc].findAvailableSlot(key);
        var (_, slot2) = m.table.findAvailableSlot(key);
        tables[loc].fillSlot(slot, key, m.table.table[slot2].val);
      }

      for i in locDom {
        locks[i].unlock();
      }
    }

    // TODO: impl and maybe equivalent on map type
    proc extend(m: distributedMap(keyType, valType)) {
      compilerError("unimplemented");
    }

    // TODO: proc this overloads?
    // TODO: getBorrowed? getValue (throws Map.KeyNotFoundError)? getAndRemove?
    // TODO: these?

    // TODO: impl
    iter keys() const ref {
      compilerError("unimplemented");
    }

    // TODO: impl
    iter items() {
      compilerError("unimplemented");
    }

    // TODO: impl
    iter values() ref {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc readThis(ch: channel) throws {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc writeThis(ch: channel) throws {
      compilerError("unimplemented");
    }

    // NOTE: Locks on the locale, so may be slower than we'd like?
    /*
      Adds a key-value pair to the map. Method returns `false` if the key
      already exists in the map.

     :arg k: The key to add to the map
     :type k: keyType

     :arg v: The value that maps to ``k``
     :type v: valueType

     :returns: `true` if `k` was not in the map and added with value `v`.
               `false` otherwise.
     :rtype: bool
    */
    proc add(in k: keyType, in v: valType): bool lifetime this < v {
      var loc: int;
      if (funcType != nothing) {
        loc = localeHasher(k, targetLocales.size);
      } else {
        loc = hashAcrossLocales(k, targetLocales.size);
      }

      var res: bool;
      on loc {
        locks[loc].lock();

        var (found, slot) = tables[loc].findAvailableSlot(k);

        // Only add if it wasn't already present
        if (!found) then
          tables[loc].fillSlot(slot, k, v);

        locks[loc].unlock();
        res = !found;
      }

      return res;
    }

    // NOTE: Locks on the locale, so may be slower than we'd like?
    /*
      Sets the value associated with a key. Method returns `false` if the key
      does not exist in the map.

     :arg k: The key whose value needs to change
     :type k: keyType

     :arg v: The desired value to the key ``k``
     :type k: valueType

     :returns: `true` if `k` was in the map and its value is updated with `v`.
               `false` otherwise.
     :rtype: bool
    */
    proc set(k: keyType, in v: valType): bool {
      var loc: int;
      if (funcType != nothing) {
        loc = localeHasher(k, targetLocales.size);
      } else {
        loc = hashAcrossLocales(k, targetLocales.size);
      }

      var res: bool;
      on loc {
        locks[loc].lock();

        var (found, slot) = tables[loc].findAvailableSlot(k);

        if (found) then
          tables[loc].fillSlot(slot, k, v);

        locks[loc].unlock();
        res = found;
      }

      return res;
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    proc addOrSet(in k: keyType, in v: valType) {
      compilerError("unimplemented");
    }

    // TODO: impl - maybe warn this is unsafe on its own?  Maybe don't provide?
    proc remove(k: keyType): bool {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc toArray(): [] (keyType, valType) {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc keysToArray(): [] keyType {
      compilerError("unimplemented");
    }

    // TODO: impl
    proc valuesToArray(): [] valType {
      compilerError("unimplemented");
    }

    // TODO: impl
    // Return new aggregator for distributed map that stores an operation that
    // should be performed on the value stored by the key.  The aggregator
    // will handle putting its contents into the map using a lower-level
    // interface.
    proc updateAggregator(updater) {
      compilerError("unimplemented");
    }

    // default function to hash across locales
    pragma "no doc"
    /* Take key and number of locales, return locale */
    proc hashAcrossLocales(key: keyType, numLocales: int): int {
      // Based on keyToLocaleAndMapIdx from v1, but I'm sure there's a better
      // way to do this (just trying to get something implemented)
      const hash = chpl__defaultHashWrapper(key);
      return hash % numLocales;
    }

    // For debugging purposes, to ensure we're distributing the map properly
    pragma "no doc"
    proc chpl_verify() {
      writeln("targetLocales = ", targetLocales);
      writeln("locDom = ", locDom);
      writeln("tables and locks locales");
      for i in tables.domain {
        writeln("tables[", i, "] is on locale ", tables[i].locale);
        writeln("locks[", i, "] is also on locale ", locks[i].locale);
      }
    }
  }
}
