.. default-domain:: chpl

.. module:: MPI
   :synopsis: MPI Bindings for Chapel.

MPI
===
**Usage**

.. code-block:: chapel

   use MPI;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   MPI/*

MPI Bindings for Chapel.


.. note::
  This is a prototype of the MPI interface for Chapel. The feature set,
  configuration support, and documentation are expected to improve over time.

MPI Version
-----------

This module implements the C-API for the MPI 1.1 standard. It currently does
not support the following routines ::

      MPI_Op_create
      MPI_Op_free
      MPI_Errhandler_create
      MPI_Errhandler_free
      MPI_Keyval_create
      MPI_Keyval_free

since all of these are built around user-defined handlers, that is not supported.

This module also includes a few functions that are in later MPI standards.
These are ::

      MPI_Init_thread (and related constants)
      MPI_Ibarrier

Compiler wrappers
-----------------

Using MPI requires pointing the compiler to the location of the
MPI headers, and including the appropriate MPI libraries. This is
often done by using compiler wrappers like "mpicc". Setting
``CHPL_TARGET_COMPILER=mpi-gnu`` and ``CHPL_TARGET_CPU=none`` will
get Chapel to use mpicc and assume that it works like gcc does.

Note that on Cray systems, this is not necessary, since the default
compilers include the necessary MPI information.

.. note::
  Currently, only the gnu compilers wrapped by MPI are supported. This class
  is expected to broaden in the future.

Interoperability Modes
----------------------

There are two modes in which Chapel and MPI can interoperate with one another:

SPMD Mode
~~~~~~~~~

SPMD mode (MPI + single locale Chapel) has no constraints on the setup of
Chapel and MPI, and is conceptually the same
as OpenMP+MPI programming. All communication here is handled by
MPI; Chapel is just used to provide local parallelism. Furthermore,
in this mode, Chapel locales do not map to MPI ranks at all; the program
just sees a single locale.
Note that the same caveats that apply to OpenMP+MPI
programming apply here. For instance, MPI must be initialized with the
appropriate thread support (see below).

**Setting up SPMD mode on a Cray:**

The recommended configuration for running multilocale MPI jobs on a Cray is as
follows:

.. code-block:: sh

 CHPL_TARGET_COMPILER=cray-prgenv-gnu
 CHPL_TASKS=fifo
 CHPL_COMM=none
 MPICH_MAX_THREAD_SAFETY=multiple

These are the configurations in which this module is currently tested.

.. note::
  Currently, running SPMD jobs on a Cray requires manually launching the jobs,
  as opposed to utilizing a built in Chapel launcher. Support for launching SPMD jobs with the built-in Chapel launchers will be
  added in the future.

Multilocale Mode
~~~~~~~~~~~~~~~~

Multilocale mode is where both Chapel and MPI communications
may be mixed. This allows the user to use optimized/convenient MPI routines
when desired, while having access to the Chapel PGAS model. Using this
model requires

1. The MPI library should (and in many cases must) support the
   :const:`MPI_THREAD_MULTIPLE` support level for threads. This is needed if
   the user plans on making MPI calls from multiple threads. It is necessary
   when using the mpi-conduit for GASNet, since the GASNet library may make
   simultaneous MPI calls.  We've had good experience with MPICH and
   :const:`MPI_THREAD_MULTIPLE`.

**Setting up multilocale mode on a Cray:**

The recommended configuration for running multilocale MPI jobs on a Cray is as
follows:

.. code-block:: sh

  CHPL_TARGET_COMPILER=cray-prgenv-{gnu, intel}
  CHPL_TASKS={qthreads, fifo}   # see discussion below
  CHPL_COMM={ugni, gasnet}
  CHPL_COMM_SUBSTRATE={aries, mpi}   # if CHPL_COMM=gasnet 
  MPICH_MAX_THREAD_SAFETY=multiple
  AMMPI_MPI_THREAD=multiple         # if CHPL_COMM_SUBSTRATE=mpi

Running under ``gasnet+aries`` might require setting ``MPICH_GNI_DYNAMIC_CONN=disabled``.
This is discussed :ref:`here <readme-cray-constraints>`.

These are the configurations in which this module is currently tested. Any
launcher should work fine for this mode. Support is expected to expand in
future versions.

Qthreads and MPI
----------------

Since MPI is not natively Qthread-aware, some care is required to avoid deadlocks. This
section describes current recommendations on using ``CHPL_TASKS=qthreads`` and the MPI
module.

We assume that ``CHPL_COMM`` is either ``ugni`` or ``gasnet+aries``.
We do not recommend using the MPI module with the ``gasnet+mpi`` communication backend
and ``qthreads``. 

1. Use non-blocking calls whenever possible. Note that this also requires using ``MPI_Test``
   instead of ``MPI_Wait``. For convenience, we provide wrappers for a subset of
   the MPI blocking calls that are implemented with non-blocking calls and correctly yield
   tasks while waiting.

2. Any blocking calls (including third-party libraries) must be preceded with a call to
   ``Barrier``.

3. Blocking calls must be serialized; concurrent blocking calls can result in deadlocks.

Configurations Constants
------------------------

This module uses two boolean config constants:
:const:`autoInitMPI` and :const:`requireThreadedMPI`.
Refer to their documentation for more information on when to use them.

Communicators
-------------

The GASNet runtime, and therefore Chapel, makes no guarantees that the MPI
ranks will match the GASNet locales. This module creates a new MPI communicator
:proc:`CHPL_COMM_WORLD` that ensures that this mapping is true.
Note that if numLocales is 1, :proc:`CHPL_COMM_WORLD` is identical to :const:`MPI_COMM_WORLD`,
which is the desired behaviour for SPMD mode programs.

.. note::
  #. Pointer arguments are written as ``ref`` arguments, so no casting to a ``c_ptr``
     is necessary.
  #. An exception to the above is if the C prototype names the argument ``array_of_*``,
     in which case it is written using an array form.
  #. Some MPI-1.1 functions were deprecated in MPI-2. These should be updated in the future, but
     are still present in this version.
  #. :const:`MPI_Aint` is represented by ptrdiff. If this is not the correct
     size, there will be an assertion failure in the code.

MPI Module Documentation
------------------------

.. note::
  For items without documentation, please refer to the MPI documentation.


.. data:: config const autoInitMPI = true

   
   Automatically initializes MPI (if not already initialized by the runtime), and
   shuts it down as well.
   

.. data:: config const requireThreadedMPI = true

   
   Ensures that MPI is running in :const:`MPI_THREAD_MULTIPLE` mode, and will
   abort if not. This is not necessary if you are running SPMD mode, but is
   likely to be necessary if you run
   in multilocale mode.
   

.. function:: proc CHPL_COMM_WORLD

   
   Define a new communicator that directly maps to Chapel locales.
   This is just a reordering of ``MPI_COMM_WORLD``, which you are, of course,
   free to continue to use. This just guarantees that locale ids and MPI
   ranks agree.
   

.. function:: proc initialize()

   
   Helper routine that also sets process IDs (world rank) and number of
   processes (world size)
   

.. function:: proc commRank(comm: MPI_Comm = MPI_COMM_WORLD): c_int

   
   Wrapper to get the rank of the communicator comm.
   Defaults to MPI_COMM_WORLD, if no communicator is passed in.
   

.. function:: proc commSize(comm: MPI_Comm = MPI_COMM_WORLD): c_int

   
   Wrapper to get the size of the communicator comm.
   Defaults to MPI_COMM_WORLD, if no communicator is passed in.
   

.. function:: proc Wait(ref request: MPI_Request, ref status: MPI_Status): c_int

   Drop in replacement for MPI_Wait, implemented with non-blocking MPI calls.
   
   This is simply implemented as a while loop that continually calls ``MPI_Test``. The
   loop will yield, allowing other tasks to run.
   

.. function:: proc Wait(ref request: MPI_Request): c_int

   Overloaded version of Wait, which ignores the returned status 

.. function:: proc Barrier(comm: MPI_Comm): c_int

   Drop in replacement for ``MPI_Barrier``, with non-blocking MPI calls.
   
   This is implemented by a call to ``MPI_Ibarrier``, followed by a call to ``Wait`` above. The
   returned value of ``MPI_Status`` is ignored.
   

.. function:: proc Send(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int

   Drop in replacement for ``MPI_Send``, with non-blocking MPI calls.
   
   This is implemented by a call to ``MPI_Isend``, followed by a call to ``Wait`` above. The
   returned value of ``MPI_Status`` is ignored.
   

.. function:: proc Recv(ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int

   Drop in replacement for ``MPI_Recv``, with non-blocking MPI calls.
   
   This is implemented by a call to ``MPI_Irecv``, followed by a call to ``Wait`` above. The
   returned value of ``MPI_Status`` is ignored.
   

.. record:: MPI_Status

   A wrapper around ``MPI_Status``. Only the defined fields are exposed 


   .. attribute:: var MPI_SOURCE: c_int

   .. attribute:: var MPI_TAG: c_int

   .. attribute:: var MPI_ERROR: c_int

.. method:: proc MPI_Status.getCount(tt: MPI_Datatype)

   Get the count from a status object 

.. type:: type MPI_Aint = c_ptrdiff

.. type:: type MPI_Group

.. type:: type MPI_Comm

.. type:: type MPI_Datatype

.. type:: type MPI_Request

.. type:: type MPI_Op

.. data:: const MPI_THREAD_SINGLE: c_int

.. data:: const MPI_THREAD_FUNNELED: c_int

.. data:: const MPI_THREAD_SERIALIZED: c_int

.. data:: const MPI_THREAD_MULTIPLE: c_int

.. type:: type MPI_Errhandler = opaque

.. data:: const MPI_SUCCESS: c_int

.. data:: const MPI_ERR_BUFFER: c_int

.. data:: const MPI_ERR_COUNT: c_int

.. data:: const MPI_ERR_TYPE: c_int

.. data:: const MPI_ERR_TAG: c_int

.. data:: const MPI_ERR_COMM: c_int

.. data:: const MPI_ERR_RANK: c_int

.. data:: const MPI_ERR_REQUEST: c_int

.. data:: const MPI_ERR_ROOT: c_int

.. data:: const MPI_ERR_GROUP: c_int

.. data:: const MPI_ERR_OP: c_int

.. data:: const MPI_ERR_TOPOLOGY: c_int

.. data:: const MPI_ERR_DIMS: c_int

.. data:: const MPI_ERR_ARG: c_int

.. data:: const MPI_ERR_UNKNOWN: c_int

.. data:: const MPI_ERR_TRUNCATE: c_int

.. data:: const MPI_ERR_OTHER: c_int

.. data:: const MPI_ERR_INTERN: c_int

.. data:: const MPI_PENDING: c_int

.. data:: const MPI_ERR_IN_STATUS: c_int

.. data:: const MPI_ERR_LASTCODE: c_int

.. data:: const MPI_BOTTOM: opaque

.. data:: const MPI_PROC_NULL: opaque

.. data:: const MPI_ANY_SOURCE: opaque

.. data:: const MPI_ANY_TAG: opaque

.. data:: const MPI_UNDEFINED: opaque

.. data:: const MPI_BSEND_OVERHEAD: opaque

.. data:: const MPI_KEYVAL_INVALID: opaque

.. data:: const MPI_ERRORS_ARE_FATAL: MPI_Errhandler

.. data:: const MPI_ERRORS_RETURN: MPI_Errhandler

.. data:: const MPI_MAX_PROCESSOR_NAME: c_int

.. data:: const MPI_MAX_ERROR_STRING: c_int

.. data:: const MPI_CHAR: MPI_Datatype

.. data:: const MPI_SHORT: MPI_Datatype

.. data:: const MPI_INT: MPI_Datatype

.. data:: const MPI_LONG: MPI_Datatype

.. data:: const MPI_UNSIGNED_CHAR: MPI_Datatype

.. data:: const MPI_UNSIGNED_SHORT: MPI_Datatype

.. data:: const MPI_UNSIGNED: MPI_Datatype

.. data:: const MPI_UNSIGNED_LONG: MPI_Datatype

.. data:: const MPI_FLOAT: MPI_Datatype

.. data:: const MPI_DOUBLE: MPI_Datatype

.. data:: const MPI_LONG_DOUBLE: MPI_Datatype

.. data:: const MPI_BYTE: MPI_Datatype

.. data:: const MPI_PACKED: MPI_Datatype

.. data:: const MPI_FLOAT_INT: MPI_Datatype

.. data:: const MPI_DOUBLE_INT: MPI_Datatype

.. data:: const MPI_LONG_INT: MPI_Datatype

.. data:: const MPI_2INT: MPI_Datatype

.. data:: const MPI_SHORT_INT: MPI_Datatype

.. data:: const MPI_LONG_DOUBLE_INT: MPI_Datatype

.. data:: const MPI_LONG_LONG_INT: MPI_Datatype

.. data:: const MPI_UB: MPI_Datatype

.. data:: const MPI_LB: MPI_Datatype

.. data:: const MPI_COMM_WORLD: MPI_Comm

.. data:: const MPI_COMM_SELF: MPI_Comm

.. data:: const MPI_COMM_NULL: MPI_Comm

.. data:: const MPI_IDENT: c_int

.. data:: const MPI_CONGRUENT: c_int

.. data:: const MPI_SIMILAR: c_int

.. data:: const MPI_UNEQUAL: c_int

.. data:: const MPI_TAG_UB: c_int

.. data:: const MPI_IO: c_int

.. data:: const MPI_HOST: c_int

.. data:: const MPI_WTIME_IS_GLOBAL: c_int

.. data:: const MPI_MAX: MPI_Op

.. data:: const MPI_MIN: MPI_Op

.. data:: const MPI_SUM: MPI_Op

.. data:: const MPI_PROD: MPI_Op

.. data:: const MPI_MAXLOC: MPI_Op

.. data:: const MPI_MINLOC: MPI_Op

.. data:: const MPI_BAND: MPI_Op

.. data:: const MPI_BOR: MPI_Op

.. data:: const MPI_BXOR: MPI_Op

.. data:: const MPI_LAND: MPI_Op

.. data:: const MPI_LOR: MPI_Op

.. data:: const MPI_LXOR: MPI_Op

