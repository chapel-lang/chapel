.. _Chapter-Locales_Chapter:

Locales
=======

Chapel provides high-level abstractions that allow programmers to
exploit locality by controlling the affinity of both data and tasks to
abstract units of processing and storage capabilities called *locales*.
The *on-statement* allows for the migration of tasks to *remote*
locales.

Throughout this section, the term *local* will be used to describe the
locale on which a task is running, the data located on this locale, and
any tasks running on this locale. The term *remote* will be used to
describe another locale, the data on another locale, and the tasks
running on another locale.

.. _Locales:

Locales
-------

A *locale* is a portion of the target parallel architecture that has
processing and storage capabilities. Chapel implementations should
typically define locales for a target architecture such that tasks
running within a locale have roughly uniform access to values stored in
the locale’s local memory and longer latencies for accessing the
memories of other locales. As an example, a cluster of multicore nodes
or SMPs would typically define each node to be a locale. In contrast a
pure shared memory machine would be defined as a single locale.

.. _The_Locale_Type:

Locale Types
~~~~~~~~~~~~

The identifier ``locale`` is a class type that abstracts a locale as
described above. Both data and tasks can be associated with a value of
locale type. A Chapel implementation may define subclass(es) of
``locale`` for a richer description of the target architecture.

.. _Locale_Methods:

Locale Methods
~~~~~~~~~~~~~~

The locale type supports the following methods:



.. code-block:: chapel

   proc locale.callStackSize: uint(64);

Returns the per-task call stack size used when creating tasks on the
locale in question. A value of 0 indicates that the call stack size is
determined by the system.



.. code-block:: chapel

   proc locale.id: int;

Returns a unique integer for each locale, from 0 to the number of
locales less one.



.. code-block:: chapel

   proc locale.maxTaskPar: int(32);

Returns an estimate of the maximum parallelism available for tasks on a
given locale.



.. code-block:: chapel

   proc locale.name: string;

Returns the name of the locale.



.. code-block:: chapel

   proc numPUs(logical: bool = false, accessible: bool = true);

Returns the number of processing unit instances available on a given
locale. Basically these are the things that execute instructions. If
``logical`` is ``false`` then the count reflects physical instances,
often referred to as *cores*. Otherwise it reflects logical instances,
such as hardware threads on multithreaded CPU architectures. If
``accessible`` is ``true`` then the count includes only those processors
the OS has made available to the program. Otherwise it includes all
processors that seem to be present.



.. code-block:: chapel

   use Memory;
   proc locale.physicalMemory(unit: MemUnits=MemUnits.Bytes, type retType=int(64)): retType;

Returns the amount of physical memory available on a given locale in
terms of the specified memory units (Bytes, KB, MB, or GB) using a value
of the specified return type.

.. _Predefined_Locales_Array:

The Predefined Locales Array
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chapel provides a predefined environment that stores information about
the locales used during program execution. This *execution environment*
contains definitions for the array of locales on which the program is
executing (``Locales``), a domain for that array (``LocaleSpace``), and
the number of locales (``numLocales``). 

.. code-block:: chapel

   config const numLocales: int;
   const LocaleSpace: domain(1) = [0..numLocales-1];
   const Locales: [LocaleSpace] locale;

When a Chapel program starts, a single task executes ``main`` on
``Locales(0)``.

Note that the Locales array is typically defined such that distinct
elements refer to distinct resources on the target parallel
architecture. In particular, the Locales array itself should not be used
in an oversubscribed manner in which a single processor resource is
represented by multiple locale values (except during development).
Oversubscription should instead be handled by creating an aggregate of
locale values and referring to it in place of the Locales array.

   *Rationale*.

   This design choice encourages clarity in the program’s source text
   and enables more opportunities for optimization.

   For development purposes, oversubscription is still very useful and
   this should be supported by Chapel implementations to allow
   development on smaller machines.

..

   *Example*.

   The code 

   .. code-block:: chapel

      const MyLocales: [0..numLocales*4] locale 
                     = [loc in 0..numLocales*4] Locales(loc%numLocales);
      on MyLocales[i] ...

   defines a new array ``MyLocales`` that is four times the size of the
   ``Locales`` array. Each locale is added to the ``MyLocales`` array
   four times in a round-robin fashion.

.. _here:

The *here* Locale
~~~~~~~~~~~~~~~~~

A predefined constant locale ``here`` can be used anywhere in a Chapel
program. It refers to the locale that the current task is running on.

   *Example*.

   The code 

   .. code-block:: chapel

      on Locales(1) {
        writeln(here.id);
      }

   results in the output ``1`` because the ``writeln`` statement is
   executed on locale 1.

The identifier ``here`` is not a keyword and can be overridden.

.. _Querying_the_Locale_of_a_Variable:

Querying the Locale of an Expression
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The locale associated with an expression (where the expression is
stored) is queried using the following syntax: 

.. code-block:: syntax

   locale-access-expression:
     expression . `locale'

When the expression is a class, the access returns the locale on which
the class object exists rather than the reference to the class. If the
expression is a value, it is considered local. The implementation may
warn about this behavior. If the expression is a locale, it is returned
directly.

   *Example*.

   Given a class C and a record R, the code 

   .. code-block:: chapel

      on Locales(1) {
        var x: int;
        var c: C;
        var r: R;
        on Locales(2) {
          on Locales(3) {
            c = new C();
            r = new R();
          }
          writeln(x.locale.id);
          writeln(c.locale.id);
          writeln(r.locale.id);
        }
      }

   results in the output 

   .. code-block:: printoutput

      1
      3
      1

   The variable ``x`` is declared and exists on ``Locales(1)``. The
   variable ``c`` is a class reference. The reference exists on
   ``Locales(1)`` but the object itself exists on ``Locales(3)``. The
   locale access returns the locale where the object exists. Lastly, the
   variable ``r`` is a record and has value semantics. It exists on
   ``Locales(1)`` even though it is assigned a value on a remote locale.

Module-scope constants that are not distributed in nature are
replicated across all locales.

   *Example*.

   For example, the following code: 

   .. code-block:: chapel

      const c = 10;
      for loc in Locales do on loc do
          writeln(c.locale.id);

   outputs 

   .. code-block:: printoutput

      0
      1
      2
      3
      4

   when running on 5 locales.

.. _On:

The On Statement
----------------

The on statement controls on which locale a block of code should be
executed or data should be placed. The syntax of the on statement is
given by 

.. code-block:: syntax

   on-statement:
     `on' expression `do' statement
     `on' expression block-statement

The locale of the expression is automatically queried as described
in :ref:`Querying_the_Locale_of_a_Variable`. Execution of the
statement occurs on this specified locale and then continues after the
``on-statement``.

Return statements may not be lexically enclosed in on statements. Yield
statements may only be lexically enclosed in on statements in parallel
iterators :ref:`Parallel_Iterators`.

.. _remote_variable_declarations:

Remote Variable Declarations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By default, when new variables and data objects are created, they are
created in the locale where the task is running. Variables can be
defined within an ``on-statement`` to define them on a particular locale
such that the scope of the variables is outside the ``on-statement``.
This is accomplished using a similar syntax but omitting the ``do``
keyword and braces. The syntax is given by: 

.. code-block:: syntax

   remote-variable-declaration-statement:
     `on' expression variable-declaration-statement
