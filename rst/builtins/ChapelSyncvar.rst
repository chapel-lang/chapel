.. default-domain:: chpl

.. module:: ChapelSyncvar
   :synopsis: Synchronization variables have a logical state associated with the value. The

Synchronization Variables
=========================
Synchronization variables have a logical state associated with the value. The
state of the variable is either full or empty. Normal reads of a
synchronization variable cannot proceed until the variable's state is full.
Normal writes of a synchronization variable cannot proceed until the variable's
state is empty.

Chapel supports two types of synchronization variables: sync and single. Both
types behave similarly, except that a single variable may only be written once.
Consequently, when a sync variable is read, its state transitions to empty,
whereas when a single variable is read, its state does not change. When either
type of synchronization variable is written, its state transitions to full.

If a task attempts to read or write a synchronization variable that is not in
the correct state, the task is suspended. When the variable transitions to the
correct state, the task is resumed. If there are multiple tasks blocked waiting
for the state transition, one is non-deterministically selected to proceed and
the others continue to wait if it is a sync variable; all tasks are selected to
proceed if it is a single variable.

.. function:: proc isSyncType(type t) param

   Returns true if `t` is a sync type, false otherwise. 

.. method:: proc sync.readFE()

   
   1) Block until the sync variable is full.
   2) Read the value of the sync variable and set the variable to empty.
   
   :returns: The value of the sync variable.
   

.. method:: proc sync.readFF()

   
   1) Block until the sync variable is full.
   2) Read the value of the sync variable and leave the variable full
   
   :returns: The value of the sync variable.
   

.. method:: proc sync.readXX()

   
   1) Read the value of the sync variable
   2) Do not inspect or change the full/empty state
   
   :returns: The value of the sync variable.
   

.. method:: proc sync.writeEF(x: valType)

   
   1) Block until the sync variable is empty.
   2) Write the value of the sync variable and leave the variable full
   
   :arg val: New value of the sync variable.
   

.. method:: proc sync.writeFF(x: valType)

   
   1) Block until the sync variable is full.
   2) Write the value of the sync variable and leave the variable full
   
   :arg val: New value of the sync variable.
   

.. method:: proc sync.writeXF(x: valType)

   
   1) Write the value of the sync variable and leave the variable full
   
   :arg val: New value of the sync variable.
   

.. method:: proc sync.reset()

   
   Resets the value of this sync variable to the default value of
   its type. This method is non-blocking and the state of the sync
   variable is set to empty when this method completes.
   

.. method:: proc sync.isFull

   
   Determine if the sync variable is full without blocking.
   Does not alter the state of the sync variable
   
   :returns: true if the state of the sync variable is full.
   

.. function:: proc isSingleType(type t) param

   Returns true if `t` is a single type, false otherwise. 

.. method:: proc single.readFF()

   
   1) Block until the single variable is full.
   2) Read the value of the single variable and leave the variable full
   
   :returns: The value of the single variable.
   

.. method:: proc single.readXX()

   
   1) Read the value of the single variable
   2) Do not inspect or change the full/empty state
   
   :returns: The value of the single variable.
   

.. method:: proc single.writeEF(x: valType)

   
   1) Block until the single variable is empty.
   2) Write the value of the single variable and leave the variable full
   
   :arg val: New value of the single variable.
   

.. method:: proc single.isFull

   
   Determine if the single variable is full without blocking.
   Does not alter the state of the single variable
   
   :returns: true if the state of the single variable is full.
   

