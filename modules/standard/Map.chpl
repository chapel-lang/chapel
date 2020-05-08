/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

/*
  This module contains the implementation of the map type which is a container
  that stores key-value associations. 

  Maps are not parallel safe by default, but can be made parallel safe by
  setting the param formal `parSafe` to true in any map constructor. When
  constructed from another map, the new map will inherit the parallel safety
  mode of its originating map.
*/
module Map {
  import ChapelLocks;
  private use HaltWrappers;

  // Lock code lifted from modules/standard/Lists.chpl.
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

  private use IO;

  pragma "no doc"
  inline proc checkForNonNilableClass(type t) type {
    if isNonNilableClass(t) {
      return t?;
    } else {
      return t;
    }
  }

  record map {
    /* Type of map keys. */
    type keyType;
    /* Type of map values. */
    type valType;

    /* If `true`, this map will perform parallel safe operations. */
    param parSafe = false;

    pragma "no doc"
    var myKeys: domain(keyType, parSafe=parSafe);
    pragma "no doc"
    var vals: [myKeys] checkForNonNilableClass(valType);


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
      :arg parSafe: If `true`, this map will use parallel safe operations.
    */
    proc init(type keyType, type valType, param parSafe=false) {
      if isGenericType(keyType) {
        compilerWarning("creating a map with key type " +
                        keyType:string);
        if isClassType(keyType) && !isGenericType(borrowed keyType) {
          compilerWarning("which now means class type with generic management");
        }
        compilerError("map key type cannot currently be generic");
        // In the future we might support it if the list is not default-inited
      }
      if isGenericType(valType) {
        compilerWarning("creating a map with value type " +
                        valType:string);
        if isClassType(valType) && !isGenericType(borrowed valType) {
          compilerWarning("which now means class type with generic management");
        }
        compilerError("map value type cannot currently be generic");
        // In the future we might support it if the list is not default-inited
      }
      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = parSafe;
    }

    proc init(type keyType, type valType, param parSafe=false)
    where isNonNilableClass(valType) {
      if isGenericType(keyType) {
        compilerWarning("creating a map with key type " +
                        keyType:string);
        if isClassType(keyType) && !isGenericType(borrowed keyType) {
          compilerWarning("which now means class type with generic management");
        }
        compilerError("map key type cannot currently be generic");
        // In the future we might support it if the list is not default-inited
      }
      if isGenericType(valType) {
        compilerWarning("creating a map with value type " +
                        valType:string);
        if isClassType(valType) && !isGenericType(borrowed valType) {
          compilerWarning("which now means class type with generic management");
        }
        compilerError("map value type cannot currently be generic");
        // In the future we might support it if the list is not default-inited
      }

      this.keyType = keyType;
      this.valType = valType;
      this.parSafe = parSafe;
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
      this.keyType = kt;
      this.valType = vt;
      this.parSafe = ps;

      this.complete();

      for key in other {
        myKeys += key;
        vals[key] = other.vals[key];
      }
    }

    /*
      Clears the contents of this map.

      .. warning::

        Clearing the contents of this map will invalidate all existing
        references to the elements contained in this map.
    */
    proc clear() {
      _enter();
      myKeys.clear();
      _leave();
    }

    /*
      The current number of keys contained in this map.
    */
    inline proc const size {
      _enter();
      var result = myKeys.size;
      _leave();
      return result;
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
      _enter();
      var result = myKeys.contains(k);
      _leave();
      return result;
    }

    /*
      Updates this map with the contents of the other, overwriting the values
      for already-existing keys.

      :arg m: The other map
      :type m: map(keyType, valType)
    */
    proc update(const ref m: map(keyType, valType, parSafe)) {
      _enter();
      for key in m {
        if !myKeys.contains(key) then
          myKeys += key;
        vals[key] = m.vals[key];
      }
      _leave();
    }

    /*
      Get the value mapped to the given key, or add the mapping if key does not
      exist.

      :arg k: The key to access
      :type k: keyType

      :returns: Reference to the value mapped to the given key.
    */
    proc this(k: keyType) ref where !isNonNilableClass(valType) {
      _enter();
      var (found, slotNum) = myKeys._value._findFilledSlot(k, needLock=false);

      if found {
        ref result = vals._value.data[slotNum];
        _leave();
        return result;
      } else if slotNum != -1 {
        const (newSlot, _) = myKeys._value._addWrapper(k, slotNum, needLock=false);
        ref result = vals._value.data[newSlot];
        _leave();
        return result;
      } else {
        boundsCheckHalt("map index out of bounds: " + k:string);
        ref result = vals._value.data[0];
        _leave();
        return result;
      }
    }

    pragma "no doc"
    proc const this(k: keyType) const
    where shouldReturnRvalueByValue(valType) && !isNonNilableClass(valType) {
      _enter();
      if !myKeys.contains(k) then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      const result = vals[k];
      _leave();
      return result;
    }

