/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

/* Chapel's standard 'map' type for key-value storage.

  This module contains the implementation of the map type which is a container
  that stores key-value associations.

  Maps are not parallel safe by default, but can be made parallel safe by
  setting the param formal `parSafe` to true in any map constructor. When
  constructed from another map, the new map will inherit the parallel safety
  mode of its originating map.
*/
module Map {
  import ChapelLocks;
  private use ChapelHashtable;
  private use HaltWrappers;
  private use IO;

  // Lock code lifted from modules/standard/List.chpl.
  // Maybe they should be combined into a Locks module.
  pragma "no doc"
  type _lockType = ChapelLocks.chpl_LocalSpinlock;

  pragma "no doc"
  class _LockWrapper {
    var lock$ = new _lockType();

    inline proc lock() {
      lock$.lock();
    }

    inline proc unlock() {
      lock$.unlock();
    }
  }

  private proc _checkKeyAndValType(type K, type V) {
    if isGenericType(K) {
      compilerWarning('creating a map with key type ', K:string, 2);
      if isClassType(K) && !isGenericType(K:borrowed) {
        compilerWarning('which is a class type with generic ',
                        'management', 2);
      }
      compilerError('map key type cannot currently be generic', 2);
    }
    if isGenericType(V) {
      compilerWarning('creating a map with value type ', V:string, 2);
      if isClassType(V) && !isGenericType(V:borrowed) {
        compilerWarning('which is a class type with generic ',
                        'management', 2);
      }
      compilerError('map value type cannot currently be generic', 2);
    }
  }

  record map {
    /* Type of map keys. */
    type keyType;
    /* Type of map values. */
    type valType;

    /* If `true`, this map will perform parallel safe operations. */
    param parSafe = false;

    /*
       Fractional value that specifies how full this map can be
       before requesting additional memory. The default value of
       0.5 means that the map will not resize until the map is more
       than 50% full. The acceptable values for this argument are
       between 0 and 1, exclusive, meaning (0,1). This is useful
       when you would like to reduce memory impact or potentially
       speed up how fast the map finds a slot. To override the
       default value of 0.5, the `defaultHashTableResizeThreshold`
       config flag can be set at runtime. Note that this default
       affects all hash-based data structures, including
       associative domains and sets.
    */
    const resizeThreshold = defaultHashTableResizeThreshold;

    pragma "no doc"
    var table: chpl__hashtable(keyType, valType);

    pragma "no doc"
    var _lock$ = if parSafe then new _LockWrapper() else none;

    pragma "no doc"
    inline proc _enter() {
      if parSafe then
        _lock$.lock();
    }

    pragma "no doc"
    inline proc _leave() {
      if parSafe then
        _lock$.unlock();
    }



    /*
      Initializes an empty map containing keys and values of given types.

      :arg keyType: The type of the keys of this map.
      :arg valType: The type of the values of this map.
      :arg resizeThreshold: Fractional value that specifies how full this map
                            can be before requesting additional memory.
      :arg initialCapacity: Integer value that specifies starting map size. The
                            map can hold at least this many values before
                            attempting to resize.
    */
    proc init(type keyType, type valType,
              resizeThreshold=defaultHashTableResizeThreshold,
              initialCapacity=16) {
      _checkKeyAndValType(keyType, valType);
      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = false;
      if resizeThreshold <= 0 || resizeThreshold >= 1 {
        warning("'resizeThreshold' must be between 0 and 1.",
                        " 'resizeThreshold' will be set to 0.5");
        this.resizeThreshold = 0.5;
      } else {
        this.resizeThreshold = resizeThreshold;
      }
      table = new chpl__hashtable(keyType, valType, this.resizeThreshold,
                                  initialCapacity);
    }

