.. _readme-sgi:

=========================
Using Chapel on SGI Altix
=========================

.. warning::
    We have not used Chapel on SGI Altix in several years.  It is
    likely the information presented here is stale or outdated.  If
    you are interested in using Chapel on SGI Altix, please let us
    know.

The Chapel team does not have access to an SGI Altix for the purposes
of testing, so this file describes the experiences that other users
have had in hopes that it will be useful to future Altix users.  This
file assumes a certain amount of familiarity building and using Chapel
in single- and multi-locale settings such as desktops or commodity
clusters.

As an opening note, performance on the Altix was not quite as good as
we'd expected based on what we see on other platforms.  We conjecture
that the cause is that our runtime pthreads are not bound to specific
cores/processors and expect that some overhead creeps in as the OS
tries to schedule them -- that perhaps it ends up moving them around
more than is ideal.  The Altix represents a larger core count per node
(not to mention a different OS) than the platforms that we've focused
on to date, so it may be more sensitive to this.  As a result, a
possible area of future work would be to look into how our runtime
might be modified to do this binding and/or to utilize a user-level
tasking library that multiplexes logical tasks onto a fixed set of
pthreads equal to (and bound to) the number of cores.

    .. note::
        This performance characterization was written when ``fifo``
        was the default tasking layer. It is likely that ``qthreads``
        performs substantially better on Altix as it provides all of
        the desired runtime capabilities listed above.  Additionally,
        previous studies done by the qthreads team have shown strong
        performance on Altix. See :ref:`readme-tasks` for more
        information on Chapel's tasking layers.


Due to its support for shared memory, an Altix machine can either be
viewed as a large single locale, or as a multi-locale machine.  Our
experience is that better performance is obtained when treating it as
a single-locale machine.  However, since most Altix installations are
organized as multi-partition machines, you will need to treat it as a
multi-locale machine if you want to span multiple partitions.

For the single-locale version, you should be able to build Chapel in
the standard way as described in the :ref:`chapelhome-quickstart`,
making sure that the ``CHPL_COMM`` environment variable is unset or set to
``none``.

For multi-locale executions, Chapel uses GASNet as our means of
specifying inter-locale communication.  Left to its own devices,
GASNet prefers to use its SHMEM conduit on the SGI Altix because it's
considered the lowest overhead communication layer with which to
implement GASNet.  Unfortunately, GASNet's SHMEM conduit is not
thread-safe (because SHMEM itself is apparently not thread-safe); or,
to use GASNet terminology, it does not support PAR mode.  Chapel's
execution model requires PAR mode, and therefore the suboptimal MPI
conduit needs to be used for GASNet executions.  

This can be done using the following settings:

.. code-block:: sh

        # cd to chapel source directory
        export CHPL_HOME=$(pwd)
        export CHPL_COMM=gasnet
        export CHPL_COMM_SUBSTRATE=mpi
        # edit make/Makefile.base to include
        CHPL_GASNET_CFG_OPTIONS += --disable-aligned-segments

In our experience, different Altix installations support MPI in
different ways.  If your Altix machine supports MPI via an ``mpicc``
compiler command and launches using an ``mpiexec`` command, then make the
following settings:

.. code-block:: sh

        export MPI_CC=mpicc
        export MPI_LDFLAGS=-mt
        export MPI_RUNCMD="mpiexec -n %N %C"
        export MPI_LIBS=

If on the other hand, your Altix machine supports MPI via a normal
compiler invocation and by linking in the mpi libraries, make the
following settings:

.. code-block:: sh

        export MPI_CC=cc   # or whatever compiler you wish to use
        export MPI_LIBS=-lmpi

Then build the Chapel compiler/runtime again using ``make`` or ``gmake``.
