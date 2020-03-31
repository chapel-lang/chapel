.. default-domain:: chpl

.. module:: BlockCycDist

BlockCycDist
============
**Usage**

.. code-block:: chapel

   use BlockCycDist;

.. class:: BlockCyclic

   
   
   This Block-Cyclic distribution maps maps blocks of indices to locales in a
   round-robin pattern according to a given block size and start index.
   
   Formally, consider a Block-Cyclic distribution with:
   
     =============  ====================================================
     rank           ``d``
     start index    ``(s_1, ...., s_d)``
     block size     ``(b_1, ...., b_d)``
     over locales   ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
     =============  ====================================================
   
   It maps an index ``(i_1, ...., i_d)``
   to the locale ``targetLocales[j_1, ...., j_d]``
   where, for each ``k`` in ``1..d``,
   we have:
   
     ``j_k = (  (i_k - s_k) / b_k  ) (mod N_k)``
   
   
   **Limitations**
   
   This distribution is work in progress and so has significant limitations.
   
   It has not been tuned for performance.
   
   The only ``idxType`` currently supported is `int` or `int(64)`.
   
   
   **Example**
   
   The following code declares a domain ``D`` distributed over
   a Block-Cyclic distribution with a start index of ``(1,1)``
   and a block size of ``(2,3)``,
   and declares an array ``A`` over that domain.
   The `forall` loop sets each array element
   to the ID of the locale to which it is mapped.
   
     .. code-block:: chapel
   
       use BlockCycDist;
   
       const Space = {1..8, 1..8};
       const D: domain(2)
         dmapped BlockCyclic(startIdx=Space.low,blocksize=(2,3)) 
         = Space;
       var A: [D] int;
   
       forall a in A do
         a = a.locale.id;
   
       writeln(A);
   
   When run on 6 locales, the output is:
   
     ::
   
       0 0 0 1 1 1 0 0
       0 0 0 1 1 1 0 0
       2 2 2 3 3 3 2 2
       2 2 2 3 3 3 2 2
       4 4 4 5 5 5 4 4
       4 4 4 5 5 5 4 4
       0 0 0 1 1 1 0 0
       0 0 0 1 1 1 0 0
   
   
   **Initializer Arguments**
   
   The ``BlockCyclic`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc BlockCyclic.init(
         startIdx,
         blocksize,
         targetLocales: [] locale = Locales, 
         dataParTasksPerLocale    = // value of dataParTasksPerLocale config const
         param rank: int          = // inferred from startIdx argument,
         type idxType             = // inferred from startIdx argument )
   
   The argument ``startIdx`` is a tuple of integers defining an index
   that will be distributed to the first locale in ``targetLocales``.
   For a single dimensional distribution ``startIdx`` can be an integer
   or a tuple with a single element.
   
   The argument ``blocksize`` is a tuple of integers defining the block
   size of indices that will be used in dealing out indices to the
   locales. For a single dimensional distribution ``blocksize`` can be an
   integer or a tuple with a single element.
   
   The argument ``targetLocales`` is an array containing the target
   locales to which this distribution maps indices and data.  The rank of
   ``targetLocales`` must match the rank of the distribution, or be one.
   If the rank of ``targetLocales`` is one, a greedy heuristic is used to
   reshape the array of target locales so that it matches the rank of the
   distribution and each dimension contains an approximately equal number
   of indices.
   
   The argument ``dataParTasksPerLocale`` determines the maximum number
   of tasks on each Locale for data parallelism.
   
   The ``rank`` and ``idxType`` arguments are inferred from the
   ``startIdx`` argument unless explicitly set.
   They must match the rank and index type of the domains
   "dmapped" using that BlockCyclic instance.
   
   
   **Data-Parallel Iteration**
   
   A `forall` loop over a Cyclic-distributed domain or array
   executes each iteration on the locale where that iteration's index
   is mapped to.
   