    /*
      Initializes an empty map containing keys and values of given types.

      :arg keyType: The type of the keys of this map.
      :arg valType: The type of the values of this map.
      :arg parSafe: If `true`, this map will use parallel safe operations.
      :arg resizeThreshold: Fractional value that specifies how full this map
                            can be before requesting additional memory.
      :arg initialCapacity: Integer value that specifies starting map size. The
                            map can hold at least this many values before
                            attempting to resize.
    */
    @unstable("'Map.parSafe' is unstable")
    proc init(type keyType, type valType, param parSafe,
              resizeThreshold=defaultHashTableResizeThreshold,
              initialCapacity=16) {
      _checkKeyAndValType(keyType, valType);
      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = parSafe;
      if resizeThreshold <= 0 || resizeThreshold >= 1 {
        warning("'resizeThreshold' must be between 0 and 1.",
                        " 'resizeThreshold' will be set to 0.5");
        this.resizeThreshold = 0.5;
      } else {
        this.resizeThreshold = resizeThreshold;
      }
      table = new chpl__hashtable(keyType, valType, this.resizeThreshold,
                                  initialCapacity);
    }

    proc init(type keyType, type valType,
              resizeThreshold=defaultHashTableResizeThreshold,
              initialCapacity=16)
      where isNonNilableClass(valType) {
      _checkKeyAndValType(keyType, valType);
      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = false;
      if resizeThreshold <= 0 || resizeThreshold >= 1 {
        warning("'resizeThreshold' must be between 0 and 1.",
                        " 'resizeThreshold' will be set to 0.5");
        this.resizeThreshold = 0.5;
      } else {
        this.resizeThreshold = resizeThreshold;
      }
      table = new chpl__hashtable(keyType, valType, this.resizeThreshold,
                                  initialCapacity);
    }

    @unstable("'Map.parSafe' is unstable")
    proc init(type keyType, type valType, param parSafe,
              resizeThreshold=defaultHashTableResizeThreshold,
              initialCapacity=16)
    where isNonNilableClass(valType) {
      _checkKeyAndValType(keyType, valType);
      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = parSafe;
      if resizeThreshold <= 0 || resizeThreshold >= 1 {
        warning("'resizeThreshold' must be between 0 and 1.",
                        " 'resizeThreshold' will be set to 0.5");
        this.resizeThreshold = 0.5;
      } else {
        this.resizeThreshold = resizeThreshold;
      }
      table = new chpl__hashtable(keyType, valType, this.resizeThreshold,
                                  initialCapacity);
    }

    /*
      Initializes a map containing elements that are copy initialized from
      the elements contained in another map.

      :arg other: The map to initialize from.
      :type other: map

      :arg parSafe: If `true`, this map will use parallel safe operations.
      :type parSafe: bool
    */
    proc init=(pragma "intent ref maybe const formal"
               other: map(?kt, ?vt, ?ps)) lifetime this < other {

      // TODO: There has got to be some way that we can abstract this!
      // Arguably this is something that the compiler should be
      // inferring for us in some way.
      this.keyType = if this.type.keyType != ? then
                        this.type.keyType else kt;
      this.valType = if this.type.valType != ? then
                        this.type.valType else vt;
      this.parSafe = if this.type.parSafe != ? then
                        this.type.parSafe else ps;
      this.resizeThreshold = other.resizeThreshold;
      this.table = new chpl__hashtable(keyType, valType,
                                       resizeThreshold);
      this.complete();

      if keyType != kt {
        compilerError('cannot initialize ', this.type:string, ' from ',
                      other.type:string, ' due to key type mismatch');
      } else if valType != vt {
        compilerError('cannot initialize ', this.type:string, ' from ',
                      other.type:string, ' due to value type mismatch');
      } else if !isCopyableType(keyType) {
        compilerError('cannot initialize ', this.type:string, ' from ',
                      other.type:string, ' because key type ',
                      keyType:string, ' is not copyable');
      } else if !isCopyableType(valType) {
        compilerError('cannot initialize ', this.type:string, ' from ',
                      other.type:string, ' because value type ',
                      valType:string, ' is not copyable');
      } else {
        for key in other.keys() {
          const (_, slot) = table.findAvailableSlot(key);
          const (_, slot2) = other.table.findFullSlot(key);
          table.fillSlot(slot, key, other.table.table[slot2].val);
        }
      }
    }

