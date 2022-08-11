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

    pragma "no doc"
    proc getLocaleForKey(key: keyType): int {
      if (funcType != nothing) {
        return localeHasher(key, targetLocales.size);
      } else {
        return hashAcrossLocales(key, targetLocales.size);
      }
    }

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
      var loc: int = this.getLocaleForKey(k);

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
        var loc: int = this.getLocaleForKey(key);

        // TODO: should there be an on statement here?
        var (_, slot) = tables[loc].findAvailableSlot(key);
        var (_, slot2) = m.table.findAvailableSlot(key);
        tables[loc].fillSlot(slot, key, m.table.table[slot2].val);
      }

      for i in locDom {
        locks[i].unlock();
      }
    }

    // TODO: maybe impl equivalent on map type
    /*
      Extends this map with the contents of the other, overwriting the values
      for already-existing keys.

      :arg m: The other map
      :type m: distributedMap(keyType, valType)
    */
    proc extend(pragma "intent ref maybe const formal"
                m: distributedMap(keyType, valType)) {
      for i in locDom {
        locks[i].lock();
      }

      if !isCopyableType(keyType) || !isCopyableType(valType) then
        compilerError("extending map with non-copyable type");

      // TODO: should we lock the provided map before doing this?  Or just the
      // locale we find each key on?
      // TODO: should there be an on statement here?
      for key in m.keys() {
        var loc1: int = this.getLocaleForKey(key);
        var loc2: int = m.getLocaleForKey(key);

        var (_, slot) = tables[loc1].findAvailableSlot(key);
        var (_, slot2) = m.tables[loc2].findAvailableSlot(key);
        tables[loc1].fillSlot(slot, key, m.tables[loc2].table[slot2].val);
      }

      for i in locDom {
        locks[i].unlock();
      }
    }

    // TODO: proc this overloads?
    // TODO: getBorrowed? getValue (throws Map.KeyNotFoundError)? getAndRemove?
    // TODO: these?

    // NOTE: doesn't return a `const ref` like its counterpart on serial maps,
    // the reference could get invalidated
    /*
      Iterates over the keys of this map.

      :yields: A reference to one of the keys contained in this map.
    */
    iter keys() const {
      if !isCopyableType(keyType) then
        compilerError('in distributedMap.keys(): map key type ' +
                      keyType:string + ' is not copyable');

      foreach loc in locDom {
        for slot in tables[loc].allSlots() {
          if tables[loc].isSlotFull(slot) then
            yield tables[loc].table[slot].key;
        }
      }
    }

    /*
      Iterates over the key-value pairs of this map.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.
    */
    iter items() {
      if !isCopyableType(keyType) then
        compilerError('in distributedMap.items(): map key type ' +
                      keyType:string + ' is not copyable');

      if !isCopyableType(valType) then
        compilerError('in distributedMap.items(): map value type ' +
                      valType:string + ' is not copyable');

      foreach loc in locDom {
        for slot in tables[loc].allSlots() {
          if tables[loc].isSlotFull(slot) {
            ref tabEntry = tables[loc].table[slot];
            yield (tabEntry.key, tabEntry.val);
          }
        }
      }
    }

    /*
      Iterates over the values of this map.

      :yields: A reference to one of the values contained in this map.
    */
    iter values() const {
      if !isCopyableType(valType) then
        compilerError('in map.values(): map value type ' + valType:string +
                      ' is not copyable');

      foreach loc in locDom {
        for slot in tables[loc].allSlots() {
          if tables[loc].isSlotFull(slot) then
            yield tables[loc].table[slot].val;
        }
      }
    }

    // TODO: impl
    proc readThis(ch: channel) throws {
      for i in locDom {
        locks[i].lock();
      }

      compilerError("unimplemented");

      for i in locDom {
        locks[i].unlock();
      }
    }

    // TODO: should this encode the locale hash in some way?
    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel

           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: channel) throws {
      for i in locDom {
        locks[i].lock();
      }

      var first = true;
      ch.write("{");

      for i in locDom {
        on i {
          for slot in tables[i].allSlots() {
            if tables[i].isSlotFull(slot) {
              if first {
                first = false;
              } else {
                ch.write(", ");
              }

              ch.write(tables[i].table[slot].key, ": ",
                       tables[i].table[slot].val);
            }
          }
        }
      }

      ch.write("}");

      for i in locDom {
        locks[i].unlock();
      }
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
      var loc: int = this.getLocaleForKey(k);

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
      var loc: int = this.getLocaleForKey(k);

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

    /* If the map doesn't contain a value at position `k` add one and
       set it to `v`. If the map already contains a value at position
       `k`, update it to the value `v`.
     */
    proc addOrSet(in k: keyType, in v: valType) {
      var loc: int = this.getLocaleForKey(k);

      on loc {
        locks[loc].lock();

        var (_, slot) = tables[loc].findAvailableSlot(k);
        tables[loc].fillSlot(slot, k, v);

        locks[loc].unlock();
      }
    }

    /*
      Removes a key-value pair from the map, with the given key.

     :arg k: The key to remove from the map

     :returns: `false` if `k` was not in the map.  `true` if it was and removed.
     :rtype: bool
    */
    proc remove(k: keyType): bool {
      var loc: int = this.getLocaleForKey(k);

      var res: bool;
      on loc {
        locks[loc].lock();

        var (found, slot) = tables[loc].findFullSlot(k);

        if (found) {
          var outKey: keyType, outVal: valType;
          tables[loc].clearSlot(slot, outKey, outVal);
          tables[loc].maybeShrinkAfterRemove();
        }

        locks[loc].unlock();
        res = found;
      }

      return res;
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
