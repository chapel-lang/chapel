:title: Users Guide: On-Clauses

.. default-domain:: chpl

On-Clauses
==========

As mentioned previously, all Chapel programs begin execution as a
single task running on locale #0.  Tasks can be moved to other locales
using *on-clauses*.  An on-clause prefixes another statement,
specifying where it should be executed.  Once the task leaves the scope
of the on-clause, it returns to its original locale.

As an example, the following program iterates through all of the
Locales on which the program is running, moving the task to that
locale and printing out a message indicating its ID:

.. literalinclude:: ../../../../../test/release/examples/users-guide/locality/onClause.chpl
  :language: chapel

Running this program on four locales generates:

.. literalinclude:: ../../../../../test/release/examples/users-guide/locality/onClause.good
  :language: text


.. TODO:
   - exhibit with a second loop nest that the task starts and returns to locale 0?
   - orthogonality with parallelism
   - data-driven on-clauses
