.. default-domain:: chpl

.. module:: List
   :synopsis: This module contains the implementation of the list type.

List
====
**Usage**

.. code-block:: chapel

   use List;


This module contains the implementation of the list type.

A list is a lightweight container similar to an array that is suitable for
building up and iterating over a collection of elements in a structured
manner.

The highly parallel nature of Chapel means that great care should be taken
when performing operations that may invalidate references to list elements.
Inserts and removals into the middle of a list are example operations that
may invalidate references. Appending an element to the end of a list will
never invalidate references to elements contained in the list.

The following operations may invalidate references to elements contained in
a list:

    - insert
    - remove
    - sort
    - pop
    - clear

Additionally, all references to list elements are invalidated when the list
is deinitialized.

Lists are not parallel safe by default, but can be made parallel safe by
setting the param formal `parSafe` to true in any list constructor. When
constructed from another list, the new list will inherit the parallel safety
mode of its originating list.

Inserts and removals into a list are O(n) worst case and should be performed
with care. Appends into a list have an amortized speed of O(1). Indexing
into a list is O(1).

.. record:: list

   
   A list is a lightweight container suitable for building up and iterating
   over a collection of elements in a structured manner. Unlike a stack, the
   list type also supports inserts or removals into the middle of the list.
   The list type is close in spirit to its Python counterpart, with fast O(1)
   random access and append operations.
   
   The list type is not parallel safe by default. For situations in which
   such protections are desirable, parallel safety can be enabled by setting
   `parSafe = true` in any list constructor.
   
   Unlike an array, the set of indices of a list is always `1..size`.
   


   .. attribute:: type eltType

      The type of the elements contained in this list. 

   .. attribute:: param parSafe = false

      If `true`, this list will perform parallel safe operations. 

   .. method:: proc init(type eltType, param parSafe = false)

      
      Initializes an empty list.
      
      :arg eltType: The type of the elements of this list.
      
      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
      

   .. method:: proc init(other: list(?t), param parSafe = false)

      
      Initializes a list containing elements that are copy initialized from
      the elements contained in another list.
      
      Used in new expressions.
      
      :arg other: The list to initialize from.
      
      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
      

   .. method:: proc init(other: [?d] ?t, param parSafe = false)

      
      Initializes a list containing elements that are copy initialized from
      the elements contained in an array.
      
      Used in new expressions.
      
      :arg other: The array to initialize from.
      
      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
      

   .. method:: proc init(other: range(?t), param parSafe = false)

      
      Initializes a list containing elements that are copy initialized from
      the elements yielded by a range.
      
      Used in new expressions.
      
      .. note::
      
        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.
      
      :arg other: The range to initialize from.
      
      :arg parSafe: If `true`, this list will use parallel safe operations.
      :type parSafe: `param bool`
      

   .. method:: proc init=(other: this.type .eltTypelist?p)

      
      Initializes a list containing elements that are copy initialized from
      the elements contained in another list.
      
      :arg other: The list to initialize from.
      

   .. method:: proc init=(other: [?d] this.type .eltType)

      
      Initializes a list containing elements that are copy initialized from
      the elements contained in an array.
      
      :arg other: The array to initialize from.
      

   .. method:: proc init=(other: range(this.type .eltType, ?b, ?d))

      
      Initializes a list containing elements that are copy initialized from
      the elements yielded by a range.
      
      .. note::
      
        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.
      
      :arg other: The range to initialize from.
      :type other: `range(this.type.eltType)`
      

   .. method:: proc ref append(in x: eltType)

      
      Add an element to the end of this list.
      
      :arg x: An element to append.
      :type x: `eltType`
      

   .. method:: proc contains(x: eltType): bool

      
      Returns `true` if this list contains an element equal to the value of
      `x`, and `false` otherwise.
      
      :arg x: An element to search for.
      :type x: `eltType`
      
      :return: `true` if this list contains `x`.
      :rtype: `bool`
      

   .. method:: proc ref first() ref throws

      
      Returns a reference to the first item in this list.
      
      .. warning::
      
        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.
      
      :return: A reference to the first item in this list.
      :rtype: `ref eltType`
      

   .. method:: proc ref last() ref

      
      Returns a reference to the last item in this list.
      
      .. warning::
      
        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.
      
      :return: A reference to the last item in this list.
      :rtype: `ref eltType`
      

   .. method:: proc ref extend(other: eltTypelist?p)

      
      Extend this list by appending a copy of each element contained in
      another list.
      
      :arg other: A list containing elements of the same type as those
        contained in this list.
      :type other: `list(eltType)`
      

   .. method:: proc ref extend(other: [?d] eltType)

      
      Extend this list by appending a copy of each element contained in an
      array.
      
      :arg other: An array containing elements of the same type as those
        contained in this list.
      :type other: `[?d] eltType`
      

   .. method:: proc ref extend(other: range(eltType, ?b, ?d))

      
      Extends this list by appending a copy of each element yielded by a
      range.
      
      .. note::
      
        Attempting to initialize a list from an unbounded range will trigger
        a compiler error.
      
      :arg other: The range to initialize from.
      :type other: `range(eltType)`
      

   .. method:: proc ref insert(idx: int, in x: eltType): bool

      
      Insert an element at a given position in this list, shifting all elements
      currently at and following that index one to the right. The call
      ``a.insert(1, x)`` inserts an element at the front of the list `a`, and
      ``a.insert((a.size + 1), x)`` is equivalent to ``a.append(x)``.
      
      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.
      
      .. warning::
      
        Inserting an element into this list may invalidate existing references
        to the elements contained in this list.
      
      :arg idx: The index into this list at which to insert.
      :type idx: `int`
      
      :arg x: The element to insert.
      :type x: `eltType`
      
      :return: `true` if `x` was inserted, `false` otherwise.
      :rtype: `bool`
      

   .. method:: proc ref insert(idx: int, arr: [?d] eltType): bool

      
      Insert an array of elements `arr` into this list at index `idx`,
      shifting all elements at and following the index `arr.size` positions
      to the right. 
      
      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.
      
      .. warning::
      
        Inserting elements into this list may invalidate existing references
        to the elements contained in this list.
      
      :arg idx: The index into this list at which to insert.
      :type idx: `int`
      
      :arg arr: An array of elements to insert.
      :type x: `[] eltType`
      
      :return: `true` if `arr` was inserted, `false` otherwise.
      :rtype: `bool`
      

   .. method:: proc ref insert(idx: int, lst: list(eltType)): bool

      
      Insert a list of elements `lst` into this list at index `idx`, shifting
      all elements at and following the index `lst.size` positions to the
      right.
      
      If the insertion is successful, this method returns `true`. If the given
      index is out of bounds, this method does nothing and returns `false`.
      
      .. warning::
      
        Inserting elements into this list may invalidate existing references
        to the elements contained in this list.
      
      :arg idx: The index into this list at which to insert.
      :type idx: `int`
      
      :arg lst: A list of elements to insert.
      :type lst: `list(eltType)`
      
      :return: `true` if `lst` was inserted, `false` otherwise.
      :rtype: `bool`
      

   .. method:: proc ref remove(x: eltType, count: int = 1): int

      
      Remove the first `count` elements from this list with values equal to
      `x`, shifting all elements following the removed item left.
      
      If the count of elements to remove is less than or equal to zero, then
      all elements from this list equal to the value of `x` will be removed.
      
      .. warning::
      
        Removing elements from this list may invalidate existing references
        to the elements contained in this list.
      
      :arg x: The value of the element to remove.
      :type x: `eltType`
      
      :arg count: The number of elements to remove.
      :type count: `int`
      
      :return: The number of elements removed.
      :rtype: `int`
      

   .. method:: proc ref pop(): eltType

      
      Remove the element at the end of this list and return it.
      
      .. warning::
      
        Popping an element from this list will invalidate any reference to
        the element taken while it was contained in this list.
      
      .. warning::
      
        Calling this method on an empty list will cause the currently running
        program to halt. If the `--fast` flag is used, no safety checks will
        be performed.
      
      :return: The element popped.
      :rtype: `eltType`
      

   .. method:: proc ref pop(idx: int): eltType

      
      Remove the element at the index `idx` from this list and return it. The
      elements at indices after `idx` are shifted one to the left in memory,
      making this operation O(n).
      
      .. warning::
      
        Popping an element from this list will invalidate any reference to
        the element taken while it was contained in this list.
      
      .. warning::
      
        Calling this method on an empty list or with values of `idx` that
        are out of bounds will cause the currently running program to halt.
        If the `--fast` flag is used, no safety checks will be performed.
      
      :arg idx: The index of the element to remove.
      :type idx: `int`
      
      :return: The element popped.
      :rtype: `eltType`
      

   .. method:: proc ref clear()

      
      Clear the contents of this list.
      
      .. warning::
      
        Clearing the contents of this list will invalidate all existing
        references to the elements contained in this list.
      

   .. method:: proc indexOf(x: eltType, start: int = 1, end: int = 0): int

      
      Return a one-based index into this list of the first item whose value
      is equal to `x`. If no such element can be found this method returns
      the value `-1`.
      
      .. warning::
      
        Calling this method on an empty list or with values of `start` or 
        `end` that are out of bounds will cause the currently running program
        to halt. If the `--fast` flag is used, no safety checks will be
        performed.
      
      :arg x: An element to search for.
      :type x: `eltType`
      
      :arg start: The start index to start searching from.
      :type start: `int`
      
      :arg end: The end index to stop searching at. A value less than or equal
                to `0` will search the entire list.
      :type end: `int`
      
      :return: The index of the element to search for, or `-1` on error.
      :rtype: `int`
      

   .. method:: proc count(x: eltType): int

      
      Return the number of times a given element is found in this list.
      
      :arg x: An element to count.
      :type x: `eltType`
      
      :return: The number of times a given element is found in this list.
      :rtype: `int`
      

   .. method:: proc ref sort(comparator: ?rec = Sort.defaultComparator)

      
      Sort the items of this list in place using a comparator. If no comparator
      is provided, sort this list using the default sort order of its elements.
      
      .. warning::
      
        Sorting the elements of this list may invalidate existing references
        to the elements contained in this list.
      
      :arg comparator: A comparator used to sort this list.
      

   .. method:: proc ref this(i: int) ref

      
      Index this list via subscript. Returns a reference to the element at a
      given index in this list.
      
      :arg i: The index of the element to access.
      
      .. warning::
      
        Use of the `this` method with an out of bounds index (while bounds
        checking is on) will cause the currently running program to halt.
      
      :return: An element from this list.
      

   .. method:: proc this(i: int) const ref

   .. itermethod:: iter these() ref

      
      Iterate over the elements of this list.
      
      :yields: A reference to one of the elements contained in this list.
      

   .. method:: proc readWriteThis(ch: channel) throws

      
      Write the contents of this list to a channel.
      
      :arg ch: A channel to write to.
      

   .. method:: proc isEmpty(): bool

      
      Returns `true` if this list contains zero elements.
      
      :return: `true` if this list is empty.
      :rtype: `bool`
      

   .. method:: proc size

      
      The current number of elements contained in this list.
      

   .. method:: proc indices

      
      Returns the list's legal indices as the range ``1..this.size``.
      
      :return: ``1..this.size``
      :rtype: `range`
      

   .. method:: proc toArray(): [] eltType

      
      Returns a new DefaultRectangular array containing a copy of each of the
      elements contained in this list.
      
      :return: A new DefaultRectangular array.
      

.. function:: proc =(ref lhs: ?tlist?, rhs: tlist?)

   
   Clear the contents of this list, then extend this now empty list with the
   elements contained in another list.
   
   .. warning::
   
     This will invalidate any references to elements previously contained in
     `lhs`.
   
   :arg lhs: The list to assign to.
   :arg rhs: The list to assign from. 
   

.. function:: proc ==(a: ?tlist?, b: tlist?): bool

   
   Returns `true` if the contents of two lists are the same.
   
   :arg a: A list to compare.
   :arg b: A list to compare.
   
   :return: `true` if the contents of two lists are equal.
   :rtype: `bool`
   

.. function:: proc !=(a: ?tlist?, b: tlist?): bool

   
   Return `true` if the contents of two lists are not the same.
   
   :arg a: A list to compare.
   :arg b: A list to compare.
   
   :return: `true` if the contents of two lists are not equal.
   :rtype: `bool`
   

