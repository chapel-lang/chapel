.. default-domain:: chpl

.. module:: Set
   :synopsis: This module contains the implementation of the set type.

Set
===
**Usage**

.. code-block:: chapel

   use Set;


This module contains the implementation of the set type.

A set is a collection of unique elements. Sets are unordered and unindexed.

The highly parallel nature of Chapel means that great care should be taken
when performing operations that may invalidate references to set elements.
Adding or removing an element from a set may invalidate references to
elements contained in the set.

All references to set elements are invalidated when the set is cleared or
deinitialized.

Sets are not parallel safe by default, but can be made parallel safe by
setting the param formal 'parSafe` to true in any set constructor. When
constructed from another set, the new set will inherit the parallel safety
mode of its originating set.

.. record:: set

   
   A set is a collection of unique elements. Attempting to add a duplicate
   element to a set has no effect.
   
   The set type supports a test for membership via the :proc:`contains`
   method, along with free functions for calculating the union, difference,
   intersection, and symmetric difference of two sets. The set type also
   defines the (proper) subset and (proper) superset operations by
   overloading common comparison operators.
   
   Sets can be iterated over. The set type makes no guarantee of a consistent
   iteration order.
   
   A set can be default initialized (containing no elements), or it may be
   initialized with elements that are copies of those contained by any
   type that supports an iterator.
   
   The set type is not parallel safe by default. For situations in which
   such protections are desirable, parallel safety can be enabled by setting
   `parSafe = true` in any set constructor. A set constructed from another
   set inherits the parallel safety mode of that set by default.
   


   .. attribute:: type eltType

      The type of the elements contained in this set. 

   .. attribute:: param parSafe = false

      If `true`, this set will perform parallel safe operations. 

   .. method:: proc init(type eltType, param parSafe = false)

      
      Initializes an empty set containing elements of the given type.
      
      :arg eltType: The type of the elements of this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
      

   .. method:: proc init(type eltType, iterable, param parSafe = false)

      
      Initialize this set with a unique copy of each element contained in
      `iterable`. If an element from `iterable` is already contained in this
      set, it will not be added again. The formal `iterable` must be a type
      with an iterator named "these" defined for it.
      
      :arg iterable: A collection of elements to add to this set.
      :arg parSafe: If `true`, this set will use parallel safe operations.
      

   .. method:: proc init=(const ref other: ?tset?)

      
      Initialize this set with a copy of each of the elements contained in
      the set `other`. This set will inherit the `parSafe` value of the
      set `other`.
      
      :arg other: A set to initialize this set with.
      

   .. method:: proc add(in x: eltType)

      
      Add a copy of the element `x` to this set. Does nothing if this set
      already contains an element equal to the value of `x`.
      
      :arg x: The element to add to this set.
      

   .. method:: proc contains(const ref x: eltType): bool

      
      Returns `true` if the given element is a member of this set, and `false`
      otherwise.
      
      :arg x: The element to test for membership.
      :return: Whether or not the given element is a member of this set.
      :rtype: `bool`
      

   .. method:: proc isDisjoint(const ref other: eltTypeset?): bool

      
      Returns `true` if this set shares no elements in common with the set
      `other`, and `false` otherwise.
      
      :arg other: The set to compare against.
      :return: Whether or not this set and `other` are disjoint.
      :rtype: `bool`
      

   .. method:: proc isIntersecting(const ref other: eltTypeset?): bool

      
      Returns `true` if this set and `other` have at least one element in
      common, and `false` otherwise.
      
      :arg other: The set to compare against.
      :return: Whether or not this set and `other` intersect.
      :rtype: `bool`
      

   .. method:: proc remove(const ref x: eltType): bool

      
      Attempt to remove the item from this set with a value equal to `x`. If
      an element equal to `x` was removed from this set, return `true`, else
      return `false` if no such value was found.
      
      .. warning::
      
        Removing an element from this set may invalidate existing references
        to the elements contained in this set.
      
      :arg x: The element to remove.
      :return: Whether or not an element equal to `x` was removed.
      :rtype: `bool`
      

   .. method:: proc clear()

      
      Clear the contents of this set.
      
      .. warning::
      
        Clearing the contents of this set will invalidate all existing
        references to the elements contained in this set.
      

   .. itermethod:: iter these()

      
      Iterate over the elements of this set.
      
      .. warning::
      
        Set iterators are currently not threadsafe. Attempting to mutate the
        state of a set while it is being iterated over is considered
        undefined behavior.
      
      :yields: A reference to one of the elements contained in this set.
      

   .. method:: proc writeThis(ch: channel) throws

      
      Write the contents of this set to a channel.
      
      :arg ch: A channel to write to.
      

   .. method:: proc isEmpty(): bool

      
      Returns `true` if this set contains zero elements.
      
      :return: `true` if this set is empty.
      :rtype: `bool`
      

   .. method:: proc size

      
      The current number of elements contained in this set.
      

   .. method:: proc toArray(): [] eltType

      
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this set. The elements of the returned array are
      not guaranteed to follow any particular ordering.
      
      :return: An array containing a copy of each of the elements in this set.
      :rtype: `[] eltType`
      

