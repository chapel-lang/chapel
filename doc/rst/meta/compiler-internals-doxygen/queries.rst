.. default-domain:: cpp

.. _Chapter-next-chpl-queries:

Query Framework
===============

This section describes the query framework that supports
incremental compilation.

This :class:`chpl::Context` class stores the compilation-wide context.
Another name for this compilation-wide context is *program database*. It
handles unique'd strings and also stores the results of queries (so that
they are are memoized). It tracks dependencies of queries in order to
update them appropriately when a dependency changes.

Queries are functions that are written in a stylized manner to interact
with the Context (aka program database). For example, a ``parse`` query
might accept as an argument a ``UniqueString path`` and return a vector
of owned AST nodes.  Another example is a query to determine the location
of an AST node; it would accept as an argument an ``ASTNode*`` and it
would return a Location.

Queries can depend upon other queries by calling them. Queries can be
called as regular C++ functions.

When running a query, the query system will manage:

 * checking to see if the query result is already saved and available for
   reuse
 * recording the queries called by that query as dependencies

Constraints
-----------

Queries should not have side effects. They should not mutate global
variables, for example.

Queries should call dependent queries in a deterministic manner. Queries
should have deterministic results - the same arguments should lead to the
same result.

A query can return a heap-allocated object or include heap allocated
objects in its result. When it does so, other queries can safely use
pointers to the heap-allocated object as arguments or in return values.
See also the `Pointer Memory Management`_ section below.

Writing a Query
---------------

To write a query, create a function that:

 * includes a :class:`chpl::Context` argument followed
   by zero or more query arguments
 * returns a const reference to the desired result type
 * is written in a particular way to interact with the Context through
   the ``QUERY_`` macros defined in query-impl.h.

Queries cannot be regular methods but they can be class methods (static
methods).

For example, here is a query that computes MyResultType from myArg1 and
myArg2:

.. code-block:: c++

    #include "chpl/queries/query-impl.h"

    const MyResultType& myQueryFunction(Context* context,
                                        MyArgType myArg1,
                                        MyOtherArgType myArg2) {
      QUERY_BEGIN(myQueryFunction, context, myArg1, myArg2)

      // do steps to compute the result
      MyResultType result = ...;
      // if an error is encountered, it can be saved with QUERY_ERROR(error)

      return QUERY_END(result);
    }

To call the query, just write e.g. ``myQueryFunction(context, arg1, arg2)``.

The macros are where the query framework takes important steps to make
this function in to a query.

In ``QUERY_BEGIN``, the query framework will check to see if there is
already a result in the Context for these arguments that can be reused.
If a result can be reused, ``QUERY_BEGIN`` will run the ``mark`` function
on the result and return it (see `Mark Functions`_ below). Otherwise, the
query proceeds to execute the rest of the function body in order to
compute the result.

The framework also supports input queries which use ``QUERY_BEGIN_INPUT``
instead of ``QUERY_BEGIN``. These queries pull in state from outside the
framework - for example, they might read a file. They will be re-run
every revision even if their dependencies are unchanged.

While running the rest of the function body, any queries called will be
automatically recorded as dependencies.

The query concludes with ``return QUERY_END(result)`` which indicates to
the query framework that ``result`` is a local variable containing the
result of the query. This result, or one equivalent to it, will be
returned and stored in the Context for future reuse. This process is
managed by the ``update`` function (see `Update Functions`_ below).

Types used as argument or return types in queries need to have certain
functionality as described in the Requirements on Types Used in Queries
section below.

Checking and Recomputing Queries
--------------------------------

The first step in a query occurs within ``QUERY_BEGIN``. In this step,
the query framework checks to see if the body of the query needs to be
run.  This uses ``lastChecked`` and ``lastChanged`` values which are
revision numbers stored for the query in the Context.

The framework will check if dependencies have changed and this process
will include recursively re-running them if necessary.

The framework will skip running the body and return an existing
result if:

 * lastChecked matches the current revision number
 * the query is not an input query and no dependencies changed in this
   revision (i.e., for all dependencies, lastChanged < currentRevision)

Otherwise, the body of the query will be run. While running the body, the
query framework tracks the queries called as dependencies. When
``QUERY_END`` is reached, the query framework decides if it is possible
to reuse the previous result. In particular, if there were no changes in
the result, the old result can be reused and then it may be possible to
skip running queries dependent on this one. This is sometimes called the
*early cutoff optimization* in the context of build systems.

Requirements on Types Used in Queries
-------------------------------------

There are some requirements on query argument/key types and on result types:

 * argument/key types must have ``std::hash<KeyType>``
   (typically by providing a hash method and calling it from
   a std::hash template specialization)
 * argument/key types must have ``std::equal_to<KeyType>``
   (typically by providing an ``==`` operator overload)
 * result types must have ``chpl::update<MyResultType>``
   (typically by providing an ``update`` method)
 * result types must have ``chpl::mark<MyResultType>``
   (typically by providing a ``mark`` method)
 * result types must be default constructable

