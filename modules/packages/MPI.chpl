/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* MPI Bindings for Chapel.


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

*/
module MPI {
  public use SysCTypes;
  require "mpi.h";

  use ReplicatedVar;

  /*
     Automatically initializes MPI (if not already initialized by the runtime), and
     shuts it down as well.
   */
  config const autoInitMPI=true;

  /*
    Ensures that MPI is running in :const:`MPI_THREAD_MULTIPLE` mode, and will
    abort if not. This is not necessary if you are running SPMD mode, but is
    likely to be necessary if you run
    in multilocale mode.
   */
  config const requireThreadedMPI=true;

  pragma "no doc"
  config const debugMPI=false;

  pragma "no doc"
  var CHPL_COMM_WORLD_REPLICATED : [rcDomain] MPI_Comm;
  rcReplicate(CHPL_COMM_WORLD_REPLICATED, MPI_COMM_NULL);

  /*
    Define a new communicator that directly maps to Chapel locales.
    This is just a reordering of ``MPI_COMM_WORLD``, which you are, of course,
    free to continue to use. This just guarantees that locale ids and MPI
    ranks agree.
  */
  proc CHPL_COMM_WORLD {
    return CHPL_COMM_WORLD_REPLICATED(1);
  }

  pragma "no doc"
  var _doinit : bool = false;

  pragma "no doc"
  var _freeChplComm : bool = false;

  pragma "no doc"
  /* Module level deinit */
  proc deinit() {
    if _freeChplComm {
      coforall loc in Locales do on loc {
          C_MPI.MPI_Comm_free(CHPL_COMM_WORLD_REPLICATED(1));
        }
    }
    if _doinit {
      coforall loc in Locales do
        on loc {
          // TODO : Need a gasnet barrier here???
          if debugMPI then writeln("Calling MPI_Finalize....");
          C_MPI.MPI_Finalize();
        }
    }
  }

  // Initialization routine
  if autoInitMPI {
    if debugMPI then writeln("Attempting to auto-initialize MPI.....");
    var flag : c_int;
    C_MPI.MPI_Initialized(flag);
    if flag==0 {
      if debugMPI then writeln("Initializing MPI....");
      _doinit = true;
      initialize();
    } else {
      var provided : c_int;
      if debugMPI then writeln("MPI already initialized.....");
      C_MPI.MPI_Query_thread(provided);
      if (provided != MPI_THREAD_MULTIPLE) &&
         requireThreadedMPI
      {
        halt("Unable to get a high enough MPI thread support.\nTry setting 'MPICH_MAX_THREAD_SAFETY=multiple' or its equivalent");
      }
      setChplComm();
    }
  }


  /*
     Helper routine that also sets process IDs (world rank) and number of
     processes (world size)
     */
  proc initialize() {
    // If we are running using the uGNI layer, then the following hack
    // appears to be necessary in order to run MPI, as well as Chapel
    // See : https://hpcrdm.lbl.gov/pipermail/upc-users/2014-May/002061.html
    if (CHPL_COMM=="ugni") {
      coforall loc in Locales do on loc {
          // This must be done on all locales!
          var pmiGniCookie = C_Env.getenv("PMI_GNI_COOKIE") : string;
          if !pmiGniCookie.isEmpty() {
            // This may be a colon separated string.
            var cookieJar = pmiGniCookie.split(":");
            const lastcookie = cookieJar.domain.last;
            try {
              cookieJar[lastcookie] = ((cookieJar[lastcookie]):int + 1):string;
            } catch e {
              halt("Unable to parse PMI_GNI_COOKIE");
            }
            const newVal = ":".join(cookieJar);
            C_Env.setenv("PMI_GNI_COOKIE",newVal.c_str(),1);
          }
        }
    }

    // The actual MPI initialization goes here.
    coforall loc in Locales do on loc {
      // TODO : Need a gasnet barrier here???
      var provided : c_int;
      C_MPI.MPI_Init_thread(nil, nil, MPI_THREAD_MULTIPLE, provided);
      if (provided != MPI_THREAD_MULTIPLE) &&
         requireThreadedMPI
      {
        halt("Unable to get a high enough MPI thread support.\nTry setting 'MPICH_MAX_THREAD_SAFETY=multiple' or its equivalent");
      }
      C_MPI.MPI_Barrier(MPI_COMM_WORLD);
    }
    setChplComm();
  }

