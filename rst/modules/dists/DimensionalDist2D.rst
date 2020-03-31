.. default-domain:: chpl

.. module:: DimensionalDist2D

DimensionalDist2D
=================
**Usage**

.. code-block:: chapel

   use DimensionalDist2D;

.. class:: DimensionalDist2D

   
   This Dimensional distribution allows the mapping from indices
   to locales to be specified as a composition of independent mappings
   along each dimension. For example, when implementing the HPL
   benchmark, the user may wish to use 2D arrays that are Block-distributed
   along one dimension and replicated along the other dimension.
   Currently only 2D domains and arrays are supported.
   
   Formally, consider a Dimensional distribution with:
   
     ====================  ====================================================
     rank                  ``d``
     dimension specifiers  ``dim_1``, ...., ``dim_d``
     over locales          ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
     ====================  ====================================================
   
   It maps an index ``(i_1, ...., i_d)``
   to the locale ``targetLocales[j_1, ...., j_d]``
   where, for each ``k`` in ``1..d``,
   the mapping is obtained using the dimension specifier:
   
     ``j_k = dim_k(i_k, N_k)``
   
   **Example**
   
   The following code declares a domain ``D`` distributed
   using a 2D Dimensional distribution that
   replicates over 2 locales (when available) in the first dimension
   and distributes using block-cyclic distribution in the second dimension.
   
     .. code-block:: chapel
   
       use DimensionalDist2D, ReplicatedDim, BlockCycDim;
   
       const Space = {1..3, 1..8};
   
       // Compute N_1 and N_2 and reshapes Locales correspondingly.
   
       var (N_1, N_2) =
         if numLocales == 1
           then (1, 1)
           else (2, numLocales/2);
   
       var MyLocaleView = {0..N_1-1, 0..N_2-1};
       var MyLocales = reshape(Locales[0..N_1*N_2-1], MyLocaleView);
   
       const D = Space
         dmapped DimensionalDist2D(MyLocales,
                                   new ReplicatedDim(numLocales = N_1),
                                   new BlockCyclicDim(numLocales = N_2,
                                                      lowIdx     = 1,
                                                      blockSize  = 2));
       var A: [D] int;
   
       for loc in MyLocales do on loc {
   
         // The ReplicatedDim specifier always accesses the local replicand.
         //
         // Therefore, 'forall a in A' when executed on MyLocales[loc1,loc2]
         // visits only the replicands on MyLocales[loc1,0..N_2-1].
   
         forall a in A do
           a = here.id;
   
         // Technicality: 'writeln(A)' would read A always on Locale 0.
         // Since we want to see what A contains on the current locale,
         // we use default-distributed 'Helper'.
         // 'Helper = A' captures the view of A on the current locale,
         // which we then print out.
   
         writeln("On ", here, ":");
         const Helper: [Space] int = A;
         writeln(Helper);
         writeln();
       }
   
   When run on 6 locales, the output is:
   
     ::
   
       On LOCALE0:
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
   
       On LOCALE1:
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
   
       On LOCALE2:
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
       0 0 1 1 2 2 0 0
   
       On LOCALE3:
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
   
       On LOCALE4:
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
   
       On LOCALE5:
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
       3 3 4 4 5 5 3 3
   
   
   **Initializer Arguments**
   
   The ``DimensionalDist2D`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc DimensionalDist2D.init(
         targetLocales: [] locale,
         di1,
         di2,
         name: string = "dimensional distribution",
         type idxType = int,
         dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
         dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
         dataParMinGranularity     = // value of  dataParMinGranularity      config const )
   
   The argument ``targetLocales`` must be a 2D array indicating
   the locales to distribute over.
   
   The arguments ``di1`` and ``di2`` are the desired dimension specifiers
   for the first and second dimension, respectively.
   
   The ``name`` argument may be useful for debugging.
   It is stored and otherwise ignored by the implementation.
   
   The ``idxType`` argument must match the index type of the domains
   "dmapped" using that DimensionalDist2D instance.
   
   The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
   and ``dataParMinGranularity`` set the knobs that are used to
   control intra-locale data parallelism for Block-distributed domains
   and arrays in the same way that the like-named config constants
   control data parallelism for ranges and default-distributed domains
   and arrays.
   
   
   **Dimension Specifiers**
   
   Presently, the following dimension specifiers are available
   (shown here with their initializer arguments):
   
   * :class:`ReplicatedDim(numLocales) <ReplicatedDim>`
   * :class:`BlockDim(numLocales, boundingBox, idxType=boundingBox.idxType) <BlockDim>`
   * :class:`BlockCyclicDim(numLocales, lowIdx, blockSize) <BlockCycDim>`
   
   
   **Limitations**
   
   Only 2D domains and arrays are supported.
   
   There may be performance issues when scaling to a large number of locales.