    /*
      Clears the contents of this map.

      .. warning::

        Clearing the contents of this map will invalidate all existing
        references to the elements contained in this map.
    */
    proc clear() {
      _enter(); defer _leave();
      for slot in table.allSlots() {
        if table.isSlotFull(slot) {
          var key: keyType;
          var val: valType;
          table.clearSlot(slot, key, val);
        }
      }
      table.maybeShrinkAfterRemove();
    }

    /*
      The current number of keys contained in this map.
    */
    inline proc const size {
      _enter(); defer _leave();
      return _size;
    }

    /* As above, but the parSafe lock must be held on entry */
    pragma "no doc"
    inline proc const _size {
      return table.tableNumFullSlots;
    }

    /*
      Returns `true` if this map contains zero keys.

      :returns: `true` if this map is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {
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
      _enter(); defer _leave();
      var (result, _) = table.findFullSlot(k);
      return result;
    }

    /*
      Extends this map with the contents of the other, overwriting the values
      for already-existing keys.

      :arg m: The other map
      :type m: map(keyType, valType)
    */
    proc extend(pragma "intent ref maybe const formal"
                m: map(keyType, valType, parSafe)) {
      _enter(); defer _leave();

      if !isCopyableType(keyType) || !isCopyableType(valType) then
        compilerError("extending map with non-copyable type");

      for key in m.keys() {
        var (_, slot) = table.findAvailableSlot(key);
        var (_, slot2) = m.table.findAvailableSlot(key);
        table.fillSlot(slot, key, m.table.table[slot2].val);
      }
    }

    /*
      Update a value in this map in a parallel safe manner via an updater
      object.

      The updater object passed to the `update()` method must define a
      `this()` method that takes two arguments: the first has this map's
      `keyType`, and the second has this map's `valType`.

      The updater object's `this()` method must return some sort of value.
      Updater objects that do not need to return anything may
      return `none`.

      If the updater object's `this()` method throws, the thrown error will
      be propagated out of `update()`.

      :arg k: The key to update
      :type k: `keyType`

      :arg updater: A class or record used to update the value at `i`

      :throws KeyNotFoundError: if `k` not in map

      :return: What the updater returns
    */
    proc update(const ref k: keyType, updater) throws {
      _enter(); defer _leave();

      var (isFull, slot) = table.findFullSlot(k);

      if !isFull then
        throw new KeyNotFoundError(k:string);

      // TODO: Use table key or argument key?
      const ref key = table.table[slot].key;
      ref val = table.table[slot].val;

      import Reflection;
      if !Reflection.canResolveMethod(updater, "this", key, val) then
        compilerError('`map.update()` failed to resolve method ' +
                      updater.type:string + '.this() for arguments (' +
                      key.type:string + ', ' + val.type:string + ')');

      return updater(key, val);
    }

    pragma "no doc"
    inline proc _warnForParSafeIndexing() {
      if parSafe then
        compilerError('cannot index into a map initialized with ',
                      '`parSafe=true`', 2);
    }

    /*
      If the key exists in the map, get a reference to the value mapped
      to the given key. If the key does not exist in the map, the value
      type is default initializable, and this proc is called in an attempt
      to modify the map, then the mapping is added and a reference to
      that value is returned. If the key does not exist in the map and
      either the value type is not default initializable or this proc
      is called without attempting to modify the map, then an error will
      be thrown.

      :arg k: The key to access
      :type k: keyType

      :throws KeyNotFoundError: if `k` not in map, `valType` is not
              default initializable, and proc is called in an attempt
              to modify the map.

      :returns: Reference to the value mapped to the given key.
    */
    proc ref this(k: keyType) ref throws
      where isDefaultInitializable(valType) {
      _warnForParSafeIndexing();

      _enter(); defer _leave();

      var (_, slot) = table.findAvailableSlot(k);
      if !table.isSlotFull(slot) {
        var val: valType;
        table.fillSlot(slot, k, val);
      }
      ref result = table.table[slot].val;
      return result;
    }

