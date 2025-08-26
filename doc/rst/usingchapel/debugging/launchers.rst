.. _debugging-with-launchers:

===================================
Running in gdb/lldb with a Launcher
===================================

When debugging multilocale Chapel programs and using some launchers, you can
set an environment variable to launch a separate terminal emulator for each
locale, each running the debugger on that locale's program instance. To use
``gdb``, set the environment variable ``CHPL_COMM_USE_GDB`` when running the
program. For ``lldb``, use ``CHPL_COMM_USE_LLDB``. This is known to work in all
of these launchers:

  * amudprun

  * aprun

  * gasnetrun_ibv

  * gasnetrun_mpi

  * gasnetrun_ofi

  * mpirun4ofi

  * pbs-aprun


The default terminal emulator program is ``xterm``, but by setting the
environment variable ``CHPL_COMM_DBG_TERM=urxvt`` you can force use of
``urxvt`` instead. Whichever terminal emulator is used must be in your ``PATH``
on the compute node or an error will result. Note that it is the user's
responsibility to make sure things are set up so the terminal emulator run in
the target environment can open its display window in the launch environment.