    pragma "no doc"
    proc const this(k: keyType) const ref
    where shouldReturnRvalueByConstRef(valType) && !isNonNilableClass(valType) {
      _enter();
      if !myKeys.contains(k) then
        halt("map index ", k, " out of bounds");
      const ref result = vals[k];
      _leave();
      return result;
    }

    pragma "no doc"
    proc const this(k: keyType)
    where isNonNilableClass(valType) {
      compilerError("Cannot access nilable class directly. Use an",
                    " appropriate accessor method instead.");
    }

    /* Get a borrowed reference to the element at position `k`.
     */
    proc getBorrowed(k: keyType) where isClass(valType) {
      _enter();
      if !myKeys.contains(k) then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        var result = vals[k].borrow();
        _leave();
        if isNonNilableClass(valType) {
          return result!;
        } else {
          return result;
        }
      }
    }

    /* Get a reference to the element at position `k`. This method is not
       available for non-nilable types.
     */
    proc getReference(k: keyType) ref
    where !isNonNilableClass(valType) {
      _enter();
      if !myKeys.contains(k) then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        ref result = vals[k];
        _leave();
        return result;
      }
    }

    proc getValue(k: keyType) const
    where isNonNilableClass(valType) {
      _enter();
      if !myKeys.contains(k) then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        const result = vals[k]: valType;
        _leave();
        return result;
      }
    }

    /* Remove the element at position `k` from the map and return its value
     */
    proc getAndRemove(k: keyType) {
      _enter();
      if !myKeys.contains(k) then
        boundsCheckHalt("map index " + k:string + " out of bounds");
      try! {
        const ref result = vals[k]: valType;
        myKeys.remove(k);
        _leave();
        return result: valType;
      }
    }

    /*
      Iterates over the keys of this map. This is a shortcut for :iter:`keys`.

      :yields: A reference to one of the keys contained in this map.
    */
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
      for key in myKeys {
        yield key;
      }
    }

    /*
      Iterates over the key-value pairs of this map.

      :yields: A tuple of references to one of the key-value pairs contained in
               this map.
    */
    iter items() const ref {
      for key in myKeys {
        yield (key, vals[key]);
      }
    }

    pragma "no doc"
    iter items() const ref where isNonNilableClass(valType) {
      try! {
        for key in myKeys {
          yield (key, vals[key]: valType);
        }
      }
    }

    /*
      Iterates over the values of this map.

      :yields: A reference to one of the values contained in this map.
    */
    iter values() ref
    where !isNonNilableClass(valType) {
      for val in vals {
        yield val;
      }
    }

    pragma "no doc"
    iter values() const where isNonNilableClass(valType) {
      try! {
        for val in vals {
          yield val: valType;
        }
      }
    }

    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel
    
           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    proc readWriteThis(ch: channel) throws {
      _enter();
      var first = true;
      //try! {
        ch <~> "{";
        for key in myKeys {
          if first {
            first = false;
          } else {
            ch <~> ", ";
          }
          ch <~> key <~> ": " <~> vals[key];
        }
        ch <~> "}";
      //}
      _leave();
    }

    /*
      Adds a key-value pair to the map. Method returns `false` if the key
      already exists in the map.

     :arg k: The key to add to the map
     :type k: keyType

     :arg v: The value that maps to ``k``
     :type k: valueType

     :returns: `true` if `k` was not in the map and added with value `v`.
               `false` otherwise.
     :rtype: bool
    */
    proc add(in k: keyType, in v: valType): bool lifetime this < v {
      _enter();
      if myKeys.contains(k) {
        _leave();
        return false;
      }

      myKeys += k;
      vals[k] = v;

      _leave();
      return true;
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
    proc set(k: keyType, in v: valType): bool {
      _enter();
      if !myKeys.contains(k) {
        _leave();
        return false;
      }

      vals[k] = v;

      _leave();
      return true;
    }

    /* If the map doesn't contain a value at position `k` add one and
       set it to `v`. If the map already contains a value at position
       `k`, update it to the value `v`.
     */
    proc addOrSet(in k: keyType, in v: valType) {
      if myKeys.contains(k) {
        this.set(k, v);
      } else {
        this.add(k, v);
      }
    }

    /*
      Removes a key-value pair from the map, with the given key.
      
     :arg k: The key to remove from the map
     :type k: keyType

     :returns: `false` if `k` was not in the map.  `true` if it was and removed.
     :rtype: bool
    */
    proc remove(k: keyType): bool {
      _enter();
      if !myKeys.contains(k) {
        _leave();
        return false;
      }
      myKeys -= k;
      _leave();
      return true;
    }

    /*
      Returns a new 0-based array containing a copy of key-value pairs as
      tuples.

      :return: A new DefaultRectangular array.
      :rtype: [] (keyType, valType)
    */
    proc toArray(): [] (keyType, valType) {
      _enter();
      var A: [0..#myKeys.size] (keyType, valType);
      for (a, key) in zip(A, myKeys) {
        a = (key, vals[key]);
      }
      _leave();
      return A;
    }

    /*
      Returns a new 0-based array containing a copy of keys. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] keyType
    */
    proc keysToArray(): [] keyType {
      _enter();
      var A: [0..#myKeys.size] keyType;
      for (a, k) in zip(A, myKeys) {
        a = k;
      }
      _leave();
      return A;
    }

    /*
      Returns a new 0-based array containing a copy of values. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] valType
    */
    proc valuesToArray(): [] valType {
      _enter();
      var A: [0..#vals.size] valType;
      for (a, v) in zip(A, vals) {
        a = v;
      }
      _leave();
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
  proc =(ref lhs: map(?kt, ?vt, ?ps), const ref rhs: map(kt, vt, ps)){
    lhs.clear();

    for key in rhs.myKeys {
      lhs.add(key, rhs.vals[key]);
    }
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
  proc ==(const ref a: map(?kt, ?vt, ?ps), const ref b: map(kt, vt, ps)): bool {
    for key in a {
      if !b.contains(key) || a.vals[key] != b.vals[key] then
        return false;
    }
    for key in b {
      if !a.contains(key) || a.vals[key] != b.vals[key] then
        return false;
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
  proc !=(const ref a: map(?kt, ?vt, ?ps), const ref b: map(kt, vt, ps)): bool {
    return !(a == b);
  }

  /* Returns a new map containing the keys and values in either a or b. */
  proc +(a: map(?keyType, ?valueType, ?parSafe),
         b: map(keyType, valueType, parSafe)) {
    return a | b;
  }

  /*
    Sets the left-hand side map to contain the keys and values in either
    a or b.
   */
  proc +=(ref a: map(?keyType, ?valueType, ?parSafe),
          b: map(keyType, valueType, parSafe)) {
    a |= b;
  }

  /* Returns a new map containing the keys and values in either a or b. */
  proc |(a: map(?keyType, ?valueType, ?parSafe),
         b: map(keyType, valueType, parSafe)) {
    var newMap = new map(keyType, valueType, parSafe);
    newMap.myKeys = a.myKeys | b.myKeys;

    for k in b do newMap[k] = b.vals[k];
    for k in a do newMap[k] = a.vals[k];
    return newMap;
  }

  /* Sets the left-hand side map to contain the keys and values in either
     a or b.
   */
  proc |=(ref a: map(?keyType, ?valueType, ?parSafe),
          b: map(keyType, valueType, parSafe)) {
    // add keys/values from b to a if they weren't already in a
    for k in b do a.add(k, b.vals[k]);
  }

  /* Returns a new map containing the keys that are in both a and b. */
  proc &(a: map(?keyType, ?valueType, ?parSafe),
         b: map(keyType, valueType, parSafe)) {
    var newMap = new map(keyType, valueType, parSafe);
    newMap.myKeys = a.myKeys & b.myKeys;

    for k in newMap do newMap[k] = a.vals[k];
    return newMap;
  }

  /* Sets the left-hand side map to contain the keys that are in both a and b.
   */
  proc &=(ref a: map(?keyType, ?valueType, ?parSafe),
          b: map(keyType, valueType, parSafe)) {
    for k in a {
      if !b.contains(k) then a.remove(k);
    }
  }

  /* Returns a new map containing the keys that are only in a, but not b. */
  proc -(a: map(?keyType, ?valueType, ?parSafe),
         b: map(keyType, valueType, parSafe)) {
    var newMap = new map(keyType, valueType, parSafe);
    newMap.myKeys = a.myKeys - b.myKeys;

    for k in newMap do newMap[k] = a.vals[k];

    return newMap;
  }

  /* Sets the left-hand side map to contain the keys that are in the
     left-hand map, but not the right-hand map. */
  proc -=(ref a: map(?keyType, ?valueType, ?parSafe),
          b: map(keyType, valueType, parSafe)) {
    for k in a do
      if b.contains(k) then a.remove(k);
  }

  /* Returns a new map containing the keys that are in either a or b, but
     not both. */
  proc ^(a: map(?keyType, ?valueType, ?parSafe),
         b: map(keyType, valueType, parSafe)) {
    var newMap = new map(keyType, valueType, parSafe);
    newMap.myKeys = a.myKeys ^ b.myKeys;

    for k in a do
      if !b.contains(k) then newMap[k] = a.vals[k];
    for k in b do
      if !a.contains(k) then newMap[k] = b.vals[k];
    return newMap;
  }

  /* Sets the left-hand side map to contain the keys that are in either the
     left-hand map or the right-hand map, but not both. */
  proc ^=(ref a: map(?keyType, ?valueType, ?parSafe),
          b: map(keyType, valueType, parSafe)) {
    for k in b {
      if a.contains(k) then a.remove(k);
      else a[k] = b.vals[k];
    }
  }
}
