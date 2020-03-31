.. default-domain:: chpl

.. module:: C_MPI
   :synopsis: Low level MPI bindings in Chapel.

C_MPI
=====
**Usage**

.. code-block:: chapel

   use MPI.C_MPI;


Low level MPI bindings in Chapel.

The :mod:`MPI` module wraps this submodule to provide a higher level API.
For documentation of the routines in this modules, please refer to your MPI
documentation.



.. function:: proc MPI_Init(argc, argv)

.. function:: proc MPI_Init_thread(argc, argv, required: c_int, ref provided: c_int): c_int

.. function:: proc MPI_Query_thread(ref provided: c_int): c_int

.. function:: proc MPI_Ibarrier(comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Barrier(comm: MPI_Comm): c_int

.. function:: proc MPI_Bcast(ref buffer, count: c_int, datatype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Gather(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Gatherv(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, ref recvcounts: c_int, ref displs: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Scatter(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Scatterv(ref sendbuf, ref sendcounts: c_int, ref displs: c_int, sendtype: MPI_Datatype, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Allgather(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int

.. function:: proc MPI_Allgatherv(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, ref recvcounts: c_int, ref displs: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int

.. function:: proc MPI_Alltoall(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int

.. function:: proc MPI_Alltoallv(ref sendbuf, ref sendcounts: c_int, ref sdispls: c_int, sendtype: MPI_Datatype, ref recvbuf, ref recvcounts: c_int, ref rdispls: c_int, recvtype: MPI_Datatype, comm: MPI_Comm): c_int

.. function:: proc MPI_Reduce(ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, root: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Allreduce(ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int

.. function:: proc MPI_Reduce_scatter(ref sendbuf, ref recvbuf, ref recvcounts: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int

.. function:: proc MPI_Scan(ref sendbuf, ref recvbuf, count: c_int, datatype: MPI_Datatype, op: MPI_Op, comm: MPI_Comm): c_int

.. function:: proc MPI_Get_processor_name(ref name: c_char, ref resultlen: c_int): c_int

.. function:: proc MPI_Errhandler_set(comm: MPI_Comm, errhandler: MPI_Errhandler): c_int

.. function:: proc MPI_Errhandler_get(comm: MPI_Comm, ref errhandler: MPI_Errhandler): c_int

.. function:: proc MPI_Error_string(errorcode: c_int, ref str: c_char, ref resultlen: c_int): c_int

.. function:: proc MPI_Error_class(errorcode: c_int, ref errorclass: c_int): c_int

.. function:: proc MPI_Wtime(): c_double

.. function:: proc MPI_Wtick(): c_double

.. function:: proc MPI_Finalize(): c_int

.. function:: proc MPI_Initialized(ref flag: c_int): c_int

.. function:: proc MPI_Abort(comm: MPI_Comm, errorcode: c_int): c_int

.. function:: proc MPI_Group_size(group: MPI_Group, ref size: c_int): c_int

   Groups, communicators etc ... 

.. function:: proc MPI_Group_rank(group: MPI_Group, ref rank: c_int): c_int

.. function:: proc MPI_Group_translate_ranks(group1: MPI_Group, n: c_int, ref ranks1: c_int, group2: MPI_Group, ref ranks2: c_int): c_int

.. function:: proc MPI_Group_compare(group1: MPI_Group, group2: MPI_Group, ref result: c_int): c_int

.. function:: proc MPI_Comm_group(comm: MPI_Comm, ref group: MPI_Group): c_int

.. function:: proc MPI_Group_union(group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_intersection(group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_difference(group1: MPI_Group, group2: MPI_Group, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_incl(group: MPI_Group, n: c_int, ref ranks: c_int, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_excl(group: MPI_Group, n: c_int, ref ranks: c_int, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_range_incl(group: MPI_Group, n: c_int, ranges: [] c_int, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_range_excl(group: MPI_Group, n: c_int, ranges: [] c_int, ref newgroup: MPI_Group): c_int

.. function:: proc MPI_Group_free(ref group: MPI_Group): c_int

.. function:: proc MPI_Comm_size(comm: MPI_Comm, ref size: c_int): c_int

.. function:: proc MPI_Comm_rank(comm: MPI_Comm, ref rank: c_int): c_int

.. function:: proc MPI_Comm_compare(comm1: MPI_Comm, comm2: MPI_Comm, ref result: c_int): c_int

.. function:: proc MPI_Comm_dup(comm: MPI_Comm, ref newcomm: MPI_Comm): c_int

.. function:: proc MPI_Comm_create(comm: MPI_Comm, group: MPI_Group, ref newcomm: MPI_Comm): c_int

.. function:: proc MPI_Comm_split(comm: MPI_Comm, color: c_int, key: c_int, ref newcomm: MPI_Comm): c_int

.. function:: proc MPI_Comm_free(ref comm: MPI_Comm): c_int

.. function:: proc MPI_Comm_test_inter(comm: MPI_Comm, ref flag: c_int): c_int

.. function:: proc MPI_Comm_remote_size(comm: MPI_Comm, ref size: c_int): c_int

.. function:: proc MPI_Comm_remote_group(comm: MPI_Comm, ref group: MPI_Group): c_int

.. function:: proc MPI_Intercomm_create(local_comm: MPI_Comm, local_leader: c_int, peer_comm: MPI_Comm, remote_leader: c_int, tag: c_int, ref newintercomm: MPI_Comm): c_int

.. function:: proc MPI_Intercomm_merge(intercomm: MPI_Comm, high: c_int, ref newintracomm: MPI_Comm): c_int

.. function:: proc MPI_Attr_put(comm: MPI_Comm, keyval: c_int, ref attribute_val): c_int

.. function:: proc MPI_Attr_get(comm: MPI_Comm, keyval: c_int, ref attribute_val, ref flag: c_int): c_int

.. function:: proc MPI_Attr_delete(comm: MPI_Comm, keyval: c_int): c_int

.. function:: proc MPI_Send(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Recv(ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int

.. function:: proc MPI_Get_count(ref status: MPI_Status, datatype: MPI_Datatype, ref count: c_int): c_int

.. function:: proc MPI_Bsend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Ssend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Rsend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Buffer_attach(ref buffer, size: c_int): c_int

.. function:: proc MPI_Buffer_detach(ref buffer, ref size: c_int): c_int

.. function:: proc MPI_Isend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Ibsend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Issend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Irsend(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Irecv(ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Wait(ref request: MPI_Request, ref status: MPI_Status): c_int

.. function:: proc MPI_Test(ref request: MPI_Request, ref flag: c_int, ref status: MPI_Status): c_int

.. function:: proc MPI_Request_free(ref request: MPI_Request): c_int

.. function:: proc MPI_Waitany(count: c_int, array_of_requests: [] MPI_Request, ref iindex: c_int, ref status: MPI_Status): c_int

.. function:: proc MPI_Testany(count: c_int, array_of_requests: [] MPI_Request, ref iindex: c_int, ref flag: c_int, ref status: MPI_Status): c_int

.. function:: proc MPI_Waitall(count: c_int, array_of_requests: [] MPI_Request, array_of_statuses: [] MPI_Status): c_int

.. function:: proc MPI_Testall(count: c_int, array_of_requests: [] MPI_Request, ref flag: c_int, array_of_statuses: [] MPI_Status): c_int

.. function:: proc MPI_Waitsome(incount: c_int, array_of_requests: [] MPI_Request, ref outcount: c_int, array_of_indices: [] c_int, array_of_statuses: [] MPI_Status): c_int

.. function:: proc MPI_Testsome(incount: c_int, array_of_requests: [] MPI_Request, ref outcount: c_int, array_of_indices: [] c_int, array_of_statuses: [] MPI_Status): c_int

.. function:: proc MPI_Iprobe(source: c_int, tag: c_int, comm: MPI_Comm, ref flag: c_int, ref status: MPI_Status): c_int

.. function:: proc MPI_Probe(source: c_int, tag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int

.. function:: proc MPI_Cancel(ref request: MPI_Request): c_int

.. function:: proc MPI_Test_cancelled(ref status: MPI_Status, ref flag: c_int): c_int

.. function:: proc MPI_Send_init(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Bsend_init(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Ssend_init(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Rsend_init(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Recv_init(ref buf, count: c_int, datatype: MPI_Datatype, source: c_int, tag: c_int, comm: MPI_Comm, ref request: MPI_Request): c_int

.. function:: proc MPI_Start(ref request: MPI_Request): c_int

.. function:: proc MPI_Startall(count: c_int, array_of_requests: [] MPI_Request): c_int

.. function:: proc MPI_Sendrecv(ref sendbuf, sendcount: c_int, sendtype: MPI_Datatype, dest: c_int, sendtag: c_int, ref recvbuf, recvcount: c_int, recvtype: MPI_Datatype, source: c_int, recvtag: MPI_Datatype, comm: MPI_Comm, ref status: MPI_Status): c_int

.. function:: proc MPI_Sendrecv_replace(ref buf, count: c_int, datatype: MPI_Datatype, dest: c_int, sendtag: c_int, source: c_int, recvtag: c_int, comm: MPI_Comm, ref status: MPI_Status): c_int

.. function:: proc MPI_Type_contiguous(count: c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Type_vector(count: c_int, blocklength: c_int, stride: c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Type_hvector(count: c_int, blocklength: c_int, stride: MPI_Aint, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Type_indexed(count: c_int, array_of_blocklengths: [] c_int, array_of_displacements: [] c_int, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Type_hindexed(count: c_int, array_of_blocklengths: [] c_int, array_of_displacements: [] MPI_Aint, oldtype: MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Type_struct(count: c_int, array_of_blocklengths: [] c_int, array_of_displacements: [] MPI_Aint, array_of_types: [] MPI_Datatype, ref newtype: MPI_Datatype): c_int

.. function:: proc MPI_Address(ref location, ref address: MPI_Aint): c_int

.. function:: proc MPI_Type_extent(datatype: MPI_Datatype, ref extent: MPI_Aint): c_int

.. function:: proc MPI_Type_size(datatype: MPI_Datatype, ref size: c_int): c_int

.. function:: proc MPI_Type_lb(datatype: MPI_Datatype, ref displacement: MPI_Aint): c_int

.. function:: proc MPI_Type_ub(datatype: MPI_Datatype, ref displacement: MPI_Aint): c_int

.. function:: proc MPI_Type_commit(ref datatype: MPI_Datatype): c_int

.. function:: proc MPI_Type_free(ref datatype: MPI_Datatype): c_int

.. function:: proc MPI_Get_elements(ref status: MPI_Status, datatype: MPI_Datatype, ref count: c_int): c_int

.. function:: proc MPI_Pack(ref inbuf, incount: c_int, datatype: MPI_Datatype, ref outbuf, outsize: c_int, ref position: c_int, comm: MPI_Comm): c_int

.. function:: proc MPI_Unpack(ref inbuf, insize: c_int, ref position: c_int, ref outbuf, outcount: c_int, datatype: MPI_Datatype, comm: MPI_Comm): c_int

.. function:: proc MPI_Pack_size(incount: c_int, datatype: MPI_Datatype, comm: MPI_Comm, ref size: c_int): c_int

.. function:: proc MPI_Cart_create(comm_old: MPI_Comm, ndims: c_int, ref dims: c_int, ref periods: c_int, reorder: c_int, ref comm_cart: MPI_Comm): c_int

.. function:: proc MPI_Dims_create(nnodes: c_int, ndims: c_int, ref dims: c_int): c_int

.. function:: proc MPI_Graph_create(comm_old: MPI_Comm, nnodes: c_int, ref iindex: c_int, ref edges: c_int, reorder: c_int, ref comm_graph: MPI_Comm): c_int

.. function:: proc MPI_Topo_test(comm: MPI_Comm, ref status: c_int): c_int

.. function:: proc MPI_Graphdims_get(comm: MPI_Comm, ref nnodes: c_int, ref nedges: c_int): c_int

.. function:: proc MPI_Graph_get(comm: MPI_Comm, maxindex: c_int, maxedges: c_int, ref iindex: c_int, ref edges: c_int): c_int

.. function:: proc MPI_Cartdim_get(comm: MPI_Comm, ref ndims: c_int): c_int

.. function:: proc MPI_Cart_get(comm: MPI_Comm, maxdims: c_int, ref dims: c_int, ref periods: c_int, ref coords: c_int): c_int

.. function:: proc MPI_Cart_rank(comm: MPI_Comm, ref coords: c_int, ref rank: c_int): c_int

.. function:: proc MPI_Cart_coords(comm: MPI_Comm, rank: c_int, maxdims: c_int, ref coords: c_int): c_int

.. function:: proc MPI_Graph_neighbors_count(comm: MPI_Comm, rank: c_int, ref nneighbors: c_int): c_int

.. function:: proc MPI_Graph_neighbors(comm: MPI_Comm, rank: c_int, maxneighbors: c_int, ref neighbors: c_int): c_int

.. function:: proc MPI_Cart_shift(comm: MPI_Comm, direction: c_int, disp: c_int, ref rank_source: c_int, ref rank_dest: c_int): c_int

.. function:: proc MPI_Cart_sub(comm: MPI_Comm, ref remain_dims: c_int, ref newcomm: MPI_Comm): c_int

.. function:: proc MPI_Cart_map(comm: MPI_Comm, ndims: c_int, ref dims: c_int, ref periods: c_int, ref newrank: c_int): c_int

.. function:: proc MPI_Graph_map(comm: MPI_Comm, nnodes: c_int, ref iindex: c_int, ref edges: c_int, ref newrank: c_int): c_int

