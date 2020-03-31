.. default-domain:: chpl

.. module:: Futures
   :synopsis: .. |---| unicode:: U+2014

Futures
=======
**Usage**

.. code-block:: chapel

   use Futures;



.. |---| unicode:: U+2014

Containers for accessing the results of asynchronous execution.

A :record:`Future` object is a container that can store the result of an
asynchronous operation, which can be retrieved when the result is ready.

Usage
-----

A valid :record:`Future` object is not created directly. Instead, a future may
be created by calling the :proc:`async()` function, which takes as arguments
the function to be executed and all arguments to that function.

The following example demonstrates a trivial use of futures. Three computations
are executed asynchronously.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-simple.chpl
   :language: chapel

.. _valid-futures:

Validity of Futures
-------------------

A future that is initialized by a call to :proc:`async()` or
:proc:`Future.andThen()` is created in a valid state.  Otherwise |---| for
example, when a future is declared but not initialized |---| the future is in
an invalid state and method calls other than :proc:`Future.isValid()` on an
invalid future will :proc:`~ChapelIO.halt()`.  If such a future object is subsequently
assigned to by a call to :proc:`async()` or :proc:`Future.andThen()`, then
the future will become valid.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-validity.chpl
   :language: chapel

Task Arguments
--------------

The task argument in a call to :proc:`async()` or :proc:`Future.andThen()`
may be a :ref:`first-class function <readme-firstClassFns>`, a
:ref:`lambda function <readme-lambdaFns>`, or a specially-constructed
class or record.  Such a record must have both a `proc this()` method for the
desired computation and a `proc retType type` method that returns the return
type of the `this()` method.  (The requirement for the `retType` method is
a currently limitation that is intended to be resolved in the future.)
For example:

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-taskfn.chpl
   :language: chapel

Future Chaining
---------------

A continuation to a future (itself a future) can be created via the
:proc:`Future.andThen()` method, which takes as its single argument a function
to be invoked asynchronously (with respect to other tasks) but strictly ordered
in execution after the result of the parent future is ready. The continuation
function takes a single argument, the result of the parent future.

The following examples demonstrate such chaining of futures.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-chaining1.chpl
   :language: chapel

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-chaining2.chpl
   :language: chapel

Future Bundling
---------------

A set of futures can be bundled via :proc:`waitAll`, which takes a
variable number of futures as arguments and returns a new future whose return
type is a tuple of the return types of the arguments.  The returned future is
ready only when all the future arguments are ready.

The following example demonstrate bundling of futures.

.. literalinclude:: ../../../../test/library/packages/Futures/futures-doc-waitall.chpl
   :language: chapel

 

.. record:: Future

   
   A container that can store the result of an asynchronous operation,
   which can be retrieved when the result is ready.
   
   A future is not created directly. Instead, one is created by calling the
   :proc:`async()` function or the :proc:`Future.andThen()` method on
   an already-existing future.
   


   .. attribute:: type retType

      
      The return type of the future.
      

   .. method:: proc get(): retType

      
      Get the result of a future, blocking until it is available.
      
      If the future is not valid, this call will :proc:`~ChapelIO.halt()`.
      

   .. method:: proc isReady(): bool

      
      Test whether the result of the future is available.
      
      If the future is not valid, this call will :proc:`~ChapelIO.halt()`.
      

   .. method:: proc isValid(): bool

      
      Test whether the future is valid. For more,
      :ref:`see above <valid-futures>`.
      

   .. method:: proc andThen(in taskFn)

      
      Asynchronously execute a function as a continuation of the future.
      
      The function argument `taskFn` must take a single argument of type
      `retType` (i.e., the return type of the parent future) and will be
      executed when the parent future's value is available.
      
      If the parent future is not valid, this call will :proc:`~ChapelIO.halt()`.
      
      :arg taskFn: The function to invoke as a continuation.
      :returns: A future of the return type of `taskFn`
      

.. function:: proc async(in taskFn)

   
   Asynchronously execute a function (taking no arguments) and return a
   :record:`Future` that will eventually hold the result of the function call.
   
   :arg taskFn: A function taking no arguments
   :returns: A future of the return type of `taskFn`
   

.. function:: proc async(in taskFn, args ...)

   
   Asynchronously execute a function (taking arguments) and return a
   :record:`Future` that will eventually hold the result of the function call.
   
   :arg taskFn: A function taking arguments with types matching `args...`
   :arg args...: Arguments to `taskFn`
   :returns: A future of the return type of `taskFn`
   

.. function:: proc waitAll(futures ...?N)

   
   Bundle a set of futures and return a :record:`Future` that will hold a
   tuple of the results of its arguments (themselves futures).
   
   :arg futures...: A variable-length argument list of futures
   :returns: A future with a return type that is a tuple of the return type of
      the arguments
   

