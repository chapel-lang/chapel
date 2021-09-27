.. default-domain:: chpl

.. _Chapter-User_Defined_Reductions_and_Scans:

=================================
User-Defined Reductions and Scans
=================================

User-defined reductions and scans are supported via class definitions
where the class implements a structural interface. The definition of
this structural interface is forthcoming as it is not yet stable.

.. note::

   While the user-defined reduction class interface is not yet described
   here in the language specification, the currently implemented
   interface is described in the
   :ref:`Reduce Intents Technote <readme-reduceIntents-interface>`.

   Additionally, the following paper describes an early version of the
   interface:

     S. J. Deitz, D. Callahan, B. L. Chamberlain, and L. Snyder.
     **Global-view abstractions for user-defined reductions and scans**.
     In *Proceedings of the Eleventh ACM SIGPLAN Symposium on Principles
     and Practice of Parallel Programming*, 2006.
