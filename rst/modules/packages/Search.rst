.. default-domain:: chpl

.. module:: Search
   :synopsis: The `Search` module is designed to support standard search routines on 1D

Search
======
**Usage**

.. code-block:: chapel

   use Search;


The `Search` module is designed to support standard search routines on 1D
arrays


.. function:: proc search(Data: [?Dom], val, comparator: ?rec = defaultComparator, lo = Dom.alignedLow, hi = Dom.alignedHigh, sorted = false)

   
   General purpose searching interface for searching through a 1D array.
   For pre-sorted arrays, denoted by passing ``sorted=true`` as an argument,
   this function wraps :proc:`binarySearch`, otherwise it wraps
   :proc:`linearSearch`.
   
   :arg Data: The array to be searched
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted, and the equality operation for the array data.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`
   :arg sorted: Indicate if the array is pre-sorted, determining which search
      algorithm to call
   :type sorted: `bool`
   
   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or the location where the value should have
      been if it was not found.
   :rtype: (`bool`, `Dom.idxType`)
   

.. function:: proc linearSearch(Data: [?Dom], val, comparator: ?rec = defaultComparator, lo = Dom.alignedLow, hi = Dom.alignedHigh)

   
   Searches through the array `Data` looking for the value `val` using
   a sequential linear search.  Returns a tuple indicating (1) whether or not
   the value was found and (2) the location of the first occurrence of the
   value if it was found, or ``hi+abs(Dom.stride)`` if it was not found.
   
   :arg Data: The array to search
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines the
       equality operation for the array data.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`
   
   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or ``hi+abs(Dom.stride)`` if it was not
      found.
   :rtype: (`bool`, `Dom.idxType`)
   
   

.. function:: proc binarySearch(Data: [?Dom], val, comparator: ?rec = defaultComparator, in lo = Dom.alignedLow, in hi = Dom.alignedHigh)

   
   Searches through the pre-sorted array `Data` looking for the value `val`
   using a sequential binary search.  If provided, only the indices `lo`
   through `hi` will be considered, otherwise the whole array will be
   searched. Returns a tuple indicating (1) whether or not the value was
   found and (2) the location of the value if it was found, or the location
   where the value should have been if it was not found.
   
   :arg Data: The sorted array to search
   :type Data: [] `eltType`
   :arg val: The value to find in the array
   :type val: `eltType`
   :arg comparator: :ref:`Comparator <comparators>` record that defines how the
      data is sorted.
   :arg lo: The lowest index to consider while searching
   :type lo: `Dom.idxType`
   :arg hi: The highest index to consider while searching
   :type hi: `Dom.idxType`
   
   :returns: A tuple indicating (1) if the value was found and (2) the location
      of the value if it was found or the location where the value should have
      been if it was not found.
   :rtype: (`bool`, `Dom.idxType`)
   
   

