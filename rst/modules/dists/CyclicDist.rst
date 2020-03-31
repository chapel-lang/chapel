.. default-domain:: chpl

.. module:: CyclicDist

CyclicDist
==========
**Usage**

.. code-block:: chapel

   use CyclicDist;

.. class:: Cyclic

   
   This Cyclic distribution maps indices to locales in a round-robin pattern
   starting at a given index.
   
   Formally, consider a Cyclic distribution with:
   
     =============  ====================================================
     rank           ``d``
     start index    ``(s_1, ...., s_d)``
     over locales   ``targetLocales: [0..N_1-1, ...., 0..N_d-1] locale``
     =============  ====================================================
   
   It maps an index ``(i_1, ...., i_d)``
   to the locale ``targetLocales[j_1, ...., j_d]``
   where, for each ``k`` in ``1..d``,
   we have:
   
     ``j_k = (i_k - s_k) (mod N_k)``
   
   
   **Example**
   
   The following code declares a domain ``D`` distributed
   using a Cyclic distribution with a start index of ``(1,1)``,
   and declares an array ``A`` over that domain.
   The `forall` loop sets each array element
   to the ID of the locale to which it is mapped.
   
     .. code-block:: chapel
   
       use CyclicDist;
   
       const Space = {1..8, 1..8};
       const D: domain(2) dmapped Cyclic(startIdx=Space.low) = Space;
       var A: [D] int;
   
       forall a in A do
         a = a.locale.id;
   
       writeln(A);
   
   When run on 6 locales, the output is:
   
     ::
   
       0 1 0 1 0 1 0 1
       2 3 2 3 2 3 2 3
       4 5 4 5 4 5 4 5
       0 1 0 1 0 1 0 1
       2 3 2 3 2 3 2 3
       4 5 4 5 4 5 4 5
       0 1 0 1 0 1 0 1
       2 3 2 3 2 3 2 3
   
   
   **Initializer Arguments**
   
   The ``Cyclic`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc Cyclic.init(
         startIdx,
         targetLocales: [] locale = Locales,
         dataParTasksPerLocale     = // value of  dataParTasksPerLocale      config const,
         dataParIgnoreRunningTasks = // value of  dataParIgnoreRunningTasks  config const,
         dataParMinGranularity     = // value of  dataParMinGranularity      config const,
         param rank: int  = // inferred from startIdx argument,
         type idxType     = // inferred from startIdx argument )
   
   The argument ``startIdx`` is a tuple of integers defining an index that
   will be distributed to the first locale in ``targetLocales``.
   In the 1-dimensional case, ``startIdx`` can be an integer
   or a tuple with a single element.
   
   The argument ``targetLocales`` is an array containing the target
   locales to which this distribution maps indices and data.
   The rank of ``targetLocales`` must match the rank of the distribution,
   or be ``1``.  If the rank of ``targetLocales`` is ``1``, a greedy
   heuristic is used to reshape the array of target locales so that it
   matches the rank of the distribution and each dimension contains an
   approximately equal number of indices.
   
   The arguments ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``,
   and ``dataParMinGranularity`` set the knobs that are used to
   control intra-locale data parallelism for Cyclic-distributed domains
   and arrays in the same way that the like-named config constants
   control data parallelism for ranges and default-distributed domains
   and arrays.
   
   The ``rank`` and ``idxType`` arguments are inferred from the
   ``startIdx`` argument unless explicitly set.
   They must match the rank and index type of the domains
   "dmapped" using that Cyclic instance.
   
   
   **Convenience Initializer Functions**
   
   It is common for a ``Cyclic`` distribution to distribute its indices
   across all locales. In this case, a convenience function can be used to
   declare variables of cyclic-distributed domain or array type.  These functions
   take a domain or list of ranges as arguments and return a cyclic-distributed
   domain or array.
   
     .. code-block:: chapel
   
       use CyclicDist;
   
       var CyclicDom1 = newCyclicDom({1..5, 1..5});
       var CyclicArr1 = newCyclicArr({1..5, 1..5}, real);
       var CyclicDom2 = newCyclicDom(1..5, 1..5);
       var CyclicArr2 = newCyclicArr(1..5, 1..5, real);
   
   
   **Data-Parallel Iteration**
   
   A `forall` loop over a Cyclic-distributed domain or array
   executes each iteration on the locale where that iteration's index
   is mapped to.
   
   Parallelism within each locale is guided by the values of
   ``dataParTasksPerLocale``, ``dataParIgnoreRunningTasks``, and
   ``dataParMinGranularity`` of the respective Cyclic instance.
   Updates to these values, if any, take effect only on the locale
   where the updates are made.
   
   
   **Limitations**
   
   This distribution has not been tuned for performance.


