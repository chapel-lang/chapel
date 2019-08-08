/*

To use the :class:`EpochManager`, first create an instance.

.. code-block:: chapel

 var manager = new unmanaged EpochManager();


Registering a Task
------------------
A task must be registered with the manager in order to use the manager.
Registration returns a token.

.. code-block:: chapel

 var tok = manager.register();


Pinning/Unpinning a Task
------------------------
To avoid reclamation while a task is accessing a resource, I.E. to enter
critical section, a task must `pin`. Correspondingly to exit critical section,
the task must `unpin`.

.. code-block:: chapel

 // Enter critical section
 tok.pin();
 // Do something

 // Exit critical section
 tok.unpin();


Deleting an object
------------------
To `delete` an object:

.. code-block:: chapel

 tok.delete_obj(myObj);


.. note::
 A task must be `pinned` to `delete` an object. The manager can only be used to
 delete ``unmanaged`` objects.


Reclaiming deleted objects
--------------------------
To try to reclaim memory:

.. code-block:: chapel

 tok.try_reclaim();


.. note::
 Alternatively, a task may call ``manager.try_reclaim()``.


Unregister a Task
-----------------
In the end, a registered task needs to `unregister` from the manager. The
registration token is a scoped variable, and hence the ending of the scope in
which the task registered would automatically `unregister` the task.
`unregister` can also be performed manually:

.. code-block:: chapel

 tok.unregister();

Destroy
-------
To destroy the manager, and reclaim all the memory managed by the manager:

.. code-block:: chapel

 delete manager;
*/
module EpochManager {

  use LockFreeLinkedList;
  use LockFreeQueue;
  use LimboList;

  /*
    :class:`EpochManager` manages reclamation of objects, ensuring
    thread-safety.
  */
  class EpochManager {

    //  Total number of epochs
    pragma "no doc"
    const EBR_EPOCHS : uint = 3;

    pragma "no doc"
    const INACTIVE : uint = 0;

    //  Global Epoch is used to synchronize registered tasks' local epochs
    pragma "no doc"
    var global_epoch : atomic uint;

    //  flag to indicate a task is trying to advance global epoch
    pragma "no doc"
    var is_setting_epoch : atomic bool;

    //  List of all tokens
    pragma "no doc"
    var allocated_list : unmanaged LockFreeLinkedList(unmanaged _token);

    //  Collection of inactive tokens, which can be recycled
    pragma "no doc"
    var free_list : unmanaged LockFreeQueue(unmanaged _token);

    //  Collection of objects marked deleted
    pragma "no doc"
    var limbo_list : [1..EBR_EPOCHS] unmanaged LimboList();

    pragma "no doc"
    var id_counter : atomic uint;

    /*
      Default initialize the manager.
    */
    proc init() {
      allocated_list = new unmanaged LockFreeLinkedList(unmanaged _token);
      free_list = new unmanaged LockFreeQueue(unmanaged _token, false);
      this.complete();

      // Initialise the free list pool with here.maxTaskPar tokens
      forall i in 0..#here.maxTaskPar {
        var tok = new unmanaged _token(i:uint, this:unmanaged);
        allocated_list.append(tok);
        free_list.enqueue(tok);
      }
      id_counter.write(here.maxTaskPar:uint);
      global_epoch.write(1);
      forall i in 1..EBR_EPOCHS do
        limbo_list[i] = new unmanaged LimboList();
    }

    /*
      Register a task.

      :returns: A handle to the manager
    */
    proc register() : owned TokenWrapper { // Should be called only once
      var tok = free_list.dequeue();
      if (tok == nil) {
        tok = new unmanaged _token(id_counter.fetchAdd(1), this:unmanaged);
        allocated_list.append(tok);
      }
      tok.is_registered.write(true);
      // return tok;
      return new owned TokenWrapper(tok, this:unmanaged);
    }

    pragma "no doc"
    proc unregister(tok: unmanaged _token) {
      if (tok.is_registered.read()) {
        unpin(tok);
        free_list.enqueue(tok);
        tok.is_registered.write(false);
      }
    }

    pragma "no doc"
    proc pin(tok: unmanaged _token) {
      // An inactive task has local_epoch set to 0. A value other than 0
      // implies active task
      if (tok.local_epoch.read() == INACTIVE) then
        tok.local_epoch.write(global_epoch.read());
    }

    pragma "no doc"
    proc unpin(tok: unmanaged _token) {
      tok.local_epoch.write(INACTIVE);
    }

