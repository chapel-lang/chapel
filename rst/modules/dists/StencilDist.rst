.. default-domain:: chpl

.. module:: StencilDist

StencilDist
===========
**Usage**

.. code-block:: chapel

   use StencilDist;

.. class:: Stencil

   
   The Stencil distribution is a variant of the :mod:`Block <BlockDist>`
   distribution that attempts to improve performance for stencil computations by
   reducing the amount of communication necessary during array accesses. From
   the user's perspective, it behaves very similarly to the Block distribution
   where reads, writes, and iteration are concerned.
   
   This distribution reduces communication by creating read-only caches for
   elements adjacent to the block of elements owned by each locale. This
   documentation may refer to these cached regions as 'ghost cells' or 'fluff'.
   This approach can avoid many fine-grained GETs and PUTs when performing a
   stencil computation near the boundary of the current locale's chunk of array
   elements. The user must manually refresh these caches after writes by calling
   the ``updateFluff`` method. Otherwise, reading and writing array elements
   behaves the same as a Block-distributed array.
   
   The indices are partitioned in the same way as the :mod:`Block <BlockDist>`
   distribution.
   
   The ``Stencil`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc Stencil.init(
         boundingBox: domain,
         targetLocales: [] locale  = Locales,
         dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
         dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
         dataParMinGranularity     = // value of  dataParMinGranularity      config const,
         param rank                = boundingBox.rank,
         type  idxType             = boundingBox.idxType,
         fluff: rank*idxType       = makeZero(rank),
         periodic: bool            = false)
   
   The ``fluff`` argument indicates the requested number of cached elements in
   each dimension. If an element of ``fluff`` is greater than zero, the user can
   use indices outside of ``boundingBox`` to index into the array. If the domain
   is not strided, you can consider indices for dimension ``i`` to be:
   
   .. code-block:: chapel
   
      boundingBox.dim(i).expand(fluff(i))
   
   If the domain is strided:
   
   .. code-block:: chapel
   
      const bb = boundingBox.dim(i);
      bb.expand(fluff(i) * abs(bb.stride));
   
   The same logic is used when determining the cached index set on each locale,
   except you can imagine ``boundingBox`` to be replaced with the returned
   value from :proc:`~ChapelArray.localSubdomain`.
   
   The ``periodic`` argument indicates whether or not the stencil distribution
   should treat the array as a discrete chunk in a larger space. When enabled,
   the ghost cells outside of ``boundingBox`` will contain values as if the
   array was replicated on all sides of the space. When disabled, the outermost
   ghost cells will be initialized with the default value of the element's type.
   The ``periodic`` functionality is disabled by default.
   
   .. note::
   
      Note that this domain does not currently handle indices outside of
      the expanded bounding box, so a user must manually wrap periodic indices
      themselves.
   
   Iterating directly over a Stencil-distributed domain or array will only yield
   indices and elements within the ``boundingBox``.
   
   **Updating the Cached Elements**
   
   Once you have completed a series of writes to the array, you will need to
   call the ``updateFluff`` function to update the cached elements for each
   locale. Here is a simple example:
   
   .. code-block:: chapel
   
     use StencilDist;
   
     const Dom = {1..10, 1..10};
     const Space = Dom dmapped Stencil(Dom, fluff=(1,1));
     var A : [Space] int;
   
     [(i,j) in Space] A[i,j] = i*10 + j;
   
     // At this point, the ghost cell caches are out of date
   
     A.updateFluff();
   
     // ghost caches are now up-to-date
   
   After updating, any read from the array should be up-to-date. The
   ``updateFluff`` function does not currently accept any arguments.
   
   **Reading and Writing to Array Elements**
   
   The Stencil distribution uses ghost cells as cached read-only values from
   other locales. When reading from a Stencil-distributed array, the
   distribution will attempt to read from the local ghost cache first. If the
   index is not within the cached index set of the current locale, then we
   default to a remote read from the locale on which the element is located.
   
   Any write to array data will be applied to the actual element, the same as if
   you were using a Block-distributed array.
   
   **Modifying Exterior Ghost Cells**
   
   Updating the outermost ghost cells can be useful when working with a periodic
   stencil-distributed array. If your array contains position information, you may
   want to modify the ghost cells to 'wrap' around the physical space correctly.
   
   You can currently do this with the ``boundaries()`` iterator on a
   stencil-distributed array. This iterator yields a tuple where the first component
   is the ghost cell element to be modified, and the second component is a tuple
   indicating the side on which this ghost cell lives. This direction tuple will
   contain values in the range ``-1..1``.
   
   The release benchmark 'miniMD' contains an example of how one might use this
   iterator.
   
   .. warning::
   
     There is a known issue with this iterator where the program will fail to
     compile if the array element is not an array or a class.
   


