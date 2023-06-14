.. default-domain:: cpp

.. _Chapter-dyno-chpl-queries-impl:

Depths of the Query System
==========================

Vocabulary
----------

-  **Dependency**: a query executed as part of another query. If query A
   executes query B, then A is said to depend on B.
-  **Preamble**: the code that happens before the main query body is
   run; the contents of ``QUERY_BEGIN``.

Recomputation Model
-------------------

One of the main concerns of the query system when generations and
incremental compilation are involved is avoiding as much work as
possible. To this end, the query system avoids re-running queries if
their results would not change. Since queries are deterministic [1]_, a
query’s output cannot change if all of its dependencies remain the same.
Therefore, the query system’s goal is to check dependencies to see if
they changed, which, in turn, requires checking the dependencies’
dependencies, and so on. As a result, recomputation begins with a
depth-first traversal of the dependency graph.

Conceptually, if none of the input (leaf) queries have changed, then
none of the queries depending on them have changed, and so on – and no
recomputation needs to happen, anywhere. However, there are cases even
when input queries *do* change, but recomputation can be avoided for
most functions. In particular, this can happen if a dependency’s inputs
do change, but its outputs don’t. However, since queries are implemented
using plain and unrestricted C++, there isn't a general way of knowing
how a change of inputs will affect the output without running the query.
Therefore, to detect such cases of unchanged output, queries whose inputs
changed *must* be re-run. See `Example of Avoiding Recomputation` for some code
that demonstrates avoiding recomputation.

The depth-first traversal, combined with the need to recompute queries
to check if their output has changed, means that dependencies are
re-computed before the function that called them is re-computed. In
other words, rather being recomputed *top-down* (from the top-level
query down to its dependencies), dependencies are recomputed
*bottom-up*. This has several implications for the way that context
state should be managed (see `State Implications`_).

Once any of a query’s dependencies’ outputs is determined to have
changed, there’s no use checking its other dependencies – any dependency
changing is sufficient to need to recompute the whole dependent query.
Moreover, whether or not other dependencies ought to be run may depend on the
result of the changed query (see `Changing Dependency Graph Example`_).
Thus, bottom-up recomputation suspends, and the dependent query is
called. At this point, the query’s function runs as normal, its
dependency queries are called as they are encountered in the C++ source
code, which may invoke their own dependencies being computed, and so on.
Thus, starting from the query-to-be-recomputed, we are once again back
to a top-down mode of execution. However, at any point during this
top-down execution, if the query needs to determine if a dependency
needs to be recomputed, another bottom-up traversal will be needed.
Thus, the execution of a query is a layering of top-down and bottom-up
(re)computations.

The recomputation-to-check-output requirement has one more practical
effect. If a dependency is determined to have changed, this has occurred
by re-running it. That means that when the parent query is called to in
turn determine its new value, once it reaches the call to the changed
dependency, the result will already be known, and the saved value will
be used. More concretely, suppose a particular query depends on queries
A, B, and C. Suppose also that all three of these queries would produce a
new output in new revision. In this case, when the parent query is invoked,
the output of `A` would already have been recomputed, whereas `B` and `C`
would still be awaiting recomputation; the use of `A` would result in returning
a cached result, whereas the uses of `B` and `C` would result in invoking
these queries' functions. In short, you cannot expect all query dependencies
to be computed in the same manner (bottom up or top down).

The Preamble is Always Called
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One important thing to note is that, once you’ve decided that
recomputation is necessary, “recomputing” the query is pretty much the
same as computing it in the first place. At the time of writing,
“recompute” is just a shim for calling the query function with the
previously-supplied arguments. However, the query function’s body
includes the preamble, and thus, the preamble is re-run whenever a
query’s result is recomputed.

The preamble does a few things, such as check if a query needs to be
recomputed [2]_, push it onto a query stack (which records
dependencies). If you are implementing logic that is sensitive to the
kind of things that occur in the preamble, keep this in mind.

Example of Avoiding Recomputation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Here’s an example of when an input query changes, but a top-level query
need not change. Suppose you’re reading a string from some input query,
computing its length, and then performing an expensive computation with
that somehow solely depends on the length.

.. code:: c++

   SomeDataStructure& expensiveQuery(Context* context) {
       QUERY_BEGIN(expensiveQuery, context);
       auto strLen = lengthQuery(context);
       auto result = doSomeExpensiveComputation(strLen);
       return QUERY_END(result);
   }

   size_t lengthQuery(Context* context) {
       QUERY_BEGIN(lengthQuery, context);
       auto str = inputQuery(context);
       auto result = str.size();
       return QUERY_END(result);
   }

   std::string inputQuery(Context* context) {
       QUERY_BEGIN_INPUT(inputQuery, context);
       auto result = "";
       return QUERY_END(result);
   }

Now, consider the following sequence of steps.

.. code:: c++

   int main() {
       QUERY_STORE_INPUT_RESULT(inputQuery, context, "hello");
       auto firstResult = expensiveQuery(context);

       context->advanceToNextRevision(false /* prepareToGC */);
       QUERY_STORE_INPUT_RESULT(inputQuery, context, "world");
       auto secondResult = expensiveQuery(context);
   }

Note that ``"hello"`` and ``"world"`` have the same length; thus,
``lengthQuery`` will return the same value, even though the input has
changed. Since queries are deterministic and pure, this means that
``expensiveQuery`` should produce the same result. Thus, even after an
input query change, the expensive computation can be avoided.