Update Functions
----------------

As described above, after running a query, the query framework needs to
decide if some or all of the old result can be reused. Reuse is important
because it can allow the system to avoid re-running queries that depend on
this one.  Result types can inform this process by implementing an
``update`` method on the result type.

For example, an ``update`` method might look like this:

.. code-block:: c++

    class MyResultType {
      static bool update(MyResultType& keep, MyResultType& addin);
    }

Or, for a query returning an ``owned`` result, it might look like this:

.. code-block:: c++

   class OwnResult {
      static bool update(owned<OwnResult>& keep, owned<OwnResult>& addin);
    }

In either case, the meaning of the arguments and return type is the same.

On entry to the ``update`` function:

 * ``keep`` is the old result from a previous revision
 * ``addin`` is the new result just computed

After running this function:

 * ``keep`` stores the result we need to keep
 * any values not kept need to be stored in ``addin``

The function returns ``true`` if anything changed in ``keep`` and
``false`` if ``keep`` was reused without changes.

Put another way, the ``update`` function needs to:

  * store the current, updated result in ``keep``
  * store the unused result in ``addin``
  * return ``false`` if ``keep`` matched ``addin`` -- that is, ``keep`` did not
    need to be updated; and ``true`` otherwise.

For most result types, ``return defaultUpdate(keep, addin);`` should be
sufficient. In the event that a result is actually a collection of
results that owns the elements (for example, when parsing, the result is
conceptually a vector of top-level symbol), the ``update`` function
should try to update only those elements of ``keep`` that changed by
swapping in the appropriate elements from ``addin``. This strategy allows
later queries that depend on such a result to use pointers to the owned
elements and to avoid updating everything if just one element changed.

The query framework actually uses template specialization of a
``chpl::update`` function object in ``update-functions.h`` to implement
``update`` for a variety of standard C++ types. The default
implementation of this ``chpl::update`` function object just calls the
update method on the type. ``update-functions.h`` also provides
``defaultUpdateOwned`` which is usually all that is needed for ``owned``
results.

Mark Functions
--------------

In order to garbage collect ``UniqueStrings`` appropriately, and to
enable consistency checking within the query framework, types returned by
a query need to support a ``mark`` function.

In some situations, the query framework can reuse a result without
running the ``update`` function for it. That can happen when all
dependencies have been checked in this revision and the dependencies are
all reused. In that event, the ``UniqueStrings`` that are contained in or
referred to by the result need to be marked so that any ``UniqueStrings``
not used can be garbage collected. This is accomplished through the
``mark`` function.

For example, an ``mark`` method might look like this:

.. code-block:: c++

    class MyResultType {
      void mark(Context* context) const;
    }

The ``mark`` function needs to traverse the returned value:

 * marking owned contained pointers with context->markOwnedPointer
   or context->markPointer(owned value)
 * marking unowned/borrowed contained pointers with
   context->markUnownedPointer or context->markPointer(pointer value)
 * marking contained UniqueStrings with ``UniqueString::mark(context)``
 * generally, marking any fields storing a custom value by calling
   ``field.mark(context)``.

The implementation actually uses a template specialization of a
``chpl::mark`` function object in ``mark-functions.h`` to implement
``mark`` for a variety of standard C++ types. The default version runs
the ``mark`` method described above.


Pointer Memory Management
-------------------------

Queries can use pointers as argument or return types. However, in order
for this to work, the pointer must point to something returned by a query
in a heap allocation. Typically, queries that want to support having the
result handled as a pointer return an ``owned`` (aka ``std::unique_ptr``)
value for the result.

It is not sufficient to simply use the address of the `const &` result of
the query - that is a location in the map that will change as the map is
updated. Instead, such patterns should use `owned` to make sure a new
heap-allocated value is created.

When working with results containing pointers, the ``update`` function
should not rely on the contents of these pointers from the ``keep``
value. The system will make sure that they refer to valid memory but they
might be a combination of old results.  Additionally, the system will
ensure that any old results being replaced will remain allocated until
the garbage collection runs outside of any query.

For example, a ``parse`` query might result in a list of ``owned`` AST
element pointers. A follow-on query, ``listSymbols``, can result in
something containing these AST element pointers, but not owning them. In
that event, the ``listSymbols`` query needs to use an ``update`` function
that does not look in to the AST element pointers. However it can compare
the pointers themselves because the ``parse`` query will update the
pointer if the contents change.

When using this pattern:

 * a query argument that is an unowned pointer always refer to something
   from the current revision
 * an unowned pointer in a query result always refer to something from
   the current revision that had ``mark`` run on it when the query owning
   it returned it
