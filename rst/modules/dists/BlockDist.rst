.. default-domain:: chpl

.. module:: BlockDist

BlockDist
=========
**Usage**

.. code-block:: chapel

   use BlockDist;

.. class:: Block

   
   This Block distribution partitions indices into blocks
   according to a ``boundingBox`` domain
   and maps each entire block onto a locale from a ``targetLocales`` array.
   
   The indices inside the bounding box are partitioned "evenly" across
   the target locales. An index outside the bounding box is mapped to the
   same locale as the nearest index inside the bounding box.
   
   Formally, an index ``idx`` is mapped to ``targetLocales[locIdx]``,
   where ``locIdx`` is computed as follows.
   
   In the 1-dimensional case, for a Block distribution with:
   
   
     =================   ====================
     ``boundingBox``     ``{low..high}``
     ``targetLocales``   ``[0..N-1] locale``
     =================   ====================
   
   we have:
   
     ===================    ==========================================
     if ``idx`` is ...      ``locIdx`` is ...
     ===================    ==========================================
     ``low<=idx<=high``     ``floor(  (idx-low)*N / (high-low+1)  )``
     ``idx < low``          ``0``
     ``idx > high``         ``N-1``
     ===================    ==========================================
   
   In the multidimensional case, ``idx`` and ``locIdx`` are tuples
   of indices; ``boundingBox`` and ``targetLocales`` are multi-dimensional;
   the above computation is applied in each dimension.
   
   
   **Example**
   
   The following code declares a domain ``D`` distributed over
   a Block distribution with a bounding box equal to the domain ``Space``,
   and declares an array ``A`` over that domain.
   The `forall` loop sets each array element
   to the ID of the locale to which it is mapped.
   
     .. code-block:: chapel
   
       use BlockDist;
   
       const Space = {1..8, 1..8};
       const D: domain(2) dmapped Block(boundingBox=Space) = Space;
       var A: [D] int;
   
       forall a in A do
         a = a.locale.id;
   
       writeln(A);
   
   When run on 6 locales, the output is:
   
     ::
   
       0 0 0 0 1 1 1 1
       0 0 0 0 1 1 1 1
       0 0 0 0 1 1 1 1
       2 2 2 2 3 3 3 3
       2 2 2 2 3 3 3 3
       2 2 2 2 3 3 3 3
       4 4 4 4 5 5 5 5
       4 4 4 4 5 5 5 5
   
   
   **Initializer Arguments**
   
   The ``Block`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc Block.init(
         boundingBox: domain,
         targetLocales: [] locale  = Locales, 
         dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
         dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
         dataParMinGranularity     = // value of  dataParMinGranularity      config const,
         param rank                = boundingBox.rank,
         type  idxType             = boundingBox.idxType,
         type  sparseLayoutType    = DefaultDist)
   
   The arguments ``boundingBox`` (a domain) and ``targetLocales`` (an array)
   define the mapping of any index of ``idxType`` type to a locale
   as described above.
   
   The rank of ``targetLocales`` must match the rank of the distribution,
   or be ``1``.  If the rank of ``targetLocales`` is ``1``, a greedy
   heuristic is used to reshape the array of target locales so that it
   matches the rank of the distribution and each dimension contains an
   approximately equal number of indices.
   
   The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
   and ``dataParMinGranularity`` set the knobs that are used to
   control intra-locale data parallelism for Block-distributed domains
   and arrays in the same way that the like-named config constants
   control data parallelism for ranges and default-distributed domains
   and arrays.
   
   The ``rank`` and ``idxType`` arguments are inferred from the ``boundingBox``
   argument unless explicitly set.  They must match the rank and index type of the
   domains "dmapped" using that Block instance. If the ``boundingBox`` argument is
   a stridable domain, the stride information will be ignored and the
   ``boundingBox`` will only use the lo..hi bounds.
   
   When a ``sparse subdomain`` is created for a ``Block`` distributed domain, the
   ``sparseLayoutType`` will be the layout of these sparse domains. The default is
   currently coordinate, but :class:`LayoutCS.CS` is an interesting alternative.
   
   **Convenience Initializer Functions**
   
   It is common for a ``Block`` distribution to distribute its ``boundingBox``
   across all locales. In this case, a convenience function can be used to
   declare variables of block-distributed domain or array type.  These functions
   take a domain or list of ranges as arguments and return a block-distributed
   domain or array.
   
     .. code-block:: chapel
   
       use BlockDist;
   
       var BlockDom1 = newBlockDom({1..5, 1..5});
       var BlockArr1 = newBlockArr({1..5, 1..5}, real);
       var BlockDom2 = newBlockDom(1..5, 1..5);
       var BlockArr2 = newBlockArr(1..5, 1..5, real);
   
   **Data-Parallel Iteration**
   
   A `forall` loop over a Block-distributed domain or array
   executes each iteration on the locale where that iteration's index
   is mapped to.
   
   Parallelism within each locale is guided by the values of
   ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``, and
   ``dataParMinGranularity`` of the respective Block instance.
   Updates to these values, if any, take effect only on the locale
   where the updates are made.
   
   **Sparse Subdomains**
   
   When a ``sparse subdomain`` is declared as a subdomain to a Block-distributed
   domain, the resulting sparse domain will also be Block-distributed. The
   sparse layout used in this sparse subdomain can be controlled with the
   ``sparseLayoutType`` initializer argument to Block.
   
   This example demonstrates a Block-distributed sparse domain and array:
   
     .. code-block:: chapel
   
       use BlockDist;
   
       const Space = {1..8, 1..8};
   
       // Declare a dense, Block-distributed domain.
       const DenseDom: domain(2) dmapped Block(boundingBox=Space) = Space;
   
       // Declare a sparse subdomain.
       // Since DenseDom is Block-distributed, SparseDom will be as well.
       var SparseDom: sparse subdomain(DenseDom);
   
       // Add some elements to the sparse subdomain.
       // SparseDom.bulkAdd is another way to do this that allows more control.
       SparseDom += [ (1,2), (3,6), (5,4), (7,8) ];
   
       // Declare a sparse array.
       // This array is also Block-distributed.
       var A: [SparseDom] int;
   
       A = 1;
   
       writeln( "A[(1, 1)] = ", A[1,1]);
       for (ij,x) in zip(SparseDom, A) {
         writeln( "A[", ij, "] = ", x, " on locale ", x.locale);
       }
   
      // Results in this output when run on 4 locales:
      // A[(1, 1)] = 0
      // A[(1, 2)] = 1 on locale LOCALE0
      // A[(3, 6)] = 1 on locale LOCALE1
      // A[(5, 4)] = 1 on locale LOCALE2
      // A[(7, 8)] = 1 on locale LOCALE3
   
   


