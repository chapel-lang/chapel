.. default-domain:: chpl

.. module:: PrivateDist

PrivateDist
===========
**Usage**

.. code-block:: chapel

   use PrivateDist;

.. class:: Private

   
   This Private distribution maps each index ``i``
   between ``0`` and ``numLocales-1`` to ``Locales[i]``.
   
   The index set of a domain distributed over a Private distribution
   is always ``0..numLocales-1``, regardless of the domain's rank,
   and cannot be changed.
   
   The following domain is available as a convenience,
   so user programs do not need to declare their own:
   
     .. code-block:: chapel
   
       const PrivateSpace: domain(1) dmapped Private();
   
   
   **Example**
   
   The following code declares a Private-distributed array ``A``.
   The `forall` loop visits each locale and sets the array element
   corresponding to that locale to that locale's number of cores.
   
     .. code-block:: chapel
   
       var A: [PrivateSpace] int;
       forall a in A do
         a = here.numPUs();
   
   
   **Data-Parallel Iteration**
   
   A `forall` loop over a Private-distributed domain or array
   runs a single task on each locale.
   That task executes the loop's iteration corresponding to
   that locale's index in the ``Locales`` array.
   
   
   **Limitations**
   
   Domains and arrays distributed over this distribution
   do not provide some standard domain/array functionality.
   
   This distribution may perform unnecessary communication
   between locales.


