/*
 * Copyright 2004-2016 Cray Inc.
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

These implement the C-API for the MPI 1.1 standard. We currently do not support the following 
routines ::

      MPI_Op_create
      MPI_Op_free
      MPI_Errhandler_create
      MPI_Errhandler_free
      MPI_Keyval_create
      MPI_Keyval_free

since all of these are built around user-defined handlers, that we support.

.. note::
  #. Pointer arguments are written as `ref` areguments, so no casting to a ``c_ptr``
     is necessary.
  #. An exception to the above is if the C prototype names the argument ``array_of_*``,
     in which case we write it using an array form.
  #. Some MPI-1.1 functions were deprecated in MPI-2. These should be updated in the future, but
     are still present in this version.
  #. We represent MPI_Aint by ptrdiff. If this is not the correct size, there will be an assertion 
     failure in the code.
*/
module MPI {
  use SysCTypes;
  require "mpi.h";

  /* Automatically initialize, and define
   worldSize and worldRank, since we will
   likely use those often. 

   You are still responsible for calling MPI_Finalize,
   until Chapel has a module termination scheme.
   */
  config const autoInitMPI=true;

  /* Define module level variables with the rank and
   size in MPI_COMM_WORLD */
  var worldRank, worldSize : c_int;

  if autoInitMPI {
    var flag : c_int;
    C_MPI.MPI_Initialized(flag);
    if flag==0 then initialize();
  }

  /* Helper routine that also sets worldRank and worldSize */
  proc initialize() {
      C_MPI.MPI_Init(0,0);
      C_MPI.MPI_Comm_size(MPI_COMM_WORLD, worldSize);
      C_MPI.MPI_Comm_rank(MPI_COMM_WORLD, worldRank);
  }


  /* A wrapper around MPI_Status. Only the defined fields are exposed */
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
    

  /******************************
    Defined Constants and Datatypes
   ******************************/

  /* MPI types.
     We define these as opaque types.
     */
  extern type MPI_Aint = c_ptrdiff;
  extern type MPI_Group;
  extern type MPI_Comm;
  extern type MPI_Datatype;
  extern type MPI_Request;
  extern type MPI_Op;

  {
    pragma "no doc"
    pragma "no prototype"
    extern proc sizeof(type t): size_t;
    assert(sizeof(MPI_Aint) == sizeof(c_ptrdiff));
  }


  // TODO : Not explicily found in the spec
  extern type MPI_Errhandler = opaque;

  /* Return codes. 
     We define these to be `c_int`.
     */

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

  /* Assorted constants. 
  TODO : These are defined as opaque, but probably 
  could be more specific.
  */
  extern const MPI_BOTTOM : opaque;
  extern const MPI_PROC_NULL : opaque;
  extern const MPI_ANY_SOURCE : opaque;
  extern const MPI_ANY_TAG : opaque;
  extern const MPI_UNDEFINED : opaque;
  extern const MPI_BSEND_OVERHEAD : opaque;
  extern const MPI_KEYVAL_INVALID : opaque;

  /* Error handling specifiers */
  extern const MPI_ERRORS_ARE_FATAL : MPI_Errhandler;
  extern const MPI_ERRORS_RETURN : MPI_Errhandler;

  /* Maximum sizes for strings. 
  TODO: Are these correctly defined?
  */
  extern const MPI_MAX_PROCESSOR_NAME : c_int;
  extern const MPI_MAX_ERROR_STRING : c_int;

  /* Elementary datatypes */
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

  /* Datatypes for reductions */
  extern const MPI_FLOAT_INT : MPI_Datatype;
  extern const MPI_DOUBLE_INT : MPI_Datatype;
  extern const MPI_LONG_INT : MPI_Datatype;
  extern const MPI_2INT : MPI_Datatype;
  extern const MPI_SHORT_INT : MPI_Datatype;
  extern const MPI_LONG_DOUBLE_INT : MPI_Datatype;

  /* Optional datatypes */
  extern const MPI_LONG_LONG_INT : MPI_Datatype;

  /* Special datatypes for constructing derived datatypes */
  extern const MPI_UB : MPI_Datatype;
  extern const MPI_LB : MPI_Datatype;

  /* Reserved communicators */
  extern const MPI_COMM_WORLD : MPI_Comm;
  extern const MPI_COMM_SELF : MPI_Comm;

  /* Communicator/ Group comparisons */
  // These all appear to be C integers in MPI code
  extern const MPI_IDENT : c_int;
  extern const MPI_CONGRUENT : c_int;
  extern const MPI_SIMILAR : c_int;
  extern const MPI_UNEQUAL : c_int;

  /* Environmental inquiry keys */ 
  // These appear to be C enums or defines....
  extern const MPI_TAG_UB : c_int; 
  extern const MPI_IO : c_int; 
  extern const MPI_HOST : c_int; 
  extern const MPI_WTIME_IS_GLOBAL : c_int; 

  /* Collective operations */
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
   C-API 

   We wrap all of these into a ``C_MPI`` submodule,
   since we likely will add in some helper routines
   above. We don't do this for the constants, since
   we'll likely end up using these more often.

   */
   module C_MPI {

  /* Special case MPI_Init -- we will send these null pointers
   and let the compiler do all the munging */
  extern proc MPI_Init(argc, argv);


  /* Collective commands */
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

  /* Environmental inquiries */
  extern proc MPI_Get_processor_name (ref name: c_char, ref resultlen: c_int): c_int;
  extern proc MPI_Errhandler_set (comm: MPI_Comm, errhandler: MPI_Errhandler): c_int;
  extern proc MPI_Errhandler_get (comm: MPI_Comm, ref errhandler: MPI_Errhandler): c_int;
  extern proc MPI_Error_string (errorcode: c_int, ref string: c_char, ref resultlen: c_int): c_int;
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

  /* Point to Point */
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

  /* Process topologies */
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
}
