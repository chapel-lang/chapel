.. default-domain:: chpl

.. module:: ReplicatedDist

ReplicatedDist
==============
**Usage**

.. code-block:: chapel

   use ReplicatedDist;

.. class:: Replicated

   
   This Replicated distribution causes a domain and its arrays
   to be replicated across the desired locales (all the locales by default).
   An array receives a distinct set of elements - a "replicand" -
   allocated on each locale.
   
   In other words, a Replicated-distributed domain has
   an implicit additional dimension - over the locales,
   making it behave as if there is one copy of its indices per locale.
   
   Consistency among the replicands is not preserved automatically.
   That is, changes to one replicand of an array are never propagated to
   the other replicands by the distribution implementation.
   If desired, consistency needs to be maintained by the user.
   
   When accessing a replicated domain or array from a locale *not* in the
   set of target locales, an error is reported if bounds-checking is on;
   undefined behavior occurs if it is off.
   
   Otherwise, only the replicand on the current locale is accessed when
   referring to the domain or array.
   
   **Example**
   
     .. code-block:: chapel
   
       const Dbase = {1..5};  // A default-distributed domain
       const Drepl = Dbase dmapped Replicated();
       var Abase: [Dbase] int;
       var Arepl: [Drepl] int;
   
       // Only the current locale's replicand is accessed
       Arepl[3] = 4;
   
       // Only the current locale's replicand is accessed
       forall (b,r) in zip(Abase,Arepl) do b = r;
       Abase = Arepl;
   
       // Access other locales' replicand with the replicand(loc) method
       Arepl.replicand(Locales[1]) = Arepl;
   
   See the :ref:`primers-replicated` primer for more examples of the Replicated
   distribution.
   
   **Initializer Arguments**
   
   The ``Replicated`` class initializer is defined as follows:
   
     .. code-block:: chapel
   
       proc Replicated.init(
         targetLocales: [] locale = Locales,
         purposeMessage: string = "used to create a Replicated")
   
   The optional ``purposeMessage`` may be useful for debugging
   when the initializer encounters an error.
   
   
   **Limitations**
   
   * Only rectangular domains are presently supported.
   