  pragma "no doc"
  proc setChplComm() {
    if numLocales > 1 {
      coforall loc in Locales do on loc {
        C_MPI.MPI_Comm_split(MPI_COMM_WORLD, 0, here.id : c_int,
          CHPL_COMM_WORLD_REPLICATED(1));
      }
    } else {
      // For a single Chapel locale, just duplicate MPI_COMM_WORLD
      // NOTE : This split in logic is necessary, especially if Chapel is running
      // in single-locale mode, but we want to continue to use MPI.
      C_MPI.MPI_Comm_dup(MPI_COMM_WORLD, CHPL_COMM_WORLD_REPLICATED(1));
    }

    // Set flag to free
    _freeChplComm = true;
  }

  /*
     Wrapper to get the rank of the communicator comm.
     Defaults to MPI_COMM_WORLD, if no communicator is passed in.
     */
  proc commRank(comm : MPI_Comm = MPI_COMM_WORLD) : c_int {
    var rank : c_int;
    C_MPI.MPI_Comm_rank(comm, rank);
    return rank;
  }

  /*
     Wrapper to get the size of the communicator comm.
     Defaults to MPI_COMM_WORLD, if no communicator is passed in.
     */
  proc commSize(comm : MPI_Comm = MPI_COMM_WORLD) : c_int {
    var size : c_int;
    C_MPI.MPI_Comm_size(comm, size);
    return size;
  }

  /* Drop in replacement for MPI_Wait, implemented with non-blocking MPI calls.

     This is simply implemented as a while loop that continually calls ``MPI_Test``. The
     loop will yield, allowing other tasks to run.
   */
  inline proc Wait(ref request: MPI_Request, ref status: MPI_Status): c_int {
    var flag, ret : c_int;
    ret = C_MPI.MPI_Test(request, flag, status);
    while (flag==0) {
      chpl_task_yield();
      ret = C_MPI.MPI_Test(request, flag, status);
    }
    return ret;
  }

  /* Overloaded version of Wait, which ignores the returned status */
  inline proc Wait(ref request: MPI_Request): c_int {
    var status: MPI_Status;
    return Wait(request, status);
  }

  /* Drop in replacement for ``MPI_Barrier``, with non-blocking MPI calls.

     This is implemented by a call to ``MPI_Ibarrier``, followed by a call to ``Wait`` above. The
     returned value of ``MPI_Status`` is ignored.
   */
  proc Barrier(comm: MPI_Comm): c_int {
    var request: MPI_Request;

    var ret = C_MPI.MPI_Ibarrier(comm, request);
    Wait(request);
    return ret;
  }