    proc ref this(k: keyType) ref throws {
      _warnForParSafeIndexing();

      _enter(); defer _leave();

      var (_, slot) = table.findAvailableSlot(k);
      if !table.isSlotFull(slot) {
        throw new KeyNotFoundError(k:string);
      }
      ref result = table.table[slot].val;
      return result;
    }

    pragma "no doc"
    proc const this(k: keyType) const ref throws {
      _warnForParSafeIndexing();

      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        throw new KeyNotFoundError(k:string);
      const ref result = table.table[slot].val;
      return result;
    }

    /* Get a borrowed reference to the element at position `k`.
     */
    @deprecated(notes="'Map.getBorrowed' is deprecated. Please rely on '[]' accessors instead.")
    proc getBorrowed(k: keyType) where isClass(valType) {
      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        var result = table.table[slot].val.borrow();
        if isNonNilableClass(valType) {
          return result!;
        } else {
          return result;
        }
      }
    }

    /* Get a reference to the element at position `k`. This method is not
       available for maps initialized with `parSafe=true`.
     */
    @deprecated(notes="'Map.getReference' is deprecated. Please rely on '[]' accessors instead.")
    proc getReference(k: keyType) ref {
      if parSafe then
        compilerError('cannot call `getReference()` on maps initialized ',
                      'with `parSafe=true`');

      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      ref result = table.table[slot].val;
      return result;
    }

    /* Get a copy of the element stored at position `k`.

      :arg k: The key to lookup in the map

      :throws KeyNotFoundError: if `k` not in map

      :returns: A copy of the value at position `k`
     */
    @deprecated(notes="'Map.getValue' is deprecated. Please rely on '[]' accessors instead.")
    proc getValue(k: keyType) const throws {
      if !isCopyableType(valType) then
        compilerError('cannot call `getValue()` for non-copyable ' +
                      'map value type: ' + valType:string);

      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        throw new KeyNotFoundError(k: string);
      try! {
        const result = table.table[slot].val: valType;
        return result;
      }
    }

    /* Get a copy of the element stored at position `k` or a sentinel
       value if an element at position `k` does not exist.

      :arg k: The key to lookup in the map
      :arg sentinel: The value to return if the map does not contain an
                     entry at position `k`

      :returns: A copy of the value at position `k` or a sentinel value
                if the map does not have an entry at position `k`
    */
    proc get(k: keyType, const sentinel: valType) const {
      if !isCopyableType(valType) then
        compilerError('cannot call `getValue()` for non-copyable ' +
                      'map value type: ' + valType:string);

      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        return sentinel;
      try! {
        const result = table.table[slot].val: valType;
        return result;
      }
    }

    @deprecated(notes="'Map.getValue' is deprecated. Please use 'Map.get' instead.")
    proc getValue(k: keyType, const sentinel: valType) const {
      if !isCopyableType(valType) then
        compilerError('cannot call `getValue()` for non-copyable ' +
                      'map value type: ' + valType:string);

      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        return sentinel;
      try! {
        const result = table.table[slot].val: valType;
        return result;
      }
    }