.. function:: proc =(ref lhs: ?tset?, const ref rhs: tset?)

   
   Clear the contents of the set `lhs`, then iterate through the contents of
   `rhs` and add a copy of each element to `lhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: The set to assign to.
   :arg rhs: The set to assign from. 
   

.. function:: proc |(const ref a: ?tset?, const ref b: tset?): set(t)

   
   Return a new set that contains the union of two sets.
   
   :arg a: A set to take the union of.
   :arg b: A set to take the union of.
   
   :return: A new set containing the union between `a` and `b`.
   :rtype: `set(?t, ?)`
   

.. function:: proc |=(ref lhs: ?tset?, const ref rhs: tset?)

   
   Assign to the set `lhs` the set that is the union of `lhs` and `rhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: A set to take the union of and then assign to.
   :arg rhs: A set to take the union of.
   

.. function:: proc +(const ref a: ?tset?, const ref b: tset?): tset?

   
   Return a new set that contains the union of two sets. Alias for the `|`
   operator.
   
   :arg a: A set to take the union of.
   :arg b: A set to take the union of.
   
   :return: A new set containing the union between `a` and `b`.
   :rtype: `set(?t, ?)`
   

.. function:: proc +=(ref lhs: ?tset?, const ref rhs: tset?)

   
   Assign to the set `lhs` the set that is the union of `lhs` and `rhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: A set to take the union of and then assign to.
   :arg rhs: A set to take the union of.
   

.. function:: proc -(const ref a: ?tset?, const ref b: tset?): set(t)

   
   Return a new set that contains the difference of two sets.
   
   :arg a: A set to take the difference of.
   :arg b: A set to take the difference of.
   
   :return: A new set containing the difference between `a` and `b`.
   :rtype: `set(t)`
   

.. function:: proc -=(ref lhs: ?tset?, const ref rhs: tset?)

   
   Assign to the set `lhs` the set that is the difference of `lhs` and `rhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: A set to take the difference of and then assign to.
   :arg rhs: A set to take the difference of.
   

.. function:: proc &(const ref a: ?tset?, const ref b: tset?): set(t)

   
   Return a new set that contains the intersection of two sets.
   
   :arg a: A set to take the intersection of.
   :arg b: A set to take the intersection of.
   
   :return: A new set containing the intersection of `a` and `b`.
   :rtype: `set(t)`
   

.. function:: proc &=(ref lhs: ?tset?, const ref rhs: tset?)

   
   Assign to the set `lhs` the set that is the intersection of `lhs` and
   `rhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: A set to take the intersection of and then assign to.
   :arg rhs: A set to take the intersection of.
   

.. function:: proc ^(const ref a: ?tset?, const ref b: tset?): set(t)

   
   Return the symmetric difference of two sets.
   
   :arg a: A set to take the symmetric difference of.
   :arg b: A set to take the symmetric difference of.
   
   :return: A new set containing the symmetric difference of `a` and `b`.
   :rtype: `set(?t, ?)`
   

.. function:: proc ^=(ref lhs: ?tset?, const ref rhs: tset?)

   
   Assign to the set `lhs` the set that is the symmetric difference of `lhs`
   and `rhs`.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     the set `lhs`.
   
   :arg lhs: A set to take the symmetric difference of and then assign to.
   :arg rhs: A set to take the symmetric difference of.
   

.. function:: proc ==(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if the sets `a` and `b` are equal. That is, they are the
   same size and contain the same elements.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if two sets are equal.
   :rtype: `bool`
   

.. function:: proc !=(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if the sets `a` and `b` are not equal.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if two sets are not equal.
   :rtype: `bool`
   

.. function:: proc <(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if `a` is a proper subset of `b`.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if `a` is a proper subset of `b`.
   :rtype: `bool`
   

.. function:: proc <=(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if `a` is a subset of `b`.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if `a` is a subset of `b`.
   :rtype: `bool`
   

.. function:: proc >(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if `a` is a proper superset of `b`.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if `a` is a proper superset of `b`.
   :rtype: `bool`
   

.. function:: proc >=(const ref a: ?tset?, const ref b: tset?): bool

   
   Return `true` if `a` is a superset of `b`.
   
   :arg a: A set to compare.
   :arg b: A set to compare.
   
   :return: `true` if `a` is a superset of `b`.
   :rtype: `bool`
   