  /* Drop in replacement for ``MPI_Send``, with non-blocking MPI calls.

     This is implemented by a call to ``MPI_Isend``, followed by a call to ``Wait`` above. The
     returned value of ``MPI_Status`` is ignored.
   */
  proc Send(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int {
    var request: MPI_Request;
    var ret = MPI_Isend (buf, count, datatype, dest, tag, comm, request);
    Wait(request);
    return ret;
  }

  /* Drop in replacement for ``MPI_Recv``, with non-blocking MPI calls.

     This is implemented by a call to ``MPI_Irecv``, followed by a call to ``Wait`` above. The
     returned value of ``MPI_Status`` is ignored.
   */
  proc Recv(ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int {
    var request: MPI_Request;
    var ret = MPI_Irecv(buf, count, datatype, source, tag, comm, request);
    Wait(request, status);
    return ret;
  }

  /* A wrapper around ``MPI_Status``. Only the defined fields are exposed */
  extern record MPI_Status {
    var MPI_SOURCE : c_int;
    var MPI_TAG : c_int;
    var MPI_ERROR : c_int;
    /* The other fields are implementation dependent */
  }

  /* Get the count from a status object */
  proc MPI_Status.getCount(tt : MPI_Datatype) {
    var count : c_int;
    C_MPI.MPI_Get_count(this, tt, count);
    return count : int;
  }


  //Defined Constants and Datatypes

  // MPI types.  We define these as opaque types.
  extern type MPI_Aint = c_ptrdiff;
  extern type MPI_Group;
  extern type MPI_Comm;
  extern type MPI_Datatype;
  extern type MPI_Request;
  extern type MPI_Op;

  {
    pragma "no doc"
    extern proc sizeof(type t): size_t;
    assert(sizeof(MPI_Aint) == sizeof(c_ptrdiff));
  }

  // MPI Thread support
  extern const MPI_THREAD_SINGLE : c_int;
  extern const MPI_THREAD_FUNNELED : c_int;
  extern const MPI_THREAD_SERIALIZED : c_int;
  extern const MPI_THREAD_MULTIPLE : c_int;

  // TODO : Not explicitly found in the spec
  extern type MPI_Errhandler = opaque;

  // Return codes.  We define these to be `c_int`.
  extern const MPI_SUCCESS : c_int;
  extern const MPI_ERR_BUFFER : c_int;
  extern const MPI_ERR_COUNT : c_int;
  extern const MPI_ERR_TYPE : c_int;
  extern const MPI_ERR_TAG : c_int;
  extern const MPI_ERR_COMM : c_int;
  extern const MPI_ERR_RANK : c_int;
  extern const MPI_ERR_REQUEST : c_int;
  extern const MPI_ERR_ROOT : c_int;
  extern const MPI_ERR_GROUP : c_int;
  extern const MPI_ERR_OP : c_int;
  extern const MPI_ERR_TOPOLOGY : c_int;
  extern const MPI_ERR_DIMS : c_int;
  extern const MPI_ERR_ARG : c_int;
  extern const MPI_ERR_UNKNOWN : c_int;
  extern const MPI_ERR_TRUNCATE : c_int;
  extern const MPI_ERR_OTHER : c_int;
  extern const MPI_ERR_INTERN : c_int;
  extern const MPI_PENDING : c_int;
  extern const MPI_ERR_IN_STATUS : c_int;
  extern const MPI_ERR_LASTCODE : c_int;

  // Assorted constants.
  //  TODO : These are defined as opaque, but probably could be more specific.
  extern const MPI_BOTTOM : opaque;
  extern const MPI_PROC_NULL : opaque;
  extern const MPI_ANY_SOURCE : opaque;
  extern const MPI_ANY_TAG : opaque;
  extern const MPI_UNDEFINED : opaque;
  extern const MPI_BSEND_OVERHEAD : opaque;
  extern const MPI_KEYVAL_INVALID : opaque;

  // Error handling specifiers
  extern const MPI_ERRORS_ARE_FATAL : MPI_Errhandler;
  extern const MPI_ERRORS_RETURN : MPI_Errhandler;

  // Maximum sizes for strings.
  //    TODO: Are these correctly defined?
  extern const MPI_MAX_PROCESSOR_NAME : c_int;
  extern const MPI_MAX_ERROR_STRING : c_int;

  // Elementary datatypes
  extern const MPI_CHAR : MPI_Datatype;
  extern const MPI_SHORT : MPI_Datatype;
  extern const MPI_INT   : MPI_Datatype;
  extern const MPI_LONG  : MPI_Datatype;
  extern const MPI_UNSIGNED_CHAR : MPI_Datatype;
  extern const MPI_UNSIGNED_SHORT : MPI_Datatype;
  extern const MPI_UNSIGNED : MPI_Datatype;
  extern const MPI_UNSIGNED_LONG : MPI_Datatype;
  extern const MPI_FLOAT  : MPI_Datatype;
  extern const MPI_DOUBLE  : MPI_Datatype;
  extern const MPI_LONG_DOUBLE : MPI_Datatype;
  extern const MPI_BYTE  : MPI_Datatype;
  extern const MPI_PACKED : MPI_Datatype;

  // Datatypes for reductions
  extern const MPI_FLOAT_INT : MPI_Datatype;
  extern const MPI_DOUBLE_INT : MPI_Datatype;
  extern const MPI_LONG_INT : MPI_Datatype;
  extern const MPI_2INT : MPI_Datatype;
  extern const MPI_SHORT_INT : MPI_Datatype;
  extern const MPI_LONG_DOUBLE_INT : MPI_Datatype;

  // Optional datatypes
  extern const MPI_LONG_LONG_INT : MPI_Datatype;

  // Special datatypes for constructing derived datatypes
  extern const MPI_UB : MPI_Datatype;
  extern const MPI_LB : MPI_Datatype;

  // Reserved communicators
  extern const MPI_COMM_WORLD : MPI_Comm;
  extern const MPI_COMM_SELF : MPI_Comm;
  extern const MPI_COMM_NULL : MPI_Comm;

  // Communicator/ Group comparisons
  // These all appear to be C integers in MPI code
  extern const MPI_IDENT : c_int;
  extern const MPI_CONGRUENT : c_int;
  extern const MPI_SIMILAR : c_int;
  extern const MPI_UNEQUAL : c_int;

  // Environmental inquiry keys
  // These appear to be C enums or defines....
  extern const MPI_TAG_UB : c_int;
  extern const MPI_IO : c_int;
  extern const MPI_HOST : c_int;
  extern const MPI_WTIME_IS_GLOBAL : c_int;

  // Collective operations
  extern const MPI_MAX : MPI_Op;
  extern const MPI_MIN : MPI_Op;
  extern const MPI_SUM : MPI_Op;
  extern const MPI_PROD : MPI_Op;
  extern const MPI_MAXLOC : MPI_Op;
  extern const MPI_MINLOC : MPI_Op;
  extern const MPI_BAND : MPI_Op;
  extern const MPI_BOR : MPI_Op;
  extern const MPI_BXOR : MPI_Op;
  extern const MPI_LAND : MPI_Op;
  extern const MPI_LOR : MPI_Op;
  extern const MPI_LXOR : MPI_Op;

  /*
   Low level MPI bindings in Chapel.

   The :mod:`MPI` module wraps this submodule to provide a higher level API.
   For documentation of the routines in this modules, please refer to your MPI
   documentation.

   */
   module C_MPI {
     use SysCTypes;
     use MPI;

  // Special case MPI_Init -- we will send these null pointers
  // and let the compiler do all the munging
  extern proc MPI_Init(argc, argv);

  // Special cases, for threading support
  extern proc MPI_Init_thread(argc, argv, required : c_int, ref provided : c_int) : c_int;
  extern proc MPI_Query_thread(ref provided : c_int) : c_int;
  extern proc MPI_Ibarrier(comm : MPI_Comm, ref request : MPI_Request) : c_int;

  // Collective commands
  extern proc MPI_Barrier (comm: MPI_Comm): c_int;
  extern proc MPI_Bcast (ref buffer, count: c_int, datatype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Gather (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Gatherv (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, ref recvcounts: c_int, ref displs: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Scatter (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Scatterv (ref sendbuf, ref sendcounts: c_int, ref displs: c_int, sendtype: MPI_Datatype,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Allgather (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int;
  extern proc MPI_Allgatherv (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, ref recvcounts: c_int, ref displs: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int;
  extern proc MPI_Alltoall (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int;
  extern proc MPI_Alltoallv (ref sendbuf, ref sendcounts: c_int, ref sdispls: c_int, sendtype: MPI_Datatype,
      ref recvbuf, ref recvcounts: c_int, ref rdispls: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int;
  extern proc MPI_Reduce (ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, root: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Allreduce (ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int;
  extern proc MPI_Reduce_scatter (ref sendbuf, ref recvbuf, ref recvcounts: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int;
  extern proc MPI_Scan (ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int;

  // Environmental inquiries
  extern proc MPI_Get_processor_name (ref name: c_char, ref resultlen: c_int): c_int;
  extern proc MPI_Errhandler_set (comm: MPI_Comm, errhandler: MPI_Errhandler): c_int;
  extern proc MPI_Errhandler_get (comm: MPI_Comm, ref errhandler: MPI_Errhandler): c_int;
  extern proc MPI_Error_string (errorcode: c_int, ref str: c_char, ref resultlen: c_int): c_int;
  extern proc MPI_Error_class (errorcode: c_int, ref errorclass: c_int): c_int;
  extern proc MPI_Wtime (): c_double;
  extern proc MPI_Wtick (): c_double;
  extern proc MPI_Finalize (): c_int;
  extern proc MPI_Initialized (ref flag: c_int): c_int;
  extern proc MPI_Abort (comm: MPI_Comm, errorcode: c_int): c_int;

  /* Groups, communicators etc ... */
  extern proc MPI_Group_size (group: MPI_Group, ref size: c_int): c_int;
  extern proc MPI_Group_rank (group: MPI_Group, ref rank: c_int): c_int;
  extern proc MPI_Group_translate_ranks (group1: MPI_Group, n: c_int, ref ranks1: c_int, group2: MPI_Group, ref ranks2: c_int): c_int;
  extern proc MPI_Group_compare (group1: MPI_Group, group2: MPI_Group, ref result: c_int): c_int;
  extern proc MPI_Comm_group (comm: MPI_Comm, ref group: MPI_Group): c_int;
  extern proc MPI_Group_union (group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_intersection (group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_difference (group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_incl (group: MPI_Group, n: c_int, ref ranks: c_int, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_excl (group: MPI_Group, n: c_int, ref ranks: c_int, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_range_incl (group: MPI_Group, n: c_int, ranges: []c_int, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_range_excl (group: MPI_Group, n: c_int, ranges: []c_int, ref newgroup: MPI_Group): c_int;
  extern proc MPI_Group_free (ref group: MPI_Group): c_int;
  extern proc MPI_Comm_size (comm: MPI_Comm, ref size: c_int): c_int;
  extern proc MPI_Comm_rank (comm: MPI_Comm, ref rank: c_int): c_int;
  extern proc MPI_Comm_compare (comm1: MPI_Comm, comm2: MPI_Comm, ref result: c_int): c_int;
  extern proc MPI_Comm_dup (comm: MPI_Comm, ref newcomm: MPI_Comm): c_int;
  extern proc MPI_Comm_create (comm: MPI_Comm, group: MPI_Group, ref newcomm: MPI_Comm): c_int;
  extern proc MPI_Comm_split (comm: MPI_Comm, color: c_int, key: c_int, ref newcomm: MPI_Comm): c_int;
  extern proc MPI_Comm_free (ref comm: MPI_Comm): c_int;
  extern proc MPI_Comm_test_inter (comm: MPI_Comm, ref flag: c_int): c_int;
  extern proc MPI_Comm_remote_size (comm: MPI_Comm, ref size: c_int): c_int;
  extern proc MPI_Comm_remote_group (comm: MPI_Comm, ref group: MPI_Group): c_int;
  extern proc MPI_Intercomm_create (local_comm: MPI_Comm, local_leader: c_int, peer_comm: MPI_Comm,
      remote_leader: c_int, tag: c_int, ref newintercomm: MPI_Comm): c_int;
  extern proc MPI_Intercomm_merge (intercomm: MPI_Comm, high: c_int, ref newintracomm: MPI_Comm): c_int;
  extern proc MPI_Attr_put (comm: MPI_Comm, keyval: c_int, ref attribute_val): c_int;
  extern proc MPI_Attr_get (comm: MPI_Comm, keyval: c_int, ref attribute_val, ref flag: c_int): c_int;
  extern proc MPI_Attr_delete (comm: MPI_Comm, keyval: c_int): c_int;

  // Point to Point
  extern proc MPI_Send (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Recv (ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int;
  extern proc MPI_Get_count (ref status: MPI_Status, datatype: MPI_Datatype, ref count: c_int): c_int;
  extern proc MPI_Bsend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Ssend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Rsend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Buffer_attach (ref buffer, size: c_int): c_int;
  extern proc MPI_Buffer_detach (ref buffer, ref size: c_int): c_int;
  extern proc MPI_Isend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Ibsend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Issend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Irsend (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Irecv (ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Wait (ref request: MPI_Request, ref status: MPI_Status): c_int;
  extern proc MPI_Test (ref request: MPI_Request, ref flag: c_int, ref status: MPI_Status): c_int;
  extern proc MPI_Request_free (ref request: MPI_Request): c_int;
  extern proc MPI_Waitany (count: c_int, array_of_requests: []MPI_Request, ref iindex : c_int, ref status: MPI_Status): c_int;
  extern proc MPI_Testany (count: c_int, array_of_requests: []MPI_Request, ref iindex : c_int, ref flag: c_int, ref status: MPI_Status): c_int;
  extern proc MPI_Waitall (count: c_int, array_of_requests: []MPI_Request, array_of_statuses: []MPI_Status): c_int;
  extern proc MPI_Testall (count: c_int, array_of_requests: []MPI_Request, ref flag: c_int, array_of_statuses: []MPI_Status): c_int;
  extern proc MPI_Waitsome (incount: c_int, array_of_requests: []MPI_Request,
      ref outcount: c_int, array_of_indices: []c_int, array_of_statuses: []MPI_Status): c_int;
  extern proc MPI_Testsome (incount: c_int, array_of_requests: []MPI_Request,
      ref outcount: c_int, array_of_indices: []c_int, array_of_statuses: []MPI_Status): c_int;
  extern proc MPI_Iprobe (source: c_int, tag: c_int, comm: MPI_Comm, ref flag: c_int, ref status: MPI_Status): c_int;
  extern proc MPI_Probe (source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int;
  extern proc MPI_Cancel (ref request: MPI_Request): c_int;
  extern proc MPI_Test_cancelled (ref status: MPI_Status, ref flag: c_int): c_int;
  extern proc MPI_Send_init (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Bsend_init (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Ssend_init (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Rsend_init (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Recv_init (ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int;
  extern proc MPI_Start (ref request: MPI_Request): c_int;
  extern proc MPI_Startall (count: c_int, array_of_requests: []MPI_Request): c_int;
  extern proc MPI_Sendrecv (ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, dest: c_int, sendtag: c_int,
      ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, source: c_int, recvtag: MPI_Datatype, comm: MPI_Comm, ref status: MPI_Status): c_int;
  extern proc MPI_Sendrecv_replace (ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, sendtag: c_int,
      source: c_int, recvtag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int;
  extern proc MPI_Type_contiguous (count: c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Type_vector (count: c_int, blocklength: c_int, stride: c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Type_hvector (count: c_int, blocklength: c_int, stride: MPI_Aint, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Type_indexed (count: c_int, array_of_blocklengths: []c_int,
      array_of_displacements: []c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Type_hindexed (count: c_int, array_of_blocklengths: []c_int,
      array_of_displacements: []MPI_Aint, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Type_struct (count: c_int, array_of_blocklengths: []c_int,
      array_of_displacements: []MPI_Aint, array_of_types: []MPI_Datatype, ref newtype: MPI_Datatype): c_int;
  extern proc MPI_Address (ref location, ref address: MPI_Aint): c_int;
  extern proc MPI_Type_extent (datatype: MPI_Datatype, ref extent: MPI_Aint): c_int;
  extern proc MPI_Type_size (datatype: MPI_Datatype, ref size: c_int): c_int;
  extern proc MPI_Type_lb (datatype: MPI_Datatype, ref displacement: MPI_Aint): c_int;
  extern proc MPI_Type_ub (datatype: MPI_Datatype, ref displacement: MPI_Aint): c_int;
  extern proc MPI_Type_commit (ref datatype: MPI_Datatype): c_int;
  extern proc MPI_Type_free (ref datatype: MPI_Datatype): c_int;
  extern proc MPI_Get_elements (ref status: MPI_Status, datatype: MPI_Datatype, ref count: c_int): c_int;
  extern proc MPI_Pack (ref inbuf, incount: c_int, datatype: MPI_Datatype, ref outbuf, outsize: c_int, ref position: c_int, comm: MPI_Comm): c_int;
  extern proc MPI_Unpack (ref inbuf, insize: c_int, ref position: c_int, ref outbuf, outcount: c_int, datatype: MPI_Datatype, comm: MPI_Comm): c_int;
  extern proc MPI_Pack_size (incount: c_int, datatype: MPI_Datatype, comm: MPI_Comm, ref size: c_int): c_int;

  // Process topologies
  extern proc MPI_Cart_create (comm_old: MPI_Comm, ndims: c_int, ref dims: c_int, ref periods: c_int, reorder: c_int, ref comm_cart: MPI_Comm): c_int;
  extern proc MPI_Dims_create (nnodes: c_int, ndims: c_int, ref dims: c_int): c_int;
  extern proc MPI_Graph_create (comm_old: MPI_Comm, nnodes: c_int, ref iindex: c_int, ref edges: c_int, reorder: c_int, ref comm_graph: MPI_Comm): c_int;
  extern proc MPI_Topo_test (comm: MPI_Comm, ref status: c_int): c_int;
  extern proc MPI_Graphdims_get (comm: MPI_Comm, ref nnodes: c_int, ref nedges: c_int): c_int;
  extern proc MPI_Graph_get (comm: MPI_Comm, maxindex: c_int, maxedges: c_int, ref iindex: c_int, ref edges: c_int): c_int;
  extern proc MPI_Cartdim_get (comm: MPI_Comm, ref ndims: c_int): c_int;
  extern proc MPI_Cart_get (comm: MPI_Comm, maxdims: c_int, ref dims: c_int, ref periods: c_int, ref coords: c_int): c_int;
  extern proc MPI_Cart_rank (comm: MPI_Comm, ref coords: c_int, ref rank: c_int): c_int;
  extern proc MPI_Cart_coords (comm: MPI_Comm, rank: c_int, maxdims: c_int, ref coords: c_int): c_int;
  extern proc MPI_Graph_neighbors_count (comm: MPI_Comm, rank: c_int, ref nneighbors: c_int): c_int;
  extern proc MPI_Graph_neighbors (comm: MPI_Comm, rank: c_int, maxneighbors: c_int, ref neighbors: c_int): c_int;
  extern proc MPI_Cart_shift (comm: MPI_Comm, direction: c_int, disp: c_int, ref rank_source: c_int, ref rank_dest: c_int): c_int;
  extern proc MPI_Cart_sub (comm: MPI_Comm, ref remain_dims: c_int, ref newcomm: MPI_Comm): c_int;
  extern proc MPI_Cart_map (comm: MPI_Comm, ndims: c_int, ref dims: c_int, ref periods: c_int, ref newrank: c_int): c_int;
  extern proc MPI_Graph_map (comm: MPI_Comm, nnodes: c_int, ref iindex: c_int, ref edges: c_int, ref newrank: c_int): c_int;

   } // End C_MPI


  module C_Env {
    use SysCTypes;
    // Helper routines to access the environment
    extern proc getenv(name : c_string) : c_string;
    extern proc setenv(name : c_string, envval : c_string, overwrite : c_int) : c_int;
  }
}