    /* Remove the element at position `k` from the map and return its value
     */
    proc getAndRemove(k: keyType) {
      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        var result: valType, key: keyType;
        table.clearSlot(slot, key, result);
        table.maybeShrinkAfterRemove();
        return result: valType;
      }
    }

    /*
      Iterates over the keys of this map. This is a shortcut for :iter:`keys`.

      :yields: A reference to one of the keys contained in this map.
    */
    @deprecated(notes="'Map.these' is deprecated. Consider 'Map.keys' to iterate over keys or 'Map.values' to iterate over values.")
    iter these() const ref {
      for key in this.keys() {
        yield key;
      }
    }

    /*
      Iterates over the keys of this map.

      :yields: A reference to one of the keys contained in this map.
    */
    iter keys() const ref {
      foreach slot in table.allSlots() {
        if table.isSlotFull(slot) then
          yield table.table[slot].key;
      }
    }

    /*
      Iterates over the key-value pairs of this map.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.
    */
    @deprecated(notes="'Map.items' is deprecated. Consider 'Map.keys' to iterate over keys or 'Map.values' to iterate over values.")
    iter items() {
      if !isCopyableType(keyType) then
        compilerError('in map.items(): map key type ' + keyType:string +
                      ' is not copyable');

      if !isCopyableType(valType) then
        compilerError('in map.items(): map value type ' + valType:string +
                      ' is not copyable');

      foreach slot in table.allSlots() {
        if table.isSlotFull(slot) {
          ref tabEntry = table.table[slot];
          yield (tabEntry.key, tabEntry.val);
        }
      }
    }

    /*
      Iterates over the values of this map.

      :yields: A reference to one of the values contained in this map.
    */
    iter values() ref
    where !isNonNilableClass(valType) {
      foreach slot in table.allSlots() {
        if table.isSlotFull(slot) then
          yield table.table[slot].val;
      }
    }

    pragma "no doc"
    iter values() const where isNonNilableClass(valType) {
      try! {
        foreach slot in table.allSlots() {
          if table.isSlotFull(slot) then
            yield table.table[slot].val: valType;
        }
      }
    }

    /*
      Reads the contents of this map from a channel. The format looks like:

        .. code-block:: chapel

           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to read from.
    */
    proc readThis(ch: fileReader) throws {
      _readWriteHelper(ch);
    }

    //
    // TODO: rewrite to use formatter interface
    //
    pragma "no doc"
    proc init(type keyType, type valType, r: fileReader) {
      this.init(keyType, valType, parSafe);
      readThis(r);
    }

    pragma "no doc"
    @unstable("'Map.parSafe' is unstable")
    proc init(type keyType, type valType, param parSafe, r: fileReader) {
      this.init(keyType, valType, parSafe);
      readThis(r);
    }

    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel

           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: fileWriter) throws {
      _readWriteHelper(ch);
    }

    pragma "no doc"
    proc _readWriteHelper(ch) throws {
      _enter(); defer _leave();
      var first = true;
      proc rwLiteral(lit:string) throws {
        if ch.writing then ch._writeLiteral(lit); else ch._readLiteral(lit);
      }
      rwLiteral("{");
      for slot in table.allSlots() {
        if table.isSlotFull(slot) {
          if first {
            first = false;
          } else {
            rwLiteral(", ");
          }
          ref tabEntry = table.table[slot];
          ref key = tabEntry.key;
          ref val = tabEntry.val;
          if ch.writing then ch.write(key); else key = ch.read(key.type);
          rwLiteral(": ");
          if ch.writing then ch.write(val); else val = ch.read(val.type);
        }
      }
      rwLiteral("}");
    }

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
      _enter(); defer _leave();
      var (found, slot) = table.findAvailableSlot(k);
      if found {
        return false;
      }

      table.fillSlot(slot, k, v);

      return true;
    }

    @deprecated(notes="'Map.set' is deprecated. Please use 'Map.replace' instead.")
    proc set(k: keyType, in v: valType): bool {
      return this.replace(k, v);
    }

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
    proc replace(k: keyType, in v: valType): bool {
      _enter(); defer _leave();
      var (found, slot) = table.findAvailableSlot(k);
      if !found {
        return false;
      }

      table.fillSlot(slot, k, v);

      return true;
    }

    /* If the map doesn't contain a value at position `k` add one and
       set it to `v`. If the map already contains a value at position
       `k`, update it to the value `v`.
     */
    proc addOrSet(in k: keyType, in v: valType) {
      _enter(); defer _leave();
      var (found, slot) = table.findAvailableSlot(k);
      table.fillSlot(slot, k, v);
    }

    /*
      Removes a key-value pair from the map, with the given key.

     :arg k: The key to remove from the map

     :returns: `false` if `k` was not in the map.  `true` if it was and removed.
     :rtype: bool
    */
    proc remove(k: keyType): bool {
      _enter(); defer _leave();
      var (found, slot) = table.findFullSlot(k);
      if !found {
        return false;
      }
      var outKey: keyType, outVal: valType;
      table.clearSlot(slot, outKey, outVal);
      table.maybeShrinkAfterRemove();
      return true;
    }

    /*
      Returns a new 0-based array containing a copy of key-value pairs as
      tuples.

      :return: A new DefaultRectangular array.
      :rtype: [] (keyType, valType)
    */
    proc toArray(): [] (keyType, valType) {
      _enter(); defer _leave();

      if !isCopyableType(keyType) || !isCopyableType(valType) then
        compilerError("toArray requires copyable key and value types");

      var A: [0..#_size] (keyType, valType);

      for (a, k, v) in zip(A, keys(), values()) {
        a = (k, v);
      }

      return A;
    }

    /*
      Returns a new 0-based array containing a copy of keys. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] keyType
    */
    proc keysToArray(): [] keyType {
      _enter(); defer _leave();

      if !isCopyableType(keyType) then
        compilerError("keysToArray requires a copyable key type");

      var A: [0..#_size] keyType;
      for (a, k) in zip(A, keys()) {
        a = k;
      }
      return A;
    }

    /*
      Returns a new 0-based array containing a copy of values. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] valType
    */
    proc valuesToArray(): [] valType {
      _enter(); defer _leave();

      if !isCopyableType(valType) then
        compilerError("valuesToArray requires a copyable value type");

      var A: [0..#_size] valType;
      for (a, v) in zip(A, values()) {
        a = v;
      }
      return A;
    }
  } // end record map

  /*
    Replace the content of this map with the other's.

    .. warning::

      This will invalidate any references to elements previously contained in
      `lhs`.

    :arg lhs: The map to assign to.
    :arg rhs: The map to assign from.
  */
  operator map.=(ref lhs: map(?kt, ?vt, ?), const ref rhs: map(kt, vt, ?)) {
    if !isCopyableType(kt) || !isCopyableType(vt) then
      compilerError("assigning map with non-copyable type");

    lhs.clear();
    try! {
      for key in rhs.keys() {
        lhs.add(key, rhs[key]);
      }
    }
  }

  pragma "no doc"
  operator map.:(x: map(?k1, ?v1, ?p1), type t: map(?k2, ?v2, ?p2)) {
    // TODO: Allow coercion between element types? If we do then init=
    // should also be changed accordingly.
    if k1 != k2 then
      compilerError('Cannot cast to map with different ',
                    'key type: ', k2:string);

    if v1 != v2 then
      compilerError('Cannot cast to map with different ',
                    'value type: ', v2:string);

    var result: t = x;
    return result;
  }

  /*
    Returns `true` if the contents of two maps are the same.

    :arg a: A map to compare.
    :type a: map

    :arg b: A map to compare.
    :type b: map (with same keyType and valType)

    :return: `true` if the contents of two maps are equal.
    :rtype: `bool`
  */
  operator map.==(const ref a: map(?kt, ?vt, ?ps),
                  const ref b: map(kt, vt, ps)): bool {
    try! {
      for key in a.keys() {
        if !b.contains(key) || a[key] != b[key] then
          return false;
      }
      for key in b.keys() {
        if !a.contains(key) || a[key] != b[key] then
          return false;
      }
    }
    return true;
  }

  /*
    Returns `true` if the contents of two maps are not the same.

    :arg a: A map to compare.
    :type a: map

    :arg b: A map to compare.
    :type b: map (with same keyType and valType)

    :return: `true` if the contents of two maps are not equal.
    :rtype: `bool`
  */
  operator map.!=(const ref a: map(?kt, ?vt, ?ps),
                  const ref b: map(kt, vt, ps)): bool {
    return !(a == b);
  }

  /*
   A `KeyNotFoundError` is thrown at runtime if an attempt is made
   to access a map value at a given key that is not in the current
   state of the `map`. An example of this is calling `map.getValue()`.
   */
  class KeyNotFoundError : Error {
    proc init() {}

    proc init(k: string) {
      var msg = "key '" + k + "' not found";
      super.init(msg);
    }
  }
}
