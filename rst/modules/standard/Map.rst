.. default-domain:: chpl

.. module:: Map
   :synopsis: This module contains the implementation of the map type which is a container

Map
===
**Usage**

.. code-block:: chapel

   use Map;


This module contains the implementation of the map type which is a container
that stores key-value associations. 

Maps are not parallel safe by default, but can be made parallel safe by
setting the param formal `parSafe` to true in any map constructor. When
constructed from another map, the new map will inherit the parallel safety
mode of its originating map.

.. record:: map

   .. attribute:: type keyType

      Type of map keys. 

   .. attribute:: type valType

      Type of map values. 

   .. attribute:: param parSafe = false

      If `true`, this map will perform parallel safe operations. 

   .. method:: proc init(type keyType, type valType, param parSafe = false)

      
      Initializes an empty map containing keys and values of given types.
      
      :arg keyType: The type of the keys of this map.
      :arg valType: The type of the values of this map.
      :arg parSafe: If `true`, this map will use parallel safe operations.
      

   .. method:: proc init=(other: map(?kt, ?vt, ?ps))

      
      Initializes a map containing elements that are copy initialized from
      the elements contained in another map.
      
      :arg other: The map to initialize from.
      :type other: map
      
      :arg parSafe: If `true`, this map will use parallel safe operations.
      :type parSafe: bool
      

   .. method:: proc clear()

      
      Clears the contents of this map.
      
      .. warning::
      
        Clearing the contents of this map will invalidate all existing
        references to the elements contained in this map.
      

   .. method:: proc size

      
      The current number of keys contained in this map.
      

   .. method:: proc isEmpty(): bool

      
      Returns `true` if this map contains zero keys.
      
      :returns: `true` if this map is empty.
      :rtype: `bool`
      

   .. method:: proc contains(const k: keyType): bool

      
      Returns `true` if the given key is a member of this map, and `false`
      otherwise.
      
      :arg k: The key to test for membership.
      :type k: keyType
      
      :returns: Whether or not the given key is a member of this map.
      :rtype: `bool`
      

   .. method:: proc update(const ref m: map(keyType, valType, parSafe))

      
      Updates this map with the contents of the other, overwriting the values
      for already-existing keys.
      
      :arg m: The other map
      :type m: map(keyType, valType)
      

   .. method:: proc this(k: keyType) ref

      
      Get the value mapped to the given key, or add the mapping if key does not
      exist.
      
      :arg k: The key to access
      :type k: keyType
      
      :returns: Reference to the value mapped to the given key.
      

   .. itermethod:: iter these() const ref

      
      Iterates over the keys of this map. This is a shortcut for :iter:`keys`.
      
      :yields: A reference to one of the keys contained in this map.
      

   .. itermethod:: iter keys() const ref

      
      Iterates over the keys of this map.
      
      :yields: A reference to one of the keys contained in this map.
      

   .. itermethod:: iter items() const ref

      
      Iterates over the key-value pairs of this map.
      
      :yields: A tuple of references to one of the key-value pairs contained in
               this map.
      

   .. itermethod:: iter values() ref

      
      Iterates over the values of this map.
      
      :yields: A reference to one of the values contained in this map.
      

   .. method:: proc readWriteThis(ch: channel) throws

      
      Writes the contents of this map to a channel. The format looks like:
      
        .. code-block:: chapel
      
           {k1: v1, k2: v2, .... , kn: vn}
      
      :arg ch: A channel to write to.
      

   .. method:: proc add(k: keyType, v: valType): bool

      
       Adds a key-value pair to the map. Method returns `false` if the key
       already exists in the map.
      
      :arg k: The key to add to the map
      :type k: keyType
      
      :arg v: The value that maps to ``k``
      :type k: valueType
      
      :returns: `true` if `k` was not in the map and added with value `v`.
                `false` otherwise.
      :rtype: bool
      

   .. method:: proc set(k: keyType, v: valType): bool

      
       Sets the value associated with a key. Method returns `false` if the key
       does not exist in the map.
      
      :arg k: The key whose value needs to change
      :type k: keyType
      
      :arg v: The desired value to the key ``k``
      :type k: valueType
      
      :returns: `true` if `k` was in the map and its value is updated with `v`.
                `false` otherwise.
      :rtype: bool
      

   .. method:: proc remove(k: keyType): bool

      
       Removes a key-value pair from the map, with the given key.
       
      :arg k: The key to remove from the map
      :type k: keyType
      
      :returns: `false` if `k` was not in the map.  `true` if it was and removed.
      :rtype: bool
      

   .. method:: proc toArray(): [] (keyType, valType)

      
      Returns a new 1-based array containing a copy of key-value pairs as
      tuples.
      
      :return: A new DefaultRectangular array.
      :rtype: [] (keyType, valType)
      

   .. method:: proc keysToArray(): [] keyType

      
      Returns a new 1-based array containing a copy of keys. Array is not
      guaranteed to be in any particular order.
      
      :return: A new DefaultRectangular array.
      :rtype: [] keyType
      

   .. method:: proc valuesToArray(): [] valType

      
      Returns a new 1-based array containing a copy of values. Array is not
      guaranteed to be in any particular order.
      
      :return: A new DefaultRectangular array.
      :rtype: [] valType
      

.. function:: proc =(ref lhs: map(?kt, ?vt, ?ps), const ref rhs: map(kt, vt, ps))

   
   Replace the content of this map with the other's.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     `lhs`.
   
   :arg lhs: The map to assign to.
   :arg rhs: The map to assign from. 
   

.. function:: proc ==(const ref a: map(?kt, ?vt, ?ps), const ref b: map(kt, vt, ps)): bool

   
   Returns `true` if the contents of two maps are the same.
   
   :arg a: A map to compare.
   :type a: map
   
   :arg b: A map to compare.
   :type b: map (with same keyType and valType)
   
   :return: `true` if the contents of two maps are equal.
   :rtype: `bool`
   

.. function:: proc !=(const ref a: map(?kt, ?vt, ?ps), const ref b: map(kt, vt, ps)): bool

   
   Returns `true` if the contents of two maps are not the same.
   
   :arg a: A map to compare.
   :type a: map
   
   :arg b: A map to compare.
   :type b: map (with same keyType and valType)
   
   :return: `true` if the contents of two maps are not equal.
   :rtype: `bool`
   

.. function:: proc +(a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Returns a new map containing the keys and values in either a or b. 

.. function:: proc +=(ref a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   
   Sets the left-hand side map to contain the keys and values in either
   a or b.
   

.. function:: proc |(a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Returns a new map containing the keys and values in either a or b. 

.. function:: proc |=(ref a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Sets the left-hand side map to contain the keys and values in either
   a or b.
   

.. function:: proc &(a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Returns a new map containing the keys that are in both a and b. 

.. function:: proc &=(ref a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Sets the left-hand side map to contain the keys that are in both a and b.
   

.. function:: proc -(a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Returns a new map containing the keys that are only in a, but not b. 

.. function:: proc -=(ref a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Sets the left-hand side map to contain the keys that are in the
   left-hand map, but not the right-hand map. 

.. function:: proc ^(a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Returns a new map containing the keys that are in either a or b, but
   not both. 

.. function:: proc ^=(ref a: map(?keyType, ?valueType, ?parSafe), b: map(keyType, valueType, parSafe))

   Sets the left-hand side map to contain the keys that are in either the
   left-hand map or the right-hand map, but not both. 

