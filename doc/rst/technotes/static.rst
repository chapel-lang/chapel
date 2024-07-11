.. _readme-static:

.. default-domain:: chpl

Function-Static Variables
=========================

.. warning::

  This work is under active development. As such, the interface is unstable and
  expected to change.

  It is likely the final version of this feature will not be implemented
  using an attribute.

.. contents::

Overview
--------
Chapel has limited support for marking variables as 'function static'. Doing
so causes them to have similar semantics to
`C++ block-static variables <https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables>`_.
In other words, a variable declared as 'function static' will be initialized once
when the function is first executed; subsequent calls to the function will
re-use the existing value of the variable. This can be useful for re-using
results of computations that do not change between function invocations. For
instance, one might compute a lookup table on the first invocation of a function,
and simply access that lookup table in subsequent runs. The following program
demonstrates this, pre-computing the Fibonacci numbers:

.. code-block:: chapel

   use CTypes;
   use Time;

   config param tableSize = 10000;

   proc computeExpensiveTable(seed1: real, seed2: real) {
       writeln("Computing an expensive table");
       var toReturn: c_array(real, tableSize);

       toReturn[0] = seed1;
       toReturn[1] = seed2;
       for i in 2..<tableSize {
           toReturn[i] = toReturn[i-1] + toReturn[i-2];
       }

       return toReturn;
   }

   proc getNthElement(x: int): real {
       @functionStatic
       ref table = computeExpensiveTable(1, 1);
       return table[x];
   }

   writeln("Element 0: ", getNthElement(0));
   writeln("Element 1: ", getNthElement(1));
   writeln("Element 100: ", getNthElement(100));

The output of the above program is as follows:

.. code-block:: none

   Computing an expensive table
   Element 0: 1
   Element 1: 1
   Element 100: 5.73148e+20

Importantly, the 'computing' line is only printed once, despite the three
invocation of ``getNthElement``. The static variable in the above snippet is
``table``. It's marked static using the ``@functionStatic`` annotation.
Because of this annotation, the initialization expression of ``table``
is only executed during the first call to ``getNthElement``.

One important aspect of the snippet above is that the variable was declared
using the ``ref`` intent. The reason for this is that ``table`` is
assigned from the (stored) cached result. Thus, if the ``var`` or ``const``
intents were used, each invocation of the function would result in copying
the table from the memoized result to the local variable. If the table
is large enough to warrant memoization, the copy could be expensive.

Generic Functions
-----------------
In generic functions with static variables, one instance of the static
variable is created for each instantiation. Consider the following example:

.. code-block:: chapel

   proc defaultValueForType(type x: numeric): x do return 0;
   proc defaultValueForType(type x: string): x do return "";

   proc genericFunction(arg) {
       @functionStatic
       ref acc = defaultValueForType(arg.type);

       acc += arg;
       return acc;
   }

   writeln(genericFunction(1));
   writeln(genericFunction(2));
   writeln(genericFunction(0.5));
   writeln(genericFunction(0.25));
   writeln(genericFunction("hello"));
   writeln(genericFunction(" world"));

In this example, one copy of the accumulator is created for each of the
functions instantiations (``int``, ``real`` and ``string``). As a result,
the output of the program is as follows:

.. code-block:: none

   1
   3
   0.5
   0.75
   hello
   hello world

Synchronization
---------------

The initialization of function-static variables is implicitly synchronized
using Chapel's ``atomic`` types. As a consequence, it's safe to call a
function with static variables from multiple concurrent threads, as well as
from multiple locales. In the latter case, the variable is stored on the first
locale to initialize it; support for alternative ways of sharing the data
(e.g., replicating the precomputed data to all locales) is considered future
work.

Sharing Kinds
-------------
Currently, Chapel's static variables support two sharing kinds: "compute-or-retrieve"
and "compute-per-locale". The former is the default; under this mode,
the first locale to reach a function-static variable computes the variable's initial
value, and the variable is stored on that locale. Other locales that subsequently
call the function access the variable remotely. For example, the following
program:

.. code-block:: chapel

   proc computeInitialValue() do return 0;

   proc getAndIncrement(): real {
       @functionStatic
       ref myVar = computeInitialValue();
       myVar += 1;
       writeln("I'm on locale ", here, ", the variable is on locale ", myVar.locale);
       return myVar;
   }

   for loc in Locales do on loc {
       writeln(getAndIncrement());
       writeln(getAndIncrement());
   }

Produces the following output when executed using 4 locales:

