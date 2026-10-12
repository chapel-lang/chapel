==============================
Debugging Multithreaded Issues
==============================

Writing parallel programs can be challenging, especially when it comes to
debugging issues that arise from multithreading (like race conditions). Chapel
provides some built-in features to help with debugging multithreaded code.

Tracking and Reporting on Tasks
-------------------------------

For certain tasking layers, Chapel supports an experimental
capability for tracking the status of tasks, primarily designed for
use in a single-locale execution.  To enable this capability, your
program must be compiled with the ``--task-tracking`` flag.

The feature itself is enabled at execution time by setting the boolean
environment variable ``CHPL_RT_ENABLE_TASK_REPORTING`` to any of the
values "1", "yes", or "true".  If this is done, then when ``<CTRL-C>``
is entered while a program is executing, a list of pending and executing
tasks will be printed to the console, giving an indication of which
tasks are at which source locations.  This is only supported with
``CHPL_TASKS=fifo``.

Note that task tracking adds a fair amount of runtime overhead to
task-parallel programs.

.. TODO tsan fails in the runtime: https://github.com/chapel-lang/chapel/issues/27670
    Using Sanitizers
    ----------------

    ThreadSanitizer (TSan) is a tool that can help detect race conditions and other
    issues in multithreaded programs. To use TSan with Chapel, you will need to
    build Chapel with the appropriate config and then set
    ``CHPL_SANITIZE_EXE=thread``. This will enable TSan for your Chapel programs.
    See :ref:`readme-sanitizers` for more details on how to set up and use
    sanitizers with Chapel.