**What steps would the query system take?** When ``expensiveQuery`` is
invoked for the second time, the query system will observe that it
hasn’t been computed yet as of the second generation, and would attempt
to determine whether or not recomputation should take place. To do so,
it will perform the depth-first graph traversal with recomputation, as
described above. The only leaf of the dependency graph in this example
is ``inputQuery``; this query has been explicitly set by the user, so it
won’t be re-run. However, it will be marked as “requiring recompute” and
``lengthQuery`` would be re- run because of that. However,
``lengthQuery`` will return the same result as before. Thus, the
recomputation check for ``expensiveQuery`` will find that none of its
dependencies have changed, and skip running it.

A more compilation-oriented example is that of typechecking an expression. A
user might add a space somewhere in the middle of a function call, but
the resulting AST would be the same, and thus, typechecking would not
need to occur.

Changing Dependency Graph Example
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once one of a query's dependencies is known to have changed, we stop
executing the dependencies, and execute the query itself. One of the reasons
for this is that queries that `were` run in the previous execution may not need
to be run at all. Consider the following example.

.. code:: c++

   bool queryWithConditional(Context* context) {
       QUERY_BEGIN(queryWithConditional, context);
       int result = queryA(context);
       if (result == 0) {
           result = queryB(context);
       }
       return QUERY_END(result);
   }

Suppose query A returns ``0`` initially. In that case, both query A
and query B would be dependencies of ``queryWithConditional``. However,
if the result of query A changes to something nonzero, re-running
query B would be unnecessary: it would never be called in the parent
query's body. Not only that, but re-running query B could be incorrect:
if it issued errors, these errors would be shown to the user, even
though there's no reason why they should be.

A slightly more complicated case is as follows.

.. code:: c++

   bool queryWithConditional(Context* context) {
       QUERY_BEGIN(queryWithConditional, context);
       bool queryAResult = queryA(context);
       int result;
       if (queryAResult) {
           result = queryB(context);
       } else {
           result = queryC(context);
       }
       return QUERY_END(result);
   }

Suppose that in the above example, query A returns ``true`` in the first
generation, and ``false`` in the second generation. When it's initially
computed, ``queryWithConditional`` would have queries A and B as its
dependencies. However, once the result of query A changes, it would be
incorrect to re-run query B (for the same reason as before). Additionally,
query C, though not listed as a dependency, will need to be executed,
possibly for the first time. This will happen when ``queryWithConditional``
is itself recomputed. In the end, ``queryWithConditional`` will have queries
A and C as its dependencies.

From a correctness perspective, an important consequence of these examples
is that dependencies should be checked in the order that they were originally
called. Otherwise, we might end up recomputing a query that would no longer
need to be called. Because of this, the query system stores an `ordered` list
of dependencies, and traverses it first-to-last.

State Implications
~~~~~~~~~~~~~~~~~~

The occasional bottom-up nature of recomputing queries means that
``Context`` state changes temporarily made by a parent query may or may
not be visible to a child query. Consider the following example:

.. code:: c++

   bool childQuery(Context* context) {
       QUERY_BEGIN(childQuery, context);
       auto result = context->someStateFlagSet;
       return QUERY_END(result);
   }

   bool parentQuery(Context* context) {
       QUERY_BEGIN(parentQuery, context);
       context->someStateFlagSet = true;
       auto result = childQuery(context);
       context->someStateFlagSet = false;
       return QUERY_END(result);
   }

   int main() {
       Context* context; // Assume initialized with some real value
       auto firstResult = parentQuery(context);
       context->advanceToNextRevision(false /* prepareToGC */);
       auto secondResult = secondResult(context);
   }

The ``firstResult`` variable will be set to ``true``, as one would
expect. The parent query would first be called, set the parent flag, and
then call the child query. The child query would read the flag and
return it.

The ``secondResult`` variable will be set to ``false``. In an effort to
determine whether or not the ``parentQuery`` needs to be recomputed, the
query system will begin a depth-first search. It will therefore first
need to determine if ``childQuery`` has changed; to do so, it must
recompute the result. Thus, it calls ``childQuery``, which reads
``context->someStateFlagSet``, and determines that the flag is *not* set
(after all, it was set to false by ``parentQuery``!). Thus, the query
returns ``false`` – a different value than it returned last time –
prompting ``parentQuery`` to be recomputed. Within the parent query, the
cached value of ``childQuery`` is used, and the ``parentQuery`` returns
``false``, too.

**Does this mean the query system is broken?** No, it does not.
Modifying global state is an effect, and thus ``parentQuery`` is not a
“law-abiding” query.

**So queries cannot rely on any Context state?** Well, not necessary.
Sometimes storing state in the context is useful. For instance, a query
might request that errors from its child queries not be issued to the
user (e.g. if it’s trying to perform a computation that might fail, but
that failure is a potential and expected result). So, queries might want
to rely on (mutable) context state – however, this cannot be done in
query code alone; the context’s implementation must be adjusted to be
aware of the state. My hope is that this document contains enough
information about the query’s system behavior to help support such an
adjustment.

.. [1]
   or at least supposed to be, under the assumptions of the query system

.. [2]
   at the time of writing – and yes, that does mean that if
   ``recompute`` is called from inside a “needs recompute” function,
   another “needs recompute” will be performed.
