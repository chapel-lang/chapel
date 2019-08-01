/*
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

  The following operations may invalidate references to elements contained in
  a map:

      - indexing using a non-existing key (e.g. `myMap[key] = value`)
      - add
      - remove
      - clear
      - update

  Maps are not parallel safe by default, but can be made parallel safe by
  setting the param formal `parSafe` to true in any map constructor. When
  constructed from another map, the new map will inherit the parallel safety
  mode of its originating map.
*/
module Maps {

  record map {

    /*
      Initializes an empty map containing keys and values of given types.

      :arg keyType: The type of the keys of this map.
      :arg valType: The type of the values of this map.
      :arg parSafe: If `true`, this map will use parallel safe operations.
    */
    proc init(type keyType, type valType, param parSafe=false) {

    }

    /*
      Initializes a map containing elements that are copy initialized from
      the elements contained in another map.

      :arg other: The map to initialize from.
      :type other: map

      :arg parSafe: If `true`, this map will use parallel safe operations.
      :type parSafe: bool
    */
    proc init=(const ref other: map(?kt, ?vt), param parSafe=other.parSafe) {

    }

    /*
      Clears the contents of this map.

      .. warning::

        Clearing the contents of this map will invalidate all existing
        references to the elements contained in this map.
    */
    proc clear() {

    }

    /*
      The current number of keys contained in this map.
    */
    inline proc const size {

    }

    /*
      Returns `true` if this map contains zero keys.

      :returns: `true` if this map is empty.
      :rtype: `bool`
    */
    inline proc const isEmpty(): bool {

    }

    /*
      Returns `true` if the given key is a member of this map, and `false`
      otherwise.

      :arg k: The key to test for membership.
      :type k: keyType

      :returns: Whether or not the given key is a member of this map.
      :rtype: `bool`
    */
    proc const contains(const ref k: keyType): bool {

    }

    /*
      Updates this map with the contents of the other, overwritting the values
      for already-existing keys.

      :arg m: The other map
      :type m: map(keyType, valType)
    */
    proc update(const ref m: zap(keyType,valType)) {

    }

    /*
      Get a reference to the value mapped to the given key.

      :arg k: The key to access
      :type k: keyType

      .. warning::

        Use of the `this` method with a non-existent key (while bounds checking
        is on) will cause the currently running program to halt.

      :returns: Reference to the value mapped to the given key.
    */
    proc const this(k: keyType) ref {

    }

    /*
      Iterates over the keys of this map.

      :yields: A reference to one of the keys contained in this map.
    */
    iter these() const ref {

    }

    /*
      Iterates over the key-value pairs of this map.

      :yields: A tuple of references to one of the key-value pairs contained in
               this map.
    */
    iter items() const ref {

    }

    /*
      Iterates over the values of this map.

      :yields: A reference to one of the values contained in this map.
    */
    iter values() ref {

    }

    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel
    
           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    proc writeThis(ch: channel) {

    }

    /*
      Adds a key-value pair to the map. Different than doing `myMap[key] =
      value`, this method returns `false` if the key already exists in the map.

     :arg k: The key to add to the map
     :type k: keyType

     :arg v: The value that maps to ``k``
     :type k: valueType

     :returns: `true` if `k` was not in the map and added with value `v`.
               `false` otherwise.
     :rtype: bool
    */
    proc add(k: keyType, v: valType): bool {

    }

    /*
      Removes a key-value pair to the map, withe the given key.
      
     :arg k: The key to remove from the map
     :type k: keyType

     :returns: `false` if `k` was not in the map.  `true` if it was and removed.
     :rtype: bool
    */
    proc remove(k: keyType): bool {

    }

    /*
      Returns a new 0-based array containing a copy of key-value pairs as
      tuples.

      :return: A new DefaultRectangular array.
      :rtype: [] (keyType, valType)
    */
    proc toArray(): [] (keyType, valType) {

    }

    /*
      Returns a new 0-based array containing a copy of keys.

      :return: A new DefaultRectangular array.
      :rtype: [] keyType
    */
    proc keysToArray(): [] keyType {

    }

    /*
      Returns a new 0-based array containing a copy of values.

      :return: A new DefaultRectangular array.
      :rtype: [] valType
    */
    proc valuesToArray(): [] valType {

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
  proc =(ref lhs: map(?kt, ?vt), const ref rhs: map(kt, vt)){

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
  proc ==(const ref a: map(?k, ?v), const ref b: map(?k, ?v)): bool {

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
  proc !=(const ref a: map(?k, ?v), const ref b: map(?k, ?v)): bool {

  }

} //end module Maps
