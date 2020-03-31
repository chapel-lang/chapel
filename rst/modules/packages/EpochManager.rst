.. default-domain:: chpl

.. module:: EpochManager
   :synopsis: Epoch-Based Memory Reclamation

EpochManager
============
**Usage**

.. code-block:: chapel

   use EpochManager;



Epoch-Based Memory Reclamation
------------------------------

In Epoch-Based Memory Reclamation, the lifetime of an arbitrary number
of objects are represented in terms of an *epoch*. While inside of an epoch,
there is a guarantee that no object that is reachable will be reclaimed. To
ensure this guarantee, objects are marked for deletion first prior to actually
deleting them by inserting them into a data structure specific to the current epoch.
Objects, prior to deletion, must be *logically removed* such that no future operations
are able to access them. One example of logical removal is removing a node from a linked
list by a task; the node is no longer accessible by other tasks after its removal, but
any other task could have access to said node *before* it was removed. To further concretize
this example by applying epochs, imagine that all tasks are in epoch *e*, a node *n* is logically
removed and therefore pushed on *e*'s data structure, then the epoch is advanced to *e'*,
then *n* can be safely deleted once no task is in *e*. 

.. note::

  The correctness of Epoch-Based Memory Reclamation is contingent on the fact that
  ``delete`` is never explicitly called on an object accessible during an epoch,
  and that all deletion is handled through the ``EpochManager`` API.

There are two variants of the EpochManager, the ``LocalEpochManager``, which is optimized
for shared-memory, and the ``EpochManager``, which is optimized for distributed memory.
The ``LocalEpochManager`` is a shared-memory variant that is implemented entirely as
a class that enables the user to specify their own lifetime management as you would with
any other class. The ``EpochManager`` on the other hand, is a ``record`` that is 
``forwarding`` to a privatized class instance, which ensures that all field accesses
and method invocations are forwarded to the an instance that is local to the current locale.
The ``EpochManager`` and ``LocalEpochManager`` share the same API and semantics in all but
one case: deallocation. Since ``EpochManager`` is a ``record``, you must explicitly invoke
``destroy`` on it, as it will not automatically clean itself up when it goes out of scope.

.. code-block:: chpl

  var localManager = new owned LocalEpochManager();
  var distManager = new EpochManager();
  // Necessary
  distManager.destroy();  

For a task to enter an epoch, they must first ``register`` their task with the manager.
Once registered, a *token* specific to that task is returned, which will automatically unregister
the task after it goes out of scope via ``owned`` lifetime semantics.

.. code-block:: chpl

  var manager = new owned EpochManager();
  var token = manager.register();
  // Optional
  token.unregister();
  forall i in 1..N with (var token = manager.register()) {}
  manager.destroy();

After registering, the task must then ``pin`` to enter the current epoch, and ``unpin``
once they are finished. This token can also be used to mark objects for reclamation
via ``deferDelete``. The ``EpochManager`` takes any type of ``unmanaged`` class and treats
them as ``object``, so no generics are required.

.. code-block:: chpl

  forall i in 1..N with (var token = manager.register()) {
    token.pin();          
    token.deferDelete(new unmanaged object());
    token.unpin();  
  }  


To advance the epoch and ensure that objects can be reclaimed, ``tryReclaim`` must
be invoked, and should be done so periodically. ``tryReclaim`` is concurrent-safe to
call, and so can be called from the context of multiple tasks.

.. code-block:: chpl

  var dom = {1..N} dmapped Cyclic(startIdx=1);
  var manager = new EpochManager();
  forall i in dom with (var token = manager.register(), var numOps : int) {
    token.pin();
    // ...
    token.unpin();
    numOps += 1;
    if numOps % 1024 == 0 then {
      manager.tryReclaim();
    }
  }
  manager.tryReclaim();
  manager.destroy();


.. class:: LocalEpochManager

   
   :class:`LocalEpochManager` manages reclamation of objects, ensuring
   thread-safety.
   


   .. method:: proc init()

      
      Default initialize the manager.
      

   .. method:: proc register(): owned TokenWrapper

      
      Register a task.
      
      :returns: A handle to the manager
      

   .. method:: proc tryReclaim()

      
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim. It is legal to call it in or outside of a read-side
      critical section (while the task is pinned).
      

   .. method:: proc deinit()

      
      Reclaim all objects
      

.. class:: TokenWrapper

   
   Handle to :class:`LocalEpochManager`
   


   .. method:: proc pin()

      
      `Pin` a task
      

   .. method:: proc unpin()

      
      `Unpin` a task
      

   .. method:: proc deferDelete(x: unmanaged nilable object)

      
      Delete an object.
      
      :arg x: The class instance to be deleted. Must be of unmanaged class type
      

   .. method:: proc tryReclaim()

      
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
      

   .. method:: proc unregister()

      
      Unregister the handle from the manager
      

   .. method:: proc deinit()

      
      Unregister the handle from the manager
      

.. record:: EpochManager

   
   :record:`EpochManager` manages reclamation of objects, ensuring
   thread-safety. It employs privatization.
   


   .. method:: proc init()

      
      Default initialize with instance of privatized class.
      

   .. method:: proc destroy()

      
      Reclaim all allocated memory; destroy all privatized objects.
      

.. class:: EpochManagerImpl

   
   The class which is privatized on each locale for
   :record:`EpochManager`.
   


   .. method:: proc register(): owned DistTokenWrapper

      
      Register a task.
      
      :returns: A handle to the manager
      

   .. method:: proc tryReclaim()

      
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
      

   .. method:: proc clear()

      
      Destroy all objects. Not thread-safe
      

.. class:: DistTokenWrapper

   
   Handle to :record:`EpochManager`
   


   .. method:: proc pin()

      
      `Pin` a task
      

   .. method:: proc unpin()

      
      `Unpin` a task
      

   .. method:: proc deferDelete(x: unmanaged nilable object)

      
      Delete an object.
      
      :arg x: The class instance to be deleted. Must be of unmanaged class type
      

   .. method:: proc tryReclaim()

      
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
      

   .. method:: proc unregister()

      
      Unregister the handle from the manager
      

   .. method:: proc deinit()

      
      Unregister the handle from the manager
      

