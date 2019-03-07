.. _readme-ibm:

===========================
Using Chapel on IBM Systems
===========================

.. warning::
    We have not used Chapel on IBM Systems in several years.  It is
    likely the information presented here is stale or outdated.  If
    you are interested in using Chapel on IBM Systems, please let us
    know.

We only have limited experience using Chapel on IBM systems.  This
file contains notes that reflect our experience, focusing first on
PowerPC-based systems and then BG systems.  If you are not familiar
with Chapel, it is recommended that you first try the
:ref:`chapelhome-quickstart` to get started with the language.


PowerPC SMP clusters
--------------------

We have run Chapel on clusters of Power5 and Power6 SMP nodes using
the following settings:

#. Set ``CHPL_HOME`` and ``MANPATH`` as indicated in :ref:`readme-chplenv`.


#. Set ``CHPL_HOST_PLATFORM`` to ``pwr6`` for a Power6 cluster.  e.g.:

   .. code-block:: sh

     export CHPL_HOST_PLATFORM=pwr6

   This will cause Chapel to use the IBM xlc and xlC compilers by
   default.


#. Set ``CHPL_COMM`` to gasnet.  For example:

   .. code-block:: sh

     export CHPL_COMM=gasnet

   See :ref:`readme-multilocale` for further information about
   running using multiple locales and GASNet.

   Note: if you are using an installation in which your xlc/xlC
   compilers and ``ar`` utility do not use 64-bit object formats by
   default, you will need to set the ``OBJECT_MODE`` variable to 64
   to use GASNet.  For example:

   .. code-block:: sh

     export OBJECT_MODE=64


#. Make sure you're in the top-level chapel/ directory:

   .. code-block:: sh

     cd $CHPL_HOME

   Make/re-make the compiler and runtime:

   .. code-block:: sh

     gmake


#. Set your ``PATH`` to include the directory
   ``$CHPL_HOME/bin/$CHPL_HOST_PLATFORM`` which is created when you
   build the compiler.  For example:

   .. code-block:: sh

     export PATH="$PATH:$CHPL_HOME/bin/$CHPL_HOST_PLATFORM"


#. Compile your Chapel program as usual.  See
   :ref:`readme-compiling` for details.  For example:

   .. code-block:: sh

     chpl -o hello6-taskpar-dist $CHPL_HOME/examples/hello6-taskpar-dist.chpl


#. When you compile a multi-locale program for, you will get a
   single binary by default (e.g., ``hello6-taskpar-dist``).  In
   order to run this program properly, you will typically need to
   write a loadleveler script that requests a number of compute
   nodes equal to the number of locales that you will specify
   through the ``-nl`` option, and launches a single copy of the
   binary per node (either using poe, or on some systems by simply
   invoking the binary directly, at the bottom of the script).  The
   parallelism within the node will be generated within the binary
   using pthreads in order to utilize all of the cores per node.  In
   our experience, the details of required options for loadleveler
   scripts vary greatly from one site to another so check with your
   site's documentation for details.


Additional Notes for Power5 Clusters
------------------------------------

Our current technique for querying the amount of memory per node is
apparently not portable to the Power5 (which is to say, we get an
insanely large value back).  When running the hpcc benchmarks with
the default configuration constants, this will exhibit itself as a
halt indicating that we can't take the `log()` of a non-positive
integer.  Set the problem size explicitly using the ``--m`` or
``--n`` flags.  If anyone has a chance to debug this problem or
suggest a better way to query the amount of memory before we come up
with a solution, please let us know.


Blue Gene/L
-----------

Our current implementation of Chapel relies heavily on POSIX threads
(pthreads) to implement both intra- and inter-locale parallelism.
Since BG/L does not support pthreads, Chapel is not supported on
this platform.  If you are interested in running Chapel on BG/L,
please contact us and let us know.


Blue Gene/P
-----------

We have done some initial experimentation with the GASNet team to
try and run Chapel on BG/P with some limited success, however more
effort is required to make this a stable and supported platform.  If
running Chapel on BG/P would be of interest to you, please contact
us and let us know.
