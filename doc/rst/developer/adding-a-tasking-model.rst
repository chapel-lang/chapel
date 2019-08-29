Adding Tasking Model
====================

This document briefly describes how to add a new tasking model to the
runtime.

Also see ``adding-a-runtime-component.rst`` on how to set up the build for
a new runtime component.

Getting Things Working Quickly
------------------------------

The runtime tasking interface currently consists of 9 functions to
support sync variable operations and an additional 30 functions to
create and manage tasks, along with public types describing task
identifiers and sync variable meta-information.  All of this needs
defining when you add a tasking layer implementation.  The easiest way
to proceed will be to copy an existing tasking model and modify it.  The
``fifo`` tasking implementation is a good candidate, since it is the
simplest one.

To begin, copy the existing tasking model you chose as a starting point.
We will pretend you are starting with the ``fifo`` tasking model, and you
are creating the ``wonderful`` tasking model.  Assuming a Linux working
environment, from the ``$CHPL_HOME`` directory, perform the following git
commands to create the copy:

.. code:: bash

    mkdir runtime/include/tasks/wonderful
    cp runtime/include/tasks/fifo/chpl-tasks-impl.h  \
        runtime/include/tasks/wonderful/
    git add runtime/include/tasks/wonderful/chpl-tasks-impl.h

    mkdir runtime/src/tasks/wonderful
    cp runtime/src/tasks/fifo/tasks-fifo.c  \
        runtime/src/tasks/wonderful/tasks-wonderful.c
    cp runtime/src/tasks/fifo/Makefile  \
        runtime/src/tasks/wonderful/
    cp runtime/src/tasks/fifo/Makefile.include  \
        runtime/src/tasks/wonderful/
    cp runtime/src/tasks/fifo/Makefile.share  \
        runtime/src/tasks/wonderful/
    git add runtime/src/tasks/wonderful

This will leave you with the following new files:
::

    runtime/include/tasks/wonderful/chpl-tasks-impl.h

    runtime/src/tasks/wonderful/tasks-wonderful.c
                            .../Makefile
                            .../Makefile.include
                            .../Makefile.share

Then, you need to modify some existing infrastructure files:

- **util/chplenv/chpl_tasks.py**:

  If your new tasking model will be the default or only model for some Chapel target architecture, you will need to change this file to select your model under the appropriate circumstances.

- **make/tasks/Makefile.<wonderful>**:

  Define ``CHPL_MAKE_THREADS`` to the appropriate threading model for your new tasking model and add any include paths. 

Finally, you have to actually implement your new tasking model.

- **runtime/include/tasks/wonderful/chpl-tasks-impl.h**:

  Here you will need to define several types that are used in the public
  Chapel tasking interface.  Among these are:
 
  - ``chpl_taskID_t``    // the type of a task identifier
     
  - ``chpl_sync_aux_t``  // meta-information associated with sync vars

- **runtime/src/tasks/wonderful/tasks-wonderful.c**:

  This file will be most of the work.  You will need to supply bodies for the 30-odd functions that make up the tasking interface provided by the runtime for use by the compiler and modules.

  While this may look like a lot to do, for some of these functions you can get by with empty versions.  This is especially true for a first implementation, where functionality outweighs performance.


More to Follow
--------------

There is much more needed here, obviously.