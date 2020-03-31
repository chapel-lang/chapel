.. default-domain:: chpl

.. module:: Collection
   :synopsis: Summary

Collection
==========
**Usage**

.. code-block:: chapel

   use Collection;



Summary
_______

A 'Collection' is an interface for a data structure that is a 'contract' between
the user and implementer that it fulfills the following:

1. Is safe parallel-safe, hence is safe to use across multiple tasks across multiple locales.
2. Supports the basic operations that any data structure needs to be truly useful, that is:

  a. Insertion of an arbitrary element. From this, we can insert bulk arbitrary elements.
  b. Removal of an arbitrary element. From this, we can remove bulk arbitrary elements.
  c. Iteration over all elements. From this, we can perform lookups over all elements.

From the standpoint of the user, who directly benefits, they obtain a very nice but minimal
guarantee on the object they are using, and from the implementor's view, they get to the
benefit from both implementing a well-design interface, and from getting some utility methods
'for-free'.

.. note::

  This module is a work in progress and may change in future releases.

Bugs and Known Issues
_____________________

1.  Parallel iteration is not apart of the core Collections module as currently the Chapel compiler
    will produce an internal error. As such, the utility methods provided 'for-free' use serial iteration.
    This issue has been documented under issue `#6998 <https://github.com/chapel-lang/chapel/issues/6998>`_ . Couple this
    fact with the fact that ``break`` ing out of a serial iterator will result in resource leakage, where destructors
    are not called, hence Collections using some RAII-based resource cleanup will end up leaking and potentially leaving
    the Collection in an undefined state. This has been documented under issue `#6912 <https://github.com/chapel-lang/chapel/issues/6912>`_ .

Methods
_______

.. class:: CollectionImpl

   .. attribute:: type eltType

      
      The type of element that this Collection holds.
      

   .. method:: proc init(type eltType)

   .. method:: proc add(elt: eltType): bool

      
      Adds an element to this data structure.
      

   .. method:: proc addBulk(elts): int

      
      Add all elements in bulk to this data structure. If the data structure
      rejects an element, we cease to offer more. We return the number of elements
      successfully added to this data structure.
      

   .. method:: proc remove(): (bool, eltType)

      
      Removes an arbitrary element from this data structure.
      

   .. method:: proc removeBulk(nElts: int)

      
      Removes `nElts` elements from this data structure, returning them as an array.
      If the data structure fails to produce a new element, we cease and shrink the array
      to its appropriate size and return that.
      

   .. method:: proc contains(elt: eltType): bool

      
      Determine whether an element exists in this collection.
      

   .. method:: proc clear()

      
      Clears all elements in this collection.
      

   .. method:: proc isEmpty(): bool

      
      Check if this data structure is empty.
      

   .. method:: proc length: int

      Deprecated - please use :proc:`CollectionImpl.size`. 

   .. method:: proc size: int

      
      Syntactic sugar for `getSize`.
      

   .. method:: proc getSize(): int

      
      Obtain the number of elements contained in this collection.
      

   .. itermethod:: iter these(): eltType

      
      Iterate over all elements in the data structure.
      

.. function:: proc +=(ref c: CollectionImpl(?eltType), elt: eltType)

   
   Syntactic sugar for :proc:`CollectionImpl.add`.
   

