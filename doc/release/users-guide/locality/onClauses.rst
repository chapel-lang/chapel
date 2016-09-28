:title: Users Guide: **on**-clauses

.. default-domain:: chpl

**on**-clauses: controlling locality/affinity
=============================================

As mentioned previously, all Chapel programs begin execution as a
single task running on locale #0.  Tasks can execute on remote locales
using *on-clauses*.  An on-clause prefixes another statement,
specifying where it should be executed.  This can be thought of as
migrating the task that encountered the on-clause to a (potentially)
remote locale.  Once the on-clause's statement completes, the original
task continues executing on the original locale.


Simple On-Clause Examples
-------------------------

As an example, the following program iterates sequentially through all
of the Locales on which the program is running, conceptually migrating
the main task to that locale, where it writes out a message indicating
the locale's ID:

.. literalinclude:: examples/users-guide/locality/onClause.chpl
  :caption:
  :language: chapel

Running this program on four locales generates:

.. literalinclude:: examples/users-guide/locality/onClause.good
  :language: text

Here's a minor change to this example which demonstrates that the
original task starts and ends on locale #0:

.. literalinclude:: examples/users-guide/locality/onClausePlusOrig.chpl
  :caption:
  :language: chapel

When run on three locales, this program generates:

.. literalinclude:: examples/users-guide/locality/onClausePlusOrig.good
  :language: text
  :lines: 1-12

Note that the statement following an on-clause may include compound
statements or function calls, so can result in an arbitrary amount of
code being executed remotely.


Locality is Orthogonal to Parallelism
-------------------------------------

It's important to note that Chapel's features for controlling locality
are orthogonal to those used to create parallelism.  Specifically,
while both of the above programs run using multiple locales, neither
is a parallel program.  Instead, a single conceptual task is moving
between locales as it executes.  Similarly, all parallel programs from
previous sections will only execute on a single locale---that is, they
only result in shared-memory parallelism.

In order to get distributed memory parallelism, all that's required is
to compose Chapel's features for parallelism and locality.  As a
simple example, the following program uses a coforall loop to create a
task per locale in combination with an on-clause to execute each task
on its corresponding locale:

.. literalinclude:: examples/users-guide/locality/coforallPlusOn.chpl
  :caption:
  :language: chapel

In effect, this creates an SPMD (Single Program, Multiple Data) style
of parallelism.  When running on four locales, the output will be a
nondeterministic permutation of the following four lines:

.. literalinclude:: examples/users-guide/locality/coforallPlusOn.good
  :language: text


Data-driven On-clauses
----------------------

While the on-clauses in the examples above use a locale value to
specify where execution should take place, more generally, on-clauses
can be specified using arbitrary expressions (typically variable
references).  In such cases, execution moves to the locale on which
the variable or expression is stored.  These are referred to as
*data-driven on-clauses*.

As a simple example of a data-driven on-clause, the following program
moves its task to the locale on which *x* is stored:

.. literalinclude:: examples/users-guide/locality/dataDrivenOnClause-trivial.chpl
  :caption:
  :language: chapel

However, this program is trivial in that *x* is stored on locale 0,
so the on-clause is essentially degenerate and doesn't move execution
anywhere:

.. literalinclude:: examples/users-guide/locality/dataDrivenOnClause-trivial.good
  :language: text

The next section will explain how variables can be declared on other
locales to make data-driven on-clauses more interesting.

(It's worth noting that even cases which use locale values to specify
their on-clauses are technically data-driven since each unique locale
value is stored on its corresponding locale.  However, in practice we
typically use the term to refer to cases when an on-clause's
expression is of a type other than *locale*).

.. UP NEXT:
   - data allocation and on-clauses
