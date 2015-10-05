.. _readme-marenostrum:

===========================
Using Chapel on MareNostrum
===========================

.. warning::
    We have not used Chapel on MareNostrum in several years.  It is
    likely the information presented here is stale or outdated.  If
    you are interested in using Chapel on MareNostrum, please let us
    know.

The following information describes how to build and use Chapel on
Barcelona Supercomputing Center's MareNostrum machine for users who
have an account there.  If you are not familiar with Chapel, it is
recommended that you first try the :ref:`chapelhome-quickstart`
to get started with the language.

#. Set ``CHPL_HOME`` to point to the directory of your installation as
   usual.  For example:

   .. code-block:: sh

     export CHPL_HOME=~/chapel-x.y.z/chapel

#. Set ``CHPL_HOST_PLATFORM`` to ``marenostrum`` and ``CHPL_COMM`` to
   ``gasnet``.  For example:

   .. code-block:: sh

     export CHPL_HOST_PLATFORM=marenostrum
     export CHPL_COMM=gasnet

   Additional information about running using multiple locales and
   GASNet can be found in :ref:`readme-multilocale`

#. Make sure you're in the top-level chapel/ directory:

   .. code-block:: sh

     cd $CHPL_HOME

   Make/re-make the compiler and runtime:

   .. code-block:: sh

     gmake

#. Set your ``PATH`` to include ``$CHPL_HOME/bin/marenostrum`` which
   is created when you build the compiler.  For example:

   .. code-block:: sh

     export PATH="$PATH:$CHPL_HOME/bin/marenostrum" 


#. Compile your Chapel program as usual.  See
   :ref:`readme-compiling` for details.  For example:

   .. code-block:: sh

     chpl -o hello6-taskpar-dist $CHPL_HOME/examples/hello6-taskpar-dist.chpl

#. The above settings will result in a multi-locale compilation.  When
   you compile a Chapel program, you should see two binaries (e.g.,
   ``hello6-taskpar-dist`` and ``hello6-taskpar-dist_real``).  The
   first binary contains code to launch the Chapel program onto the
   compute nodes using ``mnsubmit``.  The second contains the program
   code itself; it is not intended to be executed directly from the
   shell prompt.

#. On MareNostrum, a marenostrum-specific launcher is used by default
   to wrap the ``mnsubmit`` and ``slurm`` scripts required to launch a
   job on the compute nodes.  This launcher accepts the following
   flags:

   .. code-block:: sh

     --walltime <HH:MM:SS> : (required) set the max wallclock time
     --queue <name>        : (optional) specify the debug or interactive queue

   These options can also be set using the ``CHPL_LAUNCHER_WALLTIME``
   and ``CHPL_LAUNCHER_QUEUE`` environment variables, respectively.

#. Multi-locale binaries require the number of locales to be specified
   on the command line.  Other than this, execute your Chapel program
   as usual.  For example:

   .. code-block:: sh

     ./hello6-taskpar-dist -nl 2 --walltime 00:03:00 --queue debug

   You can use the ``-v`` flag to see the commands used to launch your
   program.  If you set the ``CHPL_LAUNCHER_DEBUG`` environment
   variable, the script used with the ``mnsubmit`` command will be
   preserved after your program executes.

#. On MareNostrum, jobs cannot be run in an interactive mode, so all
   program will end up in a ``.out`` file whose name matches the
   executable.  For example, the program above would leave its output
   in ``hello6-taskpar-dist.out``