.. code-block:: none

   I'm on locale LOCALE0, the variable is on locale LOCALE0
   1.0
   I'm on locale LOCALE0, the variable is on locale LOCALE0
   2.0
   I'm on locale LOCALE1, the variable is on locale LOCALE0
   3.0
   I'm on locale LOCALE1, the variable is on locale LOCALE0
   4.0
   I'm on locale LOCALE2, the variable is on locale LOCALE0
   5.0
   I'm on locale LOCALE2, the variable is on locale LOCALE0
   6.0
   I'm on locale LOCALE3, the variable is on locale LOCALE0
   7.0
   I'm on locale LOCALE3, the variable is on locale LOCALE0
   8.0

The variable ``myVar`` is shared across all locales, and each locale sees
the effect of incrementing it. On the other hand, because the first
locale is the one to call ``getAndIncrement``, the variable is stored
there.

The code would behave the same way if the ``computeOrRetrieve`` sharing kind
were explicitly specified.

.. code-block:: chapel

       @functionStatic(sharingKind.computeOrRetrieve)
       ref myVar = computeInitialValue();

The other supported sharing kind is "compute-per-locale". When using this
sharing kind, each locale gets its own copy of the static variable. When a
locale reaches the variable's declaration, it independently computes the
initial value. Subsequent changes to the static variable are only visible to
the locale. The purpose for this approach is to support the pre-computation of
values "near" where the computation takes place. This way, each locale can
reference its own copy of the pre-computed data, without the need for
any communication. By changing the sharing kind to ``comutePerLocale``:

.. code-block:: chapel

       @functionStatic(sharingKind.computePerLocale)
       ref myVar = computeInitialValue();

The output of the whole program changes to the following:

.. code-block:: none

   I'm on locale LOCALE0, the variable is on locale LOCALE0
   1.0
   I'm on locale LOCALE0, the variable is on locale LOCALE0
   2.0
   I'm on locale LOCALE1, the variable is on locale LOCALE1
   1.0
   I'm on locale LOCALE1, the variable is on locale LOCALE1
   2.0
   I'm on locale LOCALE2, the variable is on locale LOCALE2
   1.0
   I'm on locale LOCALE2, the variable is on locale LOCALE2
   2.0
   I'm on locale LOCALE3, the variable is on locale LOCALE3
   1.0
   I'm on locale LOCALE3, the variable is on locale LOCALE3
   2.0

.. warning::

  The ``computePerLocale`` sharing kind currently suffers from a memory leak.
  The copies of static variables that are not on the initial locale
  are not destroyed when the program exists. As a result, the memory associated
  with them is leaked, and their destructors are not invoked. This
  is considered a bug and will be fixed in a future release.

Implementation Details
----------------------

A variable with the ``@functionStatic`` annotation is effectively hoisted
to the module that contains its parent function. During the function's
invocation, the code initializing the variable is replaced with a conditional
that checks if the variable has been initialized. This check includes
synchronization with other threads and/or locales. If the variable has not
been initialized, and the current thread is the first to perform the check,
the initialization expression is executed and the result is stored in
the hoisted variable. As an example, the above ``getNthElement`` function
is transformed into something like the following:


.. code-block:: chapel

   var precomputed: _staticWrapper(c_array(real, tableSize));
   proc getNthElement(x: int): real {
       if precomputed.callerShouldComputeValue() {
         precomputed.setValue(computeExpensiveTable(1, 1));
       }
       ref table = precomputed.getValue();
       return table[x];
   }

The above snippet highlights the importance of the ``ref`` intent:
in the transformed code, the ``table`` variable is assigned to the result
of calling ``precomputed.getValue()``. This result is returned by reference,
but if ``table`` were a value, it would be copied.

Presently, the value is stored in a heap-allocated container class. Thus, by
default, ``_staticWrapper`` stores ``nil``; when the value is set using
``setValue``, a new instance of the container class is allocated and stored
within ``_staticWrapper``.

Limitations
-----------

The current implementation has the following limitations:

* Variables whose types have a runtime component (arrays and domains)
  cannot be stored in function-static variables. This is because the type
  of the initialization expression can only be fully determined at runtime,
  which makes it impossible to hoist the variable to the module level as
  described in `Implementation Details`_.
* There is limited support for alternative ways of sharing the data between
  locales. The current implementation supports storing data on a single
  locale, or re-computing it on all locales. Alternative sharing
  modes (e.g., computing the value once and replicating it to all locales)
  are desirable.
* Split-initialization of static variables is not supported.

Future Work
-----------

As noted at the beginning of this page, the current implementation is
a work in progress. The following items are considered future work:

* Proper language-level support that doesn't simply use an attribute. This
  might include a keyword or some kind of type.
* Better ergonomics for the user. Using ``ref`` does not seem like the ideal
  long-term solution.
* Investigation of how to support types with runtime components.
* Support for alternative ways of sharing static variables between locales.