    // Attempt to announce a new epoch
    pragma "no doc"
    proc try_advance() : uint {
      var epoch = global_epoch.read();
      for tok in allocated_list {
        var local_epoch = tok.local_epoch.read();
        if (local_epoch > 0 && local_epoch != epoch) then
          return 0;
      }

      // Advance the global epoch
      epoch = (epoch % EBR_EPOCHS) + 1;
      global_epoch.write(epoch);

      // Return epoch which is safe to be reclaimed. It is safe to
      // reclaim from e-2 epoch
      select epoch {
        when 1 do return EBR_EPOCHS - 1;
        when 2 do return EBR_EPOCHS;
        otherwise do return epoch - 2;
      }
    }

    pragma "no doc"
    proc delete_obj(tok : unmanaged _token, x : unmanaged object) {
      var del_epoch = tok.local_epoch.read();
      if (del_epoch == 0) {
        writeln("Bad local epoch! Please pin! Using global epoch!");
        del_epoch = global_epoch.read();
      }
      limbo_list[del_epoch].push(x);
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc try_reclaim() {
      var count = EBR_EPOCHS;

      // if nothing to reclaim, try the next epoch, but loop only for one
      // full cycle
      while (count) {
        count = count - 1;

        // Set a flag to let other tasks know that a task is already
        // trying to reclaim
        if (is_setting_epoch.testAndSet()) {
          return;
        }
        var reclaim_epoch = try_advance();
        if (reclaim_epoch == 0) {
          // try_advance failed
          is_setting_epoch.clear();
          return;
        }

        var reclaim_limbo_list = limbo_list[reclaim_epoch];
        var head = reclaim_limbo_list.pop();
        is_setting_epoch.clear();

        while (head != nil) {
          var next = head.next;
          delete head.val;
          reclaim_limbo_list.retire_node(head);
          head = next;
        }
      }
    }

    /*
      Reclaim all objects
    */
    proc deinit() {
      delete allocated_list;
      delete free_list;
      delete limbo_list;
    }
  }

  pragma "no doc"
  class _token {
    var local_epoch : atomic uint;
    const id : uint;
    var is_registered : atomic bool;
    var manager : unmanaged EpochManager;

    proc init(x : uint, manager : unmanaged EpochManager) {
      id = x;
      this.manager = manager;
    }

    proc pin() {
      manager.pin(this:unmanaged);
    }

    proc unpin() {
      manager.unpin(this:unmanaged);
    }

    proc delete_obj(x) {
      manager.delete_obj(this:unmanaged, x);
    }

    proc try_reclaim() {
      manager.try_reclaim();
    }

    proc unregister() {
      manager.unregister(this:unmanaged);
    }
  }

  /*
    Handle to :class:`EpochManager`
  */
  class TokenWrapper {

    pragma "no doc"
    var _tok : unmanaged _token;

    pragma "no doc"
    var manager : unmanaged EpochManager;

    pragma "no doc"
    proc init(_tok : unmanaged _token, manager : unmanaged EpochManager) {
      this._tok = _tok;
      this.manager = manager;
    }

    /*
      `Pin` a task
    */
    proc pin() {
      manager.pin(this._tok);
    }

    /*
      `Unpin` a task
    */
    proc unpin() {
      manager.unpin(this._tok);
    }

    /*
      Delete an object.

      :arg x: The class instance to be deleted. Must be of unmanaged class type
    */
    proc delete_obj(x) {
      manager.delete_obj(this._tok, x);
    }

    /*
      Try to announce a new epoch. If successful, reclaim objects which are
      safe to reclaim
    */
    proc try_reclaim() {
      manager.try_reclaim();
    }

    /*
      Unregister the handle from the manager
    */
    proc unregister() {
      manager.unregister(this._tok);
    }

    /*
      Unregister the handle from the manager
    */
    proc deinit() {
      manager.unregister(this._tok);
    }
  }

  pragma "no doc"
  class C {
    var x : int;
    proc deinit() {
      writeln("Deinit: " + this:string);
    }
  }

  pragma "no doc"
  proc main() {
    var a = new unmanaged EpochManager();
    coforall i in 1..10 {
      var tok = a.register();
      var b = new unmanaged C(i);
      tok.pin();
      tok.try_reclaim();
      tok.delete_obj(b);
      // tok.unregister();
    }
    a.try_reclaim();
    delete a;
  }
}
